#include<iostream>
using namespace std;

int main() {
    int i = 3;

    bool val = -3;
    std::cout << "val = " << val << std::endl;

    while (i) {
        // Print the the current iteration
        std::cout << "i = " << i << ":\n";



        int x = 2;
        for (int j = i; j < 5; j = j + x) {
            std::cout << "\t"; // tab
            std::cout << "i = " << i; // Print i
            std::cout << ", j = " << j; // Print j
            std::cout << ", j + x = " << j + x; // Print j + x, Loop will run until j + x > 5
            std::cout << "\n"; // newline
        }
        i--;
        // i--;
    }

}