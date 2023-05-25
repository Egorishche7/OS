#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <Windows.h>

using namespace std;

volatile unsigned int* arr;
int length;
int k;
unsigned int product;
condition_variable cv1, cv2;
mutex mtx1, mtx2;
bool ready1 = false, ready2 = false;
mutex mtx;

void work()
{
    int time;
    cout << "Enter time interval: ";
    cin >> time;

    unsigned int temp;
    for (int i = 0; i < length; i++)
        if (arr[i] % 2 == 0)
        {
            for (int j = i; j > 0; j--)
            {
                temp = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = temp;
            }
            this_thread::sleep_for(chrono::milliseconds(time));
        }
    
    unique_lock<mutex> lock_mtx1(mtx1);
    ready1 = true;
    cv1.notify_one();
}

void MultElement()
{
    mtx.lock();
    unique_lock<mutex> lock_mtx2(mtx2);
    while (!ready2)
        cv2.wait(lock_mtx2);

    unsigned int res = 1;
    for (int i = 0; i < k; i++)
        if (arr[i] != 0)
            res *= arr[i];
    product = res;

    mtx.unlock();
}

int main()
{
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

    thread t1(work);
    SuspendThread(t1.native_handle());

    cout << "\nEnter integer 1 <= k <= " << size << ": ";
    cin >> k;

    ResumeThread(t1.native_handle());

    thread t2(MultElement);

    unique_lock<mutex> lock_mtx1(mtx1);
    while (!ready1)
        cv1.wait(lock_mtx1);

    cout << "\nChanged array: " << endl;
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
        if (i == k - 1)
        {
            unique_lock<mutex> lock_mtx2(mtx2);
            ready2 = true;
            cv2.notify_one();
        }
    }
    mtx.lock();
    cout << "\nProduct of changed array elements from arr[1] to arr[" << k << "]: " << product << endl;
    mtx.unlock();

    t1.join();
    t2.join();

    delete[] arr;
    arr = nullptr;

    return 0;
}