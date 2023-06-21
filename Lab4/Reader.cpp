#include <iostream>
#include <Windows.h>

const int ARRAY_SIZE = 3;
const int A_INDEX = 0;
const int B_INDEX = 1;
const int C_INDEX = 2;
const int NUM_OF_MESSAGES_INDEX = 1;
const int SEMAPHORE_DECREASE_STEP = 1;

HANDLE semaphore;
HANDLE* eventsReader;
HANDLE eventReaderEnded;

int openEvents() {
	eventsReader = new HANDLE[ARRAY_SIZE];
	eventsReader[A_INDEX] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"readerA");
	if (nullptr == eventsReader[A_INDEX]) {
		std::cout << "Reader: Event_A opening failed\n";
		system("pause");
		return GetLastError();
	}
	eventsReader[B_INDEX] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"readerB");
	if (nullptr == eventsReader[B_INDEX]) {
		std::cout << "Reader: Event_B opening failed\n";
		system("pause");
		return GetLastError();
	}
	eventsReader[C_INDEX] = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"readerC");
	if (nullptr == eventsReader[C_INDEX]) {
		std::cout << "Reader: Event_C opening failed\n";
		system("pause");
		return GetLastError();
	}

	eventReaderEnded = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"readerEnded");
	if (nullptr == eventReaderEnded) {
		std::cout << "Writer: End opening failed\n";
		system("pause");
		return GetLastError();
	}

	return 0;
}

void startMessageReading(int m) {
	for (int i = 0; i < m; i++) {
		int k = WaitForMultipleObjects(ARRAY_SIZE, eventsReader, FALSE, INFINITE);
		k -= WAIT_OBJECT_0;
		if (k == A_INDEX) {
			std::cout << "Reader: A\n";
		} else if (k == B_INDEX) {
			std::cout << "Reader: B\n";
		} else {
			std::cout << "Reader: C\n";
		}
	}
}

void releaseResources() {
	CloseHandle(semaphore);
	CloseHandle(eventsReader[A_INDEX]);
	CloseHandle(eventsReader[B_INDEX]);
	CloseHandle(eventsReader[C_INDEX]);
	CloseHandle(eventReaderEnded);

	delete[] eventsReader;
	eventsReader = nullptr;
}

int main(int args, char* argv[]) {

	semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"semaphore");
	if (semaphore == nullptr) {
		std::cout << "Reader: Semaphore opening failed\n";
		system("pause");
		return GetLastError();
	}

	WaitForSingleObject(semaphore, INFINITE);
	std::cout << "Reader: Active process\n";

	int flag = openEvents();
	if (flag != 0) {
		return flag;
	}

	startMessageReading(atoi(argv[NUM_OF_MESSAGES_INDEX]));

	ReleaseSemaphore(semaphore, SEMAPHORE_DECREASE_STEP, nullptr);
	SetEvent(eventReaderEnded);

	releaseResources();
	system("pause");

	return 0;
}
