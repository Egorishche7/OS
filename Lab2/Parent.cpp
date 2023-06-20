#include <iostream>
#include <Windows.h>
#include <string>

const DWORD SCREEN_BUFFER_HEIGHT = 600;

int size;
double* arr;

void initArray() {
    do {
        std::cout << "Enter array size: ";
        std::cin >> size;
    } while (size < 1 && std::cout << "Array size must be positive.\n");

    arr = new double[size];
    std::cout << "Enter array elements:\n";
    for (int i = 0; i < size; i++) {
        std::cin >> arr[i];
    }
}

void setScreenBufferHeight(STARTUPINFO& si) {
    si.dwFlags = STARTF_USECOUNTCHARS;
    si.dwYCountChars = SCREEN_BUFFER_HEIGHT;
}

wchar_t* buildCommandLine() {
    std::wstring line = L"\"Child.exe\" ";
    line += std::to_wstring(size) + L" ";
    for (int i = 0; i < size; i++) {
        line += std::to_wstring(arr[i]) + L" ";
    }

    wchar_t* cmd = new wchar_t[line.size()];
    wcscpy_s(cmd, line.size() + 1, line.c_str());

    return cmd;
}

int main() {

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    setScreenBufferHeight(si);

    initArray();

    wchar_t* cmd = buildCommandLine();

    if (!CreateProcess(nullptr, cmd, nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &si, &pi)) {
        std::cerr << "\nProcess is not created.";
        return GetLastError();
    }
    else {
        std::cout << "\nProcess is created.\n";
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    delete[] arr, cmd;
    arr = nullptr;
    cmd = nullptr;

    return 0;
}
