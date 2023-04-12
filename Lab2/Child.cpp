#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Error!" << endl;
        exit(1);
    }

    int size = atoi(argv[1]);
    cout << "Source array:" << endl;
    for (int i = 0; i < size; i++)
        cout << "array[" << i + 1 << "]=" << atof(argv[i + 2]) << endl;

    double* newArr = new double[size];
    for (int i = 0; i < size; i++)
        newArr[i] = atof(argv[i + 2]);

    double temp;
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (newArr[j] > newArr[j + 1])
            {
                temp = newArr[j];
                newArr[j] = newArr[j + 1];
                newArr[j + 1] = temp;
            }

    cout << "\nSorted array:" << endl;
    for (int i = 0; i < size; i++)
        cout << "array[" << i + 1 << "]=" << newArr[i] << endl;

    delete[] newArr;
    newArr = nullptr;

    system("pause");

    return 0;
}