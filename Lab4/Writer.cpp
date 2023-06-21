#include <iostream>
#include <Windows.h>

const int NUM_OF_MESSAGES_INDEX = 1;

HANDLE mutex;
HANDLE eventWriterA;
HANDLE eventWriterB;
HANDLE eventWriterC;
HANDLE eventWriterEnded;

int openEvents() {
	eventWriterA = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"writerA");
	if (nullptr == eventWriterA) {
		std::cout << "Writer: eventWriterA opening failed\n";
		system("pause");
		return GetLastError();
	}
	eventWriterB = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"writerB");
	if (nullptr == eventWriterB) {
		std::cout << "Writer: eventWriterB opening failed\n";
		system("pause");
		return GetLastError();
	}
	eventWriterC = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"writerC");
	if (nullptr == eventWriterC) {
		std::cout << "Writer: eventWriterC opening failed\n";
		system("pause");
		return GetLastError();
	}

	eventWriterEnded = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"writerEnded");
	if (nullptr == eventWriterEnded) {
		std::cout << "Writer: End opening failed\n";
		system("pause");
		return GetLastError();
	}

	return 0;
}

void startMessageWriting(int m) {
	for (int i = 0; i < m; i++) {
		char c;
		std::cout << "Writer: Enter 'A', or 'B', or 'C': ";
		std::cin >> c;
		if (c == 'A') {
			SetEvent(eventWriterA);
		} else if (c == 'B') {
			SetEvent(eventWriterB);
		} else if (c == 'C') {
			SetEvent(eventWriterC);
		}
	}
}

void closeHandles() {
	CloseHandle(mutex);
	CloseHandle(eventWriterA);
	CloseHandle(eventWriterB);
	CloseHandle(eventWriterC);
	CloseHandle(eventWriterEnded);
}

int main(int argc, char* argv[]) {

	mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"mutex");
	if (nullptr == mutex) {
		std::cout << "Writer: Mutex opening failed\n";
		system("pause");
		return GetLastError();
	}

	WaitForSingleObject(mutex, INFINITE);
	std::cout << "Writer: Active process\n";

	int flag = openEvents();
	if (flag != 0) {
		return flag;
	}

	startMessageWriting(atoi(argv[NUM_OF_MESSAGES_INDEX]));

	SetEvent(eventWriterEnded);
	ReleaseMutex(mutex);

	closeHandles();
	system("pause");

	return 0;
}
