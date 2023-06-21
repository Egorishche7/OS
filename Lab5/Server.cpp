#include <iostream>
#include <Windows.h>
#include <conio.h>

const int INFO_ARRAY_SIZE = 2;
const int ARRAY_SIZE_INDEX = 0;
const int N_INDEX = 1;
const int MAX_VALUE = 20;
const int OFFSET = 1;

int info[INFO_ARRAY_SIZE];
double* arr;
int count;
double* squares;
double sum;

HANDLE hEvent;
SECURITY_ATTRIBUTES sa;
HANDLE hReadPipe, hWritePipe;
STARTUPINFO si;
PROCESS_INFORMATION pi;

void generateArrElements() {
	srand(time(0));
	for (int i = 0; i < info[ARRAY_SIZE_INDEX]; i++) {
		arr[i] = ((double)rand() / RAND_MAX) * MAX_VALUE;
	}
}

void initData() {
	do {
		std::cout << "Enter array size: ";
		std::cin >> info[ARRAY_SIZE_INDEX];
	} while (info[ARRAY_SIZE_INDEX] < 0 && std::cout << "Array size must be positive.");

	arr = new double[info[ARRAY_SIZE_INDEX]];
	generateArrElements();
}

void createInheritHandle() {
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = TRUE;
}

wchar_t* buildCommandLine() {
	wchar_t* line = new wchar_t[80];
	wsprintf(line, L"\"Sum.exe\" %d %d", (int)hReadPipe, (int)hWritePipe);

	return line;
}

int write() {
	DWORD dwBytesWritten;

	if (!WriteFile(hWritePipe, info, sizeof(int) * INFO_ARRAY_SIZE, &dwBytesWritten, nullptr)) {
		std::cout << "Server: Info wasn't written.";
		return GetLastError();
	}
	if (!WriteFile(hWritePipe, arr, sizeof(double) * info[ARRAY_SIZE_INDEX], &dwBytesWritten, nullptr)) {
		std::cout << "Server: Array elements weren't written.";
		return GetLastError();
	}

	return 0;
}

int read() {
	DWORD dwBytesRead;

	if (!ReadFile(hReadPipe, &count, sizeof(int), &dwBytesRead, nullptr)) {
		std::cout << "Server: Squares array size wasn't read.";
		return GetLastError();
	}
	squares = new double[count];
	if (!ReadFile(hReadPipe, squares, sizeof(double) * count, &dwBytesRead, nullptr)) {
		std::cout << "Server: Squares weren't read.";
		return GetLastError();
	}
	if (!ReadFile(hReadPipe, &sum, sizeof(double), &dwBytesRead, nullptr)) {
		std::cout << "Server: Sum wasn't read.";
		return GetLastError();
	}

	return 0;
}

void printData() {
	std::cout << "\nArray size: " << info[ARRAY_SIZE_INDEX];
	std::cout << "\nN = " << info[N_INDEX];
	std::cout << "\nSource array:\n";
	for (int i = 0; i < info[ARRAY_SIZE_INDEX]; i++) {
		std::cout << "array[" << i + OFFSET << "]=" << arr[i] << "\n";
	}
	std::cout << "\nNew array (squares of array elements > N):\n";
	for (int i = 0; i < count; i++) {
		std::cout << "array[" << i + OFFSET << "]=" << squares[i] << "\n";
	}
	std::cout << "Sum of squares: " << sum << "\n";
}

void releaseResources(wchar_t* cmd) {
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(hEvent);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	delete[] arr, cmd, squares;
	arr = squares = nullptr;
	cmd = nullptr;
}

int main() {

	initData();
	createInheritHandle();

	hEvent = CreateEvent(nullptr, FALSE, FALSE, L"event");
	if (nullptr == hEvent) {
		std::cout << "Server: Event wasn't created.";
		return GetLastError();
	}

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
		std::cout << "Server: Pipe creation failed.";
		return GetLastError();
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	wchar_t* cmd = buildCommandLine();

	if (!CreateProcess(nullptr, cmd, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, FALSE, FALSE, &si, &pi)) {
		std::cout << "Server: Client creation failed.";
		return GetLastError();
	}

	std::cout << "Enter number N: ";
	std::cin >> info[N_INDEX];

	int flag1 = write();
	if (flag1 != 0) {
		return flag1;
	}

	WaitForSingleObject(hEvent, INFINITE);

	int flag2 = read();
	if (flag2 != 0) {
		return flag2;
	}

	printData();

	releaseResources(cmd);

	return 0;
}
