#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

int main()
{
	int info[2];
	cout << "Enter array size: ";
	cin >> info[0];

	double* arr = new double[info[0]];
	srand(time(0));
	for (int i = 0; i < info[0]; i++)
		arr[i] = ((double)rand() / RAND_MAX) * 20;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hEvent;
	HANDLE hReadPipe, hWritePipe;
	SECURITY_ATTRIBUTES sa;

	hEvent = CreateEvent(nullptr, FALSE, FALSE, L"Event");
	if (hEvent == nullptr)
	{
		cout << "Server: Event wasn't created.";
		return GetLastError();
	}

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		cout << "Server: Pipe creation failed.";
		return GetLastError();
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	wchar_t* line = new wchar_t[80];
	wsprintf(line, L"\"Sum.exe\" %d %d", (int)hReadPipe, (int)hWritePipe);
	if (!CreateProcess(nullptr, line, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE, FALSE, FALSE, &si, &pi))
	{
		cout << "Server: Client creation failed.";
		return GetLastError();
	}

	cout << "Enter number N: ";
	cin >> info[1];

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, info, sizeof(int) * 2, &dwBytesWritten, nullptr))
	{
		cout << "Server: Info wasn't written.";
		return GetLastError();
	}
	if (!WriteFile(hWritePipe, arr, sizeof(double) * info[0], &dwBytesWritten, nullptr))
	{
		cout << "Server: Array elements weren't written.";
		return GetLastError();
	}

	WaitForSingleObject(hEvent, INFINITE);

	int m;
	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, &m, sizeof(int), &dwBytesRead, nullptr))
	{
		cout << "Server: Squares array size wasn't read.";
		return GetLastError();
	}
	double* squares = new double[m];
	if (!ReadFile(hReadPipe, squares, sizeof(double) * m, &dwBytesRead, nullptr))
	{
		cout << "Server: Squares weren't read.";
		return GetLastError();
	}
	double sum;
	if (!ReadFile(hReadPipe, &sum, sizeof(double), &dwBytesRead, nullptr))
	{
		cout << "Server: Sum wasn't read.";
		return GetLastError();
	}

	cout << "\nArray size: " << info[0] << endl;
	cout << "N = " << info[1] << endl;
	cout << "Source array:" << endl;
	for (int i = 0; i < info[0]; i++)
		cout << "array[" << i + 1 << "]=" << arr[i] << endl;
	cout << "\nNew array (squares of array elements > N):" << endl;
	for (int i = 0; i < m; i++)
		cout<< "array[" << i + 1 << "]=" << squares[i] << endl;
	cout << "Sum of squares: " << sum << endl;

	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(hEvent);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	delete[] arr, line, squares;
	arr = squares = nullptr;
	line = nullptr;

	return 0;
}