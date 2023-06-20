#include <iostream>
#include <Windows.h>
#include <process.h>

const int MINIMAL_ARRAY_SIZE = 1;
const DWORD START_TIME = 50;
const DWORD STOP_TIME = 3000;
const DWORD TIME_OUT = 20;

struct Pair {
	int* arr = nullptr;
	int size = 0;
};

void initArray(Pair& p) {
	int size;
	do {
		std::cout << "Enter array size: ";
		std::cin >> size;
	} while (size < MINIMAL_ARRAY_SIZE && std::cout << "Array size must be positive.\n");
	p.size = size;
	p.arr = new int[size];

	std::cout << "Enter array elements:\n";
	for (int i = 0; i < size; i++) {
		std::cin >> p.arr[i];
	}
}

int findSum(Pair* p) {
	int sum = 0;
	for (int i = 0; i < p->size; i++) {
		sum += p->arr[i];
		Sleep(TIME_OUT);
	}
	return sum;
}

UINT WINAPI worker(void* ptrPair) {
	std::cout << "Thread is started.\n";

	Pair* p = reinterpret_cast<Pair*>(ptrPair);
	std::cout << "Sum: " << findSum(p) << "\n";

	std::cout << "Thread is finished.\n";
	return 0;
}

int main() {

	Pair p;
	initArray(p);

	HANDLE hThread;
	UINT IDThread;
	hThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, worker, &p, 0, &IDThread));
	if (nullptr == hThread) {
		return GetLastError();
	}

	Sleep(START_TIME);
	SuspendThread(hThread);
	Sleep(STOP_TIME);
	ResumeThread(hThread);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	delete[] p.arr;
	p.arr = nullptr;

	return 0;
}
