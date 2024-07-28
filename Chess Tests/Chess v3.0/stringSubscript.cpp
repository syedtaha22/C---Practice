#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, World!";
    
    // Attempting to access an index beyond the length of the string
    char ch = str[20]; // Here, index 20 is out of range for the string
    
    std::cout << "Character at index 20: " << ch << std::endl;
    
    return 0;
}
