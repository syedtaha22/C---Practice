#include<iostream>
#include<bitset>

int main() {
    std::bitset<3> b1(5);

    b1.reset(2); // 101 -> 001

    b1.flip(1); // 001 -> 011

    b1.set(2); // 011 -> 111

    b1.reset(); // 111 -> 000
    b1.set(1); // 000 -> 010

    std::cout << b1.count() << std::endl; // Gives the number of 1s in the bitset
    std::cout << b1.all() << std::endl; // Are all the bits set to 1?
    std::cout << b1.any() << std::endl; // Are any of the bits set to 1?
    std::cout << b1.none() << std::endl; // Are none of the bits set to 1?
    std::cout << b1.test(1) << std::endl; // Is the bit at position 1 set to 1
    b1.flip(); // 010 -> 101
    std::cout << b1.size() << std::endl; // Returns the size of the bitset

    std::cout << b1.to_string() << std::endl; // Returns the string representation of the bitset
    b1._Getword(0); // Returns the word at position 0


    std::cout << b1 << std::endl;



    return 0;


}