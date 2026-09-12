#include <print>
#include "easyin.hpp"

using namespace std;
using namespace easyin;

int main() {

    // Example using integer input
    int a, b;
    print("Enter first integer: ");
    input(a);
    print("Enter second integer: ");
    input(b);
    println("Sum of integers: {}", a + b);

    // Example using string input for multiple words
    string fullname;
    print("Enter your first and last name: ");
    inputln(fullname);
    println("Hello, {}!", fullname);

    // Example of a failed input read
    int x;
    print("Enter an integer: ");
    bool success = input(x);
    if (!success) {
        println("Failed to read an integer.");
    } else {
        println("You entered: {}", x);
    }

    // The stream is already clean here, even after the failure above -
    // no manual clear()/ignore() needed before this read
    string another;
    print("Enter one more line: ");
    inputln(another);
    println("You entered: {}", another);

    // inputAvailable() can be used to check if input is available in the stream
    int age = 0;

    while (!input(age)) {
        if (!inputAvailable()) {
            println("No more input available.");
            return 1; // Exit if no more input is available
        }
        println("Please enter a valid number.");
    }

    return 0;
}