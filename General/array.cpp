#include<iostream>
#include<array>


int main() {
    std::array<int, 3> arr;
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;

    for (const int num : arr) {
        std::cout << num << std::endl;
    }

    std::cout << "Size of array: " << arr.size() << std::endl;

    std::cout << "Element at index 2: " << arr.at(2) << std::endl;
    // or
    std::cout << "Element at index 2: " << arr[2] << std::endl;

    // Access out of bound element
    // Performs bounds checking
    // std::cout << "Element at index 3: " << arr.at(3) << std::endl;
    // or
    // Does not perform bounds checking
    std::cout << "Element at index 3: " << arr[3] << std::endl;

    // get a pointer to the first element
    int* aptr = &arr[0];

    std::cout << "First element: " << *aptr << std::endl;
    std::cout << "Second element: " << *(aptr + 1) << std::endl;
    std::cout << "Third element: " << *(aptr + 2) << std::endl;
    std::cout << "Fourth element: " << *(aptr + 3) << std::endl;

}