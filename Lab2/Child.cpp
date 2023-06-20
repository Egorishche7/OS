#include <iostream>

const int ARRAY_SIZE_INDEX = 1;
const int ERROR_CODE_1 = 1;
const int MIN_NUMBER_OF_ARGS = 2;
const int OFFSET = 2;

int size;
double* newArr;

void initNewArray(char* arrInfo[]) {
    size = atoi(arrInfo[ARRAY_SIZE_INDEX]);
    newArr = new double[size];
    for (int i = 0; i < size; i++) {
        newArr[i] = atof(arrInfo[i + OFFSET]);
    }
}

void printArray(std::string text) {
    std::cout << text;
    for (int i = 0; i < size; i++) {
        std::cout << newArr[i] << " ";
    }
    std::cout << "\n";
}

void bubbleSort() {
    double temp;
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (newArr[j] > newArr[j + 1]) {
                temp = newArr[j];
                newArr[j] = newArr[j + 1];
                newArr[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc < MIN_NUMBER_OF_ARGS) {
        std::cerr << "Missing command line arguments!\n";
        exit(ERROR_CODE_1);
    }

    initNewArray(argv);
    printArray("Source array:\n");

    bubbleSort();
    printArray("Sorted array:\n");

    delete[] newArr;
    newArr = nullptr;

    system("pause");

    return 0;
}
