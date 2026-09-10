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

    return 0;
}