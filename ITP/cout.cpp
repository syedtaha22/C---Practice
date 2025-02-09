#include<iostream>

int main() {
    int x = 5;

    int result = ++x + 0 + ++x;

    std::cout << "Result: " << result << std::endl;
    std::cout << "Final value of cout: " << x << std::endl;
}

/*
    int x = 8;
    int y = 12;


    result = --x + y-- + ++x + --y;
    result =

    x=7, y=12, x=8, y=10
*/