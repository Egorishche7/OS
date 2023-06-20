#include <iostream>
#include <Windows.h>
#include <string>

const int MINIMAL_ARRAY_SIZE = 1;
const int MINIMAL_PRODUCT_VALUE = 1;
const int OFFSET = 1;
const int EVEN_NUMBERS_DIVISOR = 2;

unsigned int* arr;
int length;
int k;
unsigned int product;

HANDLE hEvent1, hEvent2;
CRITICAL_SECTION cs;

void initArray() {
    do {
        std::cout << "Enter array size: ";
        std::cin >> length;
    } while (length < MINIMAL_ARRAY_SIZE && std::cout << "Array size must be positive.\n");

    arr = new unsigned int[length];
    std::cout << "Enter array elements:\n";
    for (int i = 0; i < length; i++) {
        std::cin >> arr[i];
    }
}

void printArray(std::string text) {
    std::cout << text;
    for (int i = 0; i < length; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

void sort(int time) {
    unsigned int temp;
    for (int i = 0; i < length; i++) {
        if (0 == arr[i] % EVEN_NUMBERS_DIVISOR) {
            for (int j = i; j > 0; j--) {
                temp = arr[j - OFFSET];
                arr[j - OFFSET] = arr[j];
                arr[j] = temp;
            }
            Sleep(time);
        }
    }
}

unsigned int findProduct() {
    unsigned int res = MINIMAL_PRODUCT_VALUE;
    for (int i = 0; i < k; i++) {
        if (arr[i] != 0) {
            res *= arr[i];
        }
    }
    return res;
}

DWORD WINAPI work(LPVOID) {
    int time;
    do {
        std::cout << "Enter time interval (ms): ";
        std::cin >> time;
    } while (time < 0 && std::cout << "Time interval mustn't be negative.\n");

    sort(time);
    SetEvent(hEvent1);

    return 0;
}

DWORD WINAPI MultElement(LPVOID) {
    EnterCriticalSection(&cs);
    WaitForSingleObject(hEvent2, INFINITE);

    product = findProduct();
    LeaveCriticalSection(&cs);

    return 0;
}

int main() {

    InitializeCriticalSection(&cs);

    hEvent1 = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (NULL == hEvent1) {
        return GetLastError();
    }
    hEvent2 = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (NULL == hEvent2) {
        CloseHandle(hEvent1);
        return GetLastError();
    }

    initArray();
    printArray("\nSource array size: " + std::to_string(length) + "\nSource array:\n");

    HANDLE hThread_1;
    DWORD IDThread_1;
    hThread_1 = CreateThread(NULL, 0, work, NULL, CREATE_SUSPENDED, &IDThread_1);
    if (NULL == hThread_1) {
        CloseHandle(hEvent1);
        CloseHandle(hEvent2);
        return GetLastError();
    }

    do {
        std::cout << "\nEnter integer " << MINIMAL_ARRAY_SIZE << " <= k <= " << length << ": ";
        std::cin >> k;
    } while ((k < MINIMAL_ARRAY_SIZE || k > length)
        && std::cout << "k must be not less than " << MINIMAL_ARRAY_SIZE << " and not greater than " << length);

    ResumeThread(hThread_1);

    HANDLE hThread_2;
    DWORD IDThread_2;
    hThread_2 = CreateThread(NULL, 0, MultElement, NULL, 0, &IDThread_2);
    if (NULL == hThread_2) {
        CloseHandle(hEvent1);
        CloseHandle(hEvent2);
        CloseHandle(hThread_1);
        return GetLastError();
    }

    WaitForSingleObject(hEvent1, INFINITE);

    std::cout << "\nChanged array:\n";
    for (int i = 0; i < length; i++) {
        std::cout << arr[i] << " ";
        if (i == k - OFFSET) {
            SetEvent(hEvent2);
        }
    }
    EnterCriticalSection(&cs);
    std::cout << "\nProduct of changed array elements from arr[" << MINIMAL_ARRAY_SIZE
        << "] to arr[" << k << "]: " << product << "\n";
    LeaveCriticalSection(&cs);

    WaitForSingleObject(hThread_2, INFINITE);
    DeleteCriticalSection(&cs);
    CloseHandle(hThread_1);
    CloseHandle(hThread_2);
    CloseHandle(hEvent1);
    CloseHandle(hEvent2);

    delete[] arr;
    arr = nullptr;

    return 0;
}
