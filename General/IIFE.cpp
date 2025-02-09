/*
    Explaination:
    - IIFE stands for Immediately Invoked Function Expression.
    - In C++, IIFE can be implemented using Lambda Functions.
    - Lambda Functions are anonymous functions that can be used to define a function inline.
    - Lambda Functions can be invoked immediately by adding () at the end of the function definition.
    - The syntax for Lambda Functions is:
        [capture clause] (parameters) -> return_type { function_body }
    - The capture clause is used to capture variables from the enclosing scope.
    - The parameters are the input parameters to the function.
    - The return_type is the return type of the function.
    - The function_body is the code that defines the function.
    - In the given code, the hello_worls function is defined as a Lambda Function.
    - The Lambda Function is immediately invoked by adding () at the end of the function definition.
    - This allows the function to be executed immediately without the need for a separate function call.
    - This pattern is useful for defining and executing functions in a single step.
    - It can be used to encapsulate code and avoid polluting the global namespace with function definitions.
    - Overall, the IIFE pattern using Lambda Functions provides a concise and efficient way to define and execute functions in C++.
*/

// Include the iostream library
#include <iostream>

// Define and immediately invoke a Lambda Function
int main() {
    // Define a Lambda Function that prints "Hello, World!"
    []() {
        std::cout << "Hello, World!" << std::endl;
        }(); // Immediately invoke the Lambda Function

    return 0; // Return 0 to indicate successful completion
}