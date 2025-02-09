#include <iostream> // Include the iostream library
#include <vector> // Include the vector library
#include <cmath> // Include the cmath library

// Function to calculate factorial of a number using Dynamic Programming
int factorial(int n) {
    std::vector<int> fact(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; i++) {
        fact[i] = i * fact[i - 1];
    }
    return fact[n];
}

// Function to calculate Absolute Error
long double absolute_error(long double curr, long double prev) {
    return std::abs((curr - prev) / curr) * 100;
}

// Function to calculate e^x upto n significant digits
long double calculate_e(double x, int n) {
    long double curr = 0; // Variable to hold current Approximation
    long double prev = 0; // Variable to hold previous Approximation
    long double tol = 0.5 * pow(10, 2 - n); // Tolerance for n significant digits
    // Loop to calculate e^x
    for (int i = 0; ; i++) {
        prev = curr; // Set previous value to current value
        curr += std::pow(x, i) / static_cast<long double> (factorial(i)); // Update current value

        long double abs_error = absolute_error(curr, prev); // Calculate Absolute Error
        std::cout << "Absolute Error: " << abs_error << "%" << std::endl; // Print abs_error for each iteration

        if (abs_error < tol) break; // Check if Stopping Criteria is met
    }
    return curr; // Return the final value of e^x
}

int main() {
    std::cout << std::fixed; // Set fixed precision
    std::cout.precision(10); // Set precision to 10 decimal places

    // Calculate e^0.5 upto 1 to 9 significant digits
    long double e = calculate_e(0.5, 3);
    std::cout << "e^0.5 upto 3 significant digits: " << e << std::endl;

    return 0; // return 0 to indicate successful completion
}