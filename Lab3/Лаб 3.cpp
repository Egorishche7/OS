#include <iostream>
#include <Windows.h>

using namespace std;

volatile unsigned int* arr;
int length;
int k;
unsigned int product;
HANDLE hAddEvent, hOutEvent;
CRITICAL_SECTION cs;

DWORD WINAPI work(LPVOID)
{
    int time;
    cout << "Enter time interval: ";
    cin >> time;

    unsigned int temp;
    for (int i = 0; i < length; i++)
        if (arr[i] % 2 == 0)
            for (int j = i; j > 0; j--)
            {
                temp = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = temp;
            }
            Sleep(time);

    SetEvent(hOutEvent);

    return 0;
}

DWORD WINAPI MultElement(LPVOID)
{
    EnterCriticalSection(&cs);
    WaitForSingleObject(hAddEvent, INFINITE);

    unsigned int res = 1;
    for (int i = 0; i < k; i++)
        if (arr[i] != 0)
            res *= arr[i];
    product = res;

    LeaveCriticalSection(&cs);

    return 0;
}

int main()
{
    HANDLE hThread_1, hThread_2;
    DWORD IDThread_1, IDThread_2;

    InitializeCriticalSection(&cs);

    hAddEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hAddEvent == NULL)
        return GetLastError();
    hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hOutEvent == NULL)
        return GetLastError();

    int size;
    cout << "Enter array size: ";
    cin >> size;
    arr = new unsigned int[size];
    length = size;

    unsigned int temp;
    cout << "Enter array elements:" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << "array[" << i + 1 << "]=";
        cin >> temp;
        arr[i] = temp;
    }

    cout << "\nSource array size: " << size << endl;
    cout << "Source array: " << endl;
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";

    hThread_1 = CreateThread(NULL, 0, work, NULL, CREATE_SUSPENDED, &IDThread_1);
    if (hThread_1 == NULL)
        return GetLastError();

    cout << "\nEnter integer 1 <= k <= " << size << ": ";
    cin >> k;

    ResumeThread(hThread_1);

    hThread_2 = CreateThread(NULL, 0, MultElement, NULL, 0, &IDThread_2);
    if (hThread_2 == NULL)
        return GetLastError();

    WaitForSingleObject(hOutEvent, INFINITE);

    cout << "\nChanged array: " << endl;
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
        if (i == k - 1)
            SetEvent(hAddEvent);
    }
    EnterCriticalSection(&cs);
    cout << "\nProduct of changed array elements from arr[1] to arr[" << k << "]: " << product << endl;
    LeaveCriticalSection(&cs);

    WaitForSingleObject(hThread_2, INFINITE);
    DeleteCriticalSection(&cs);
    CloseHandle(hThread_1);
    CloseHandle(hThread_2);
    CloseHandle(hOutEvent);
    CloseHandle(hAddEvent);

    delete[] arr;
    arr = nullptr;

    return 0;
}
