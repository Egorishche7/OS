#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.dwFlags = STARTF_USECOUNTCHARS;
    si.dwYCountChars = 600;

    int size;
    cout << "Input array size: ";
    cin >> size;
    double* arr = new double[size];

    cout << "Input array elements:" << endl;
    for (int i = 0; i < size; i++)
    {
        cout << "array[" << i + 1 << "]=";
        cin >> arr[i];
    }

    wstring line1 = L"\"Child.exe\" ";
    line1 += to_wstring(size) + L" ";
    for (int i = 0; i < size; i++)
        line1 += to_wstring(arr[i]) + L" ";

    wchar_t* line2 = new wchar_t[line1.size()];
    wcscpy_s(line2, line1.size() + 1, line1.c_str());

    if (!CreateProcess(NULL, line2, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    {
        cerr << "\nProcess is not created.";
        return GetLastError();
    }
    else
    {
        cout << "\nProcess is created." << endl;
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    delete[] arr, line2;
    arr = nullptr;
    line2 = nullptr;

    return 0;
}