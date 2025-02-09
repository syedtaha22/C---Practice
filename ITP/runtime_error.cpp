#include<iostream>

int main() {
    int a = 10;
    int b = 0;
    int result = a / b;  // Division by zero, causes a runtime error.

    std::cout << "Result: " << result << std::endl;
}


