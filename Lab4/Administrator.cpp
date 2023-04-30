#include <iostream>
#include <Windows.h>
#include <string>

using namespace std;

int main()
{
	HANDLE Mutex = CreateMutex(nullptr, FALSE, L"Mutex");

	HANDLE Semaphore = CreateSemaphore(nullptr, 1, 1, L"Semaphore");

	HANDLE Event_Reader_A = CreateEvent(nullptr, FALSE, FALSE, L"Reader_A");
	HANDLE Event_Reader_B = CreateEvent(nullptr, FALSE, FALSE, L"Reader_B");
	HANDLE Event_Reader_C = CreateEvent(nullptr, FALSE, FALSE, L"Reader_C");
	HANDLE* Events_Writer = new HANDLE[3];
	Events_Writer[0] = CreateEvent(nullptr, FALSE, FALSE, L"Writer_A");
	Events_Writer[1] = CreateEvent(nullptr, FALSE, FALSE, L"Writer_B");
	Events_Writer[2] = CreateEvent(nullptr, FALSE, FALSE, L"Writer_C");
	HANDLE Event_Writer_Ended = CreateEvent(nullptr, FALSE, FALSE, L"Writer_Ended");
	HANDLE Event_Reader_Ended = CreateEvent(nullptr, FALSE, FALSE, L"Reader_Ended");

	int n, m;
	cout << "Administrator: Enter number of processes: ";
	cin >> n;
	cout << "Administrator: Enter number of sended messages for Writer (and recieved for Reader): ";
	cin >> m;

	STARTUPINFO* si_w = new STARTUPINFO[n];
	STARTUPINFO* si_r = new STARTUPINFO[n];
	PROCESS_INFORMATION* pi_w = new PROCESS_INFORMATION[n];
	PROCESS_INFORMATION* pi_r = new PROCESS_INFORMATION[n];

	wstring str_w = L"\"Writer.exe\" ";
	str_w += to_wstring(m);
	wchar_t* wstr_w = new wchar_t[str_w.length()];
	wcscpy_s(wstr_w, str_w.length() + 1, str_w.c_str());

	wstring str_r = L"\"Reader.exe\" ";
	str_r += to_wstring(m);
	wchar_t* wstr_r = new wchar_t[str_r.length()];
	wcscpy_s(wstr_r, str_r.length() + 1, str_r.c_str());

	for (int i = 0; i < n; i++)
	{
		ZeroMemory(&si_w[i], sizeof(STARTUPINFO));
		ZeroMemory(&si_r[i], sizeof(STARTUPINFO));
		if (!CreateProcess(nullptr, wstr_w, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si_w[i], &pi_w[i]))
		{
			cout << "Administrator: Writer opening error";
			return GetLastError();
		}
		if (!CreateProcess(nullptr, wstr_r, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si_r[i], &pi_r[i]))
		{
			cout << "Administrator: Reader opening error";
			return GetLastError();
		}
	}

	for (int i = 0; i < n * m; i++)
	{
		int k = WaitForMultipleObjects(3, Events_Writer, FALSE, INFINITE);
		k -= WAIT_OBJECT_0;
		if (k == 0)
		{
			cout << "Administrator: A\n";
			SetEvent(Event_Reader_A);
		}
		else if (k == 1)
		{
			cout << "Administrator: B\n";
			SetEvent(Event_Reader_B);
		}
		else
		{
			cout << "Administrator: C\n";
			SetEvent(Event_Reader_C);
		}
		if ((i + 1) % m == 0)
		{
			WaitForSingleObject(Event_Writer_Ended, INFINITE);
			cout << "Administrator: Writer ended\n";
			WaitForSingleObject(Event_Reader_Ended, INFINITE);
			cout << "Administrator: Reader ended\n";
		}
	}

	CloseHandle(Mutex);
	CloseHandle(Semaphore);
	CloseHandle(Event_Reader_A);
	CloseHandle(Event_Reader_B);
	CloseHandle(Event_Reader_C);
	CloseHandle(Events_Writer[0]);
	CloseHandle(Events_Writer[1]);
	CloseHandle(Events_Writer[2]);
	CloseHandle(Event_Writer_Ended);
	CloseHandle(Event_Reader_Ended);

	delete[] si_w, si_r, pi_w, pi_r, str_w, str_r, Events_Writer;
	si_w = si_r = nullptr;
	pi_w = pi_r = nullptr;
	Events_Writer = nullptr;

	return 0;
}