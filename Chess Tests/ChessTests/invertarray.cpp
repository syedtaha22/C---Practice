#include <iostream>

const int PawnPST[64] = {
    0,   0,   0,   0,   0,   0,   0,   0,
    50,  50,  50,  50,  50,  50,  50,  50,
    10,  10,  20,  30,  30,  20,  10,  10,
    5,   5,  10,  25,  25,  10,   5,   5,
    0,   0,   0,  20,  20,   0,   0,   0,
    5,  -5, -10,   0,   0, -10,  -5,   5,
    5,  10,  10, -20, -20,  10,  10,   5,
    0,   0,   0,   0,   0,   0,   0,   0
};

int* getSymmetricallyInvertedArray(const int* originalArray) {
    int* invertedArray = new int[64]; // Create an array of size 64

    for (int i = 0; i < 64; ++i) {
        invertedArray[i] = originalArray[63 - i];
    }

    return invertedArray;
}

int main() {
    int* invertedPawnPST = getSymmetricallyInvertedArray(PawnPST);

    // Print the inverted array
    for (int i = 0; i < 64; ++i) {
        std::cout << invertedPawnPST[i] << " ";
        if ((i + 1) % 8 == 0)
            std::cout << std::endl;
    }

    // Don't forget to delete the dynamically allocated array to avoid memory leaks
    delete[] invertedPawnPST;

    return 0;
}
