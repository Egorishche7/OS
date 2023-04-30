#include <iostream>
#include <Windows.h>

using namespace std;

int main(int args, char** argv)
{
	HANDLE Semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"Semaphore");
	if (Semaphore == nullptr)
	{
		cout << "Reader: Semaphore opening failed\n";
		system("pause");
		return GetLastError();
	}

	WaitForSingleObject(Semaphore, INFINITE);
	cout << "Reader: Active process\n";

	HANDLE* Events_Reader = new HANDLE[3];
	Events_Reader[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Reader_A");
	if (Events_Reader[0] == nullptr)
	{
		cout << "Reader: Event_A opening failed\n";
		system("pause");
		return GetLastError();
	}
	Events_Reader[1] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Reader_B");
	if (Events_Reader[1] == nullptr)
	{
		cout << "Reader: Event_B opening failed\n";
		system("pause");
		return GetLastError();
	}
	Events_Reader[2] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Reader_C");
	if (Events_Reader[2] == nullptr)
	{
		cout << "Reader: Event_C opening failed\n";
		system("pause");
		return GetLastError();
	}

	HANDLE Event_Reader_Ended = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Reader_Ended");
	if (Event_Reader_Ended == nullptr)
	{
		cout << "Writer: End opening failed\n";
		system("pause");
		return GetLastError();
	}

	for (int i = 0; i < atoi(argv[1]); i++)
	{
		int k = WaitForMultipleObjects(3, Events_Reader, FALSE, INFINITE);
		k -= WAIT_OBJECT_0;
		if (k == 0)
		{
			cout << "Reader: A\n";
			ResetEvent(Events_Reader[0]);
		}
		else if (k == 1)
		{
			cout << "Reader: B\n";
			ResetEvent(Events_Reader[1]);
		}
		else
		{
			cout << "Reader: C\n";
			ResetEvent(Events_Reader[2]);
		}
	}

	ReleaseSemaphore(Semaphore, 1, nullptr);
	SetEvent(Event_Reader_Ended);

	CloseHandle(Semaphore);
	CloseHandle(Events_Reader[0]);
	CloseHandle(Events_Reader[1]);
	CloseHandle(Events_Reader[2]);
	CloseHandle(Event_Reader_Ended);
	system("pause");

	return 0;
}