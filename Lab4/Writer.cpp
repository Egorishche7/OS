#include <iostream>
#include <Windows.h>

using namespace std;

int main(int argc, char** argv)
{
	HANDLE Mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"Mutex");
	if (Mutex == nullptr)
	{
		cout << "Writer: Mutex opening failed\n";
		system("pause");
		return GetLastError();
	}

	WaitForSingleObject(Mutex, INFINITE);
	cout << "Writer: Active process\n";

	HANDLE Event_Writer_A = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Writer_A");
	if (Event_Writer_A == nullptr)
	{
		cout << "Writer: Event_Writer_A opening failed\n";
		system("pause");
		return GetLastError();
	}
	HANDLE Event_Writer_B = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Writer_B");
	if (Event_Writer_B == nullptr)
	{
		cout << "Writer: Event_Writer_B opening failed\n";
		system("pause");
		return GetLastError();
	}
	HANDLE Event_Writer_C = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Writer_C");
	if (Event_Writer_C == nullptr)
	{
		cout << "Writer: Event_Writer_C opening failed\n";
		system("pause");
		return GetLastError();
	}

	HANDLE Event_Writer_Ended = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Writer_Ended");
	if (Event_Writer_Ended == nullptr)
	{
		cout << "Writer: End opening failed\n";
		system("pause");
		return GetLastError();
	}

	for (int i = 0; i < atoi(argv[1]); i++)
	{
		char c;
		cout << "Writer: Enter 'A', or 'B', or 'C': ";
		cin >> c;
		if (c == 'A')
			SetEvent(Event_Writer_A);
		else if (c == 'B')
			SetEvent(Event_Writer_B);
		else if (c == 'C')
			SetEvent(Event_Writer_C);
	}

	SetEvent(Event_Writer_Ended);
	ReleaseMutex(Mutex);

	CloseHandle(Mutex);
	CloseHandle(Event_Writer_A);
	CloseHandle(Event_Writer_B);
	CloseHandle(Event_Writer_C);
	CloseHandle(Event_Writer_Ended);
	system("pause");

	return 0;
}