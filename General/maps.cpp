#include <iostream>
#include <unordered_map>

char getKeyFromValue(const std::unordered_map<char, int>& my_unordered_map, int value) {
    for (const auto& pair : my_unordered_map) {
        if (pair.second == value) {
            return pair.first;
        }
    }

    // std::cout << value;
    return '0';
}

int main() {
    std::unordered_map<char, int> my_unordered_map = {
        {'a', 1},
        {'b', 2},
        {'c', 3},
        {'d', 4},
        {'e', 5},
        {'f', 6},
        {'g', 7},
        {'h', 8}
    };

    int targetValue = 9;
    std::cout << "Key corresponding to value " << targetValue << " is: " << getKeyFromValue(my_unordered_map, targetValue) << std::endl;

    return 0;
}
