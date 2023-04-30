#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

struct Pair
{
	int* arr = nullptr;
	int size = 0;
};

UINT WINAPI worker(void* pPair)
{
	cout << "Thread is started." << endl;

	Pair* p = reinterpret_cast<Pair*>(pPair);
	int sum = 0;
	for (int i = 0; i < p->size; i++)
	{
		sum += p->arr[i];
		Sleep(20);
	}
	cout << "Sum: " << sum << endl;
	cout << "Thread is finished." << endl;

	return 0;
}

int main()
{
	HANDLE hThread;
	UINT IDThread;

	Pair p;
	int size;
	cout << "Enter array size: ";
	cin >> size;
	p.size = size;
	p.arr = new int[size];

	cout << "Enter array elements:" << endl;
	for (int i = 0; i < size; i++)
	{
		cout << "array[" << i + 1 << "]=";
		cin >> p.arr[i];
	}

	hThread = (HANDLE)_beginthreadex(NULL, 0, worker, &p, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();

	Sleep(20);
	SuspendThread(hThread);
	Sleep(5000);
	ResumeThread(hThread);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	delete[] p.arr;
	p.arr = nullptr;

	return 0;
}
