#include <iostream>
#include <string>

std::string format(int number) {
    std::string str = std::to_string(number);
    int len = str.length();
    if (len <= 3) // No need for formatting if the number is less than 1000
        return str;

    int commas = (len - 1) / 3; // Calculate the number of commas to be inserted
    std::string formatted;
    formatted.reserve(len + commas);

    int i = 0;
    int j = len % 3 == 0 ? 3 : len % 3; // Adjust for the first segment
    formatted.append(str, 0, j);

    for (; i + j < len; i += j, j = 3) {
        formatted.push_back(',');
        formatted.append(str, i + j, 3);
    }

    return formatted;
}

int main() {
    std::cout << format(2300) << std::endl;    // Output: 2,300
    std::cout << format(100000) << std::endl;  // Output: 100,000
    return 0;
}
