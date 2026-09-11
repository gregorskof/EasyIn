#include <print>
#include "easyin.hpp"

using namespace std;
// using namespace easyin;
using easyin::input;
using easyin::inputln;

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

    return 0;
}