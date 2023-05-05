#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cerr << "Error";
		return 1;
	}

	HANDLE hReadPipe = (HANDLE)atoi(argv[1]);
	HANDLE hWritePipe = (HANDLE)atoi(argv[2]);
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Event");

	if (hEvent == nullptr)
	{
		cout << "Client: Event wasn't read.";
		return GetLastError();
	}

	int info[2];
	DWORD dwBytesRead;
	if (!ReadFile(hReadPipe, info, sizeof(int) * 2, &dwBytesRead, nullptr))
		cout << "Client: Info wasn't read.";

	int n = info[0], N = info[1];

	double* arr = new double[n];
	if (!ReadFile(hReadPipe, arr, sizeof(double) * n, &dwBytesRead, nullptr))
		cout << "Client: Array elements weren't read.";

	int m = 0;
	cout << "Array elements > N:" << endl;
	for (int i = 0; i < n; i++)
		if (arr[i] > N)
		{
			cout << arr[i] << endl;
			m++;
		}

	cout << "New array (squares of array elements > N):" << endl;
	double* squares = new double[m];
	double sum = 0;
	for (int i = 0, j = 0; i < n; i++)
		if (arr[i] > N)
		{
			squares[j] = arr[i] * arr[i];
			cout << "array[" << j + 1 << "]=" << squares[j] << endl;
			sum += squares[j];
			j++;
		}

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, &m, sizeof(m), &dwBytesWritten, nullptr))
		cout << "Client: Squares array size wasn't written.";
	if (!WriteFile(hWritePipe, squares, sizeof(double) * m, &dwBytesWritten, nullptr))
		cout << "Client: Squares wasn't written.";
	if (!WriteFile(hWritePipe, &sum, sizeof(sum), &dwBytesWritten, nullptr))
		cout << "Client: Sum wasn't written.";

	cout << "Sum of squares: " << sum << endl;

	SetEvent(hEvent);
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(hEvent);

	delete[] arr, squares;
	arr = squares = nullptr;

	system("pause");

	return 0;
}