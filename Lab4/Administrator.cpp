#include <iostream>
#include <Windows.h>
#include <string>

const int ARRAY_SIZE = 3;
const int A_INDEX = 0;
const int B_INDEX = 1;
const int C_INDEX = 2;
const int MIN_SEMAPHORE_VALUE = 1;
const int MAX_SEMAPHORE_VALUE = 1;
const int MIN_NUMBER_OF_PROCESSES = 1;
const int MIN_NUMBER_OF_MESSAGES = 1;
const int OFFSET = 1;

STARTUPINFO* siW;
STARTUPINFO* siR;
PROCESS_INFORMATION* piW;
PROCESS_INFORMATION* piR;

HANDLE mutex;
HANDLE semaphore;
HANDLE eventReaderA;
HANDLE eventReaderB;
HANDLE eventReaderC;
HANDLE* eventsWriter;
HANDLE eventWriterEnded;
HANDLE eventReaderEnded;

void createSynchObj() {
	mutex = CreateMutex(nullptr, FALSE, L"mutex");

	semaphore = CreateSemaphore(nullptr, MIN_SEMAPHORE_VALUE, MAX_SEMAPHORE_VALUE, L"semaphore");

	eventReaderA = CreateEvent(nullptr, FALSE, FALSE, L"readerA");
	eventReaderB = CreateEvent(nullptr, FALSE, FALSE, L"readerB");
	eventReaderC = CreateEvent(nullptr, FALSE, FALSE, L"readerC");

	eventsWriter = new HANDLE[ARRAY_SIZE];
	eventsWriter[A_INDEX] = CreateEvent(nullptr, FALSE, FALSE, L"writerA");
	eventsWriter[B_INDEX] = CreateEvent(nullptr, FALSE, FALSE, L"writerB");
	eventsWriter[C_INDEX] = CreateEvent(nullptr, FALSE, FALSE, L"writerC");

	eventWriterEnded = CreateEvent(nullptr, FALSE, FALSE, L"writerEnded");
	eventReaderEnded = CreateEvent(nullptr, FALSE, FALSE, L"readerEnded");
}

void initSourceData(int& n, int& m) {
	do {
		std::cout << "Administrator: Enter number of processes: ";
		std::cin >> n;
	} while (n < MIN_NUMBER_OF_PROCESSES
		&& std::cout << "Administrator: Number of processes must be positive\n");
	do {
		std::cout << "Administrator: Enter number of sended messages for Writer (and recieved for Reader): ";
		std::cin >> m;
	} while (m < MIN_NUMBER_OF_MESSAGES
		&& std::cout << "Administrator: number of sended messages for Writer (and recieved for Reader) must be positive\n");

	siW = new STARTUPINFO[n];
	siR = new STARTUPINFO[n];
	piW = new PROCESS_INFORMATION[n];
	piR = new PROCESS_INFORMATION[n];
}

wchar_t* buildCommandLine(std::wstring str, int m) {
	std::wstring line = str;
	line += std::to_wstring(m) + L" ";

	wchar_t* cmd = new wchar_t[line.size()];
	wcscpy_s(cmd, line.size() + OFFSET, line.c_str());

	return cmd;
}

int runAllProcesses(int n, wchar_t* cmd1, wchar_t* cmd2) {
	for (int i = 0; i < n; i++) {
		ZeroMemory(&siW[i], sizeof(STARTUPINFO));
		ZeroMemory(&siR[i], sizeof(STARTUPINFO));
		if (!CreateProcess(nullptr, cmd1, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &siW[i], &piW[i])) {
			std::cout << "Administrator: Writer opening error";
			return GetLastError();
		}
		if (!CreateProcess(nullptr, cmd2, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &siR[i], &piR[i])) {
			std::cout << "Administrator: Reader opening error";
			return GetLastError();
		}
	}

	return 0;
}

void startMessageTranslation(int n, int m) {
	int k;
	for (int i = 0; i < n * m; i++) {
		k = WaitForMultipleObjects(ARRAY_SIZE, eventsWriter, FALSE, INFINITE);
		k -= WAIT_OBJECT_0;
		if (k == A_INDEX) {
			std::cout << "Administrator: A\n";
			SetEvent(eventReaderA);
		} else if (k == B_INDEX) {
			std::cout << "Administrator: B\n";
			SetEvent(eventReaderB);
		} else {
			std::cout << "Administrator: C\n";
			SetEvent(eventReaderC);
		}
		if ((i + OFFSET) % m == 0) {
			WaitForSingleObject(eventWriterEnded, INFINITE);
			std::cout << "Administrator: Writer ended\n";
			WaitForSingleObject(eventReaderEnded, INFINITE);
			std::cout << "Administrator: Reader ended\n";
		}
	}
}

void releaseResources(wchar_t* cmd1, wchar_t* cmd2) {
	CloseHandle(mutex);
	CloseHandle(semaphore);
	CloseHandle(eventReaderA);
	CloseHandle(eventReaderB);
	CloseHandle(eventReaderC);
	CloseHandle(eventsWriter[A_INDEX]);
	CloseHandle(eventsWriter[B_INDEX]);
	CloseHandle(eventsWriter[C_INDEX]);
	CloseHandle(eventWriterEnded);
	CloseHandle(eventReaderEnded);

	delete[] siW, siR, piW, piR, cmd1, cmd2, eventsWriter;
	siW = siR = nullptr;
	piW = piR = nullptr;
	cmd1 = cmd2 = nullptr;
	eventsWriter = nullptr;
}

int main() {

	createSynchObj();

	int n, m;
	initSourceData(n, m);

	wchar_t* cmdW = buildCommandLine(L"\"Writer.exe\" ", m);
	wchar_t* cmdR = buildCommandLine(L"\"Reader.exe\" ", m);

	int flag = runAllProcesses(n, cmdW, cmdR);
	if (flag != 0) {
		return flag;
	}

	startMessageTranslation(n, m);

	releaseResources(cmdW, cmdR);

	return 0;
}
