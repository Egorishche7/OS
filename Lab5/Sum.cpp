#include <iostream>
#include <Windows.h>
#include <conio.h>

const int INFO_ARRAY_SIZE = 2;
const int ARRAY_SIZE_INDEX = 0;
const int N_INDEX = 1;
const int READ_HANDLE_INDEX = 1;
const int WRIGHT_HANDLE_INDEX = 2;
const int ERROR_CODE_1 = 1;
const int MIN_NUMBER_OF_ARGS = 2;
const int OFFSET = 1;

int info[INFO_ARRAY_SIZE];
double* arr;
double* squares;

HANDLE hEvent;
HANDLE hReadPipe;
HANDLE hWritePipe;

int initHandles(char* argv[]) {
	hReadPipe = reinterpret_cast<HANDLE>(atoi(argv[READ_HANDLE_INDEX]));
	hWritePipe = reinterpret_cast<HANDLE>(atoi(argv[WRIGHT_HANDLE_INDEX]));

	hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"event");
	if (hEvent == nullptr) {
		std::cout << "Client: Event wasn't read.\n";
		system("pause");
		return GetLastError();
	}

	return 0;
}

void read(int& n, int& N) {
	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, info, sizeof(int) * INFO_ARRAY_SIZE, &dwBytesRead, nullptr)) {
		std::cout << "Client: Info wasn't read.\n";
	}

	n = info[ARRAY_SIZE_INDEX], N = info[N_INDEX];

	arr = new double[n];
	if (!ReadFile(hReadPipe, arr, sizeof(double) * n, &dwBytesRead, nullptr)) {
		std::cout << "Client: Array elements weren't read.\n";
	}
}

int findElemGreaterN(int n, int N) {
	int count = 0;
	std::cout << "Array elements > N:\n";
	for (int i = 0; i < n; i++) {
		if (arr[i] > N) {
			std::cout << arr[i] << "\n";
			count++;
		}
	}
	return count;
}

int findSum(int count, int n, int N) {
	std::cout << "New array (squares of array elements > N):\n";
	squares = new double[count];
	double sum = 0;
	for (int i = 0, j = 0; i < n; i++) {
		if (arr[i] > N) {
			squares[j] = arr[i] * arr[i];
			std::cout << "array[" << j + OFFSET << "]=" << squares[j] << "\n";
			sum += squares[j];
			j++;
		}
	}
	return sum;
}

void wright(int count, double sum) {
	DWORD dwBytesWritten;

	if (!WriteFile(hWritePipe, &count, sizeof(count), &dwBytesWritten, nullptr)) {
		std::cout << "Client: Squares array size wasn't written.\n";
	}
	if (!WriteFile(hWritePipe, squares, sizeof(double) * count, &dwBytesWritten, nullptr)) {
		std::cout << "Client: Squares wasn't written.\n";
	}
	if (!WriteFile(hWritePipe, &sum, sizeof(sum), &dwBytesWritten, nullptr)) {
		std::cout << "Client: Sum wasn't written.\n";
	}

	std::cout << "Sum of squares: " << sum << "\n";
}

void releaseResources() {
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(hEvent);

	delete[] arr, squares;
	arr = squares = nullptr;
}

int main(int argc, char* argv[]) {

	if (argc < MIN_NUMBER_OF_ARGS) {
		std::cerr << "Missing command line arguments!\n";
		exit(ERROR_CODE_1);
	}

	int flag = initHandles(argv);
	if (flag != 0) {
		return flag;
	}

	int n, N;
	read(n, N);

	int count = findElemGreaterN(n, N);
	double sum = findSum(count, n, N);
	wright(count, sum);

	SetEvent(hEvent);
	system("pause");

	return 0;
}
