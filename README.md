# EasyIn

EasyIn is a tiny, beginner-friendly input library for modern C++.

It provides two simple input functions for reading values and lines, plus a helper for checking whether an input stream is still in a good state.

Use it with C++20 output such as `std::cout`, or pair it with C++23's `std::print()` and `std::println()`.

## Features

* `easyin::input()` reads a value using stream extraction (`>>`); for strings, it reads one whitespace-delimited word.
* `easyin::inputln()` reads a full line after skipping leading whitespace.
* `easyin::inputAvailable()` checks whether the input stream is still in a good state.
* `input()` and `inputln()` return `true` when a read succeeds and `false` when it fails.
* Failed reads automatically attempt to clean the stream by clearing its error state and discarding the remainder of the current line.
* All functions use `std::cin` by default and can also work with another `std::istream`, such as a file or string stream.
* All functions are `inline`, so the header can safely be included from multiple `.cpp` files.
* Header-only, with no external dependencies.

## Quick start

Place `easyin.hpp` next to your `main.cpp`.

This complete example uses C++20:

```cpp
#include <iostream>
#include <string>
#include "easyin.hpp"

using easyin::input;
using easyin::inputln;
using easyin::inputAvailable;

int main() {
    int age{};
    std::string name;

    std::cout << "Enter your age: ";

    while (!input(age)) {
        if (!inputAvailable()) {
            std::cerr << "Input ended or the stream is unavailable.\n";
            return 1;
        }

        std::cerr << "Please enter a valid age: ";
    }

    std::cout << "Enter your name: ";

    if (!inputln(name)) {
        std::cerr << "Could not read a name.\n";
        return 1;
    }

    std::cout
        << "Hello, "
        << name
        << "! You are "
        << age
        << " years old.\n";
}
```

Build with GCC or Clang:

```sh
g++ -std=c++20 main.cpp -o myprogram
```

For Clang, replace `g++` with `clang++`.

For MSVC, use a Developer Command Prompt:

```bat
cl /EHsc /std:c++20 main.cpp
```

The `using` declarations allow the short names:

```cpp
input(age);
inputln(name);
inputAvailable();
```

Alternatively, omit the declarations and use the namespace directly:

```cpp
easyin::input(age);
easyin::inputln(name);
easyin::inputAvailable();
```

Keeping the namespace qualification avoids introducing those function names into your own scope.

## Functions

### `easyin::input()`

```cpp
inline bool input(auto &var, std::istream &in = std::cin);
```

Reads a value into `var` using:

```cpp
in >> var;
```

It accepts types that support stream extraction, such as:

* `int`
* `double`
* `char`
* `std::string`
* other types with a compatible `operator>>`

Example:

```cpp
int age;

if (easyin::input(age)) {
    std::cout << "Age: " << age << '\n';
}
```

When the stream argument is omitted, `std::cin` is used automatically.

For strings:

```cpp
std::string name;
easyin::input(name);
```

If the user enters:

```text
John Smith
```

`name` contains:

```text
John
```

This happens because `input()` uses normal C++ stream extraction.

Whitespace such as spaces, tabs and newlines separates values.

### Failed reads

`input()` returns:

```text
true
```

when extraction succeeds, and:

```text
false
```

when extraction fails.

For example:

```cpp
int age;

if (!easyin::input(age)) {
    std::cout << "That was not a valid integer.\n";
}
```

After a failed read, EasyIn attempts to clean the stream automatically, so you normally do not need to manually call:

```cpp
std::cin.clear();
std::cin.ignore();
```

### Partial numeric input

`input()` uses normal stream extraction rather than validating an entire line.

For example, reading an integer from:

```text
12abc
```

can successfully read:

```text
12
```

while leaving:

```text
abc
```

in the stream for a later read.

EasyIn does not perform application-specific validation.

For example, checking whether an age is positive still belongs in your program:

```cpp
int age;

if (easyin::input(age) && age >= 0) {
    // Valid age for this application
}
```

---

### `easyin::inputln()`

```cpp
inline bool inputln(std::string &var, std::istream &in = std::cin);
```

Reads a line into a `std::string` using:

```cpp
std::getline(in >> std::ws, var);
```

Example:

```cpp
std::string fullname;

easyin::inputln(fullname);
```

If the user enters:

```text
John Smith
```

the complete text is stored:

```text
John Smith
```

Spaces inside the line are preserved.

Like `input()`, `inputln()` returns `true` when reading succeeds and `false` when it fails.

### Why `std::ws` is used

A common C++ problem appears when formatted input and `getline()` are mixed:

```cpp
int age;
std::string name;

std::cin >> age;
std::getline(std::cin, name);
```

After reading `age`, the newline from pressing Enter can remain in the stream.

The following `getline()` may immediately read that newline instead of waiting for the name.

EasyIn's `inputln()` uses:

```cpp
std::getline(in >> std::ws, var);
```

`std::ws` consumes leading whitespace before `getline()` begins reading.

This makes code such as:

```cpp
input(age);
inputln(name);
```

convenient.

### Important whitespace behaviour

`std::ws` does not only remove a leftover newline.

It removes leading whitespace in general, including:

* spaces at the beginning of a line
* tabs at the beginning of a line
* blank lines

For example, input such as:

```text
    indented text
```

does not preserve the indentation.

An intentionally empty line also cannot be captured with `inputln()`.

If exact formatting matters, use standard `std::getline()` directly:

```cpp
std::getline(file, line);
```

Internal and trailing spaces in a non-empty line are still preserved.

### Text left on the same line

`inputln()` does not automatically move to the next physical line.

For example, if `input()` successfully reads part of a line and leaves other text behind, `inputln()` can read that remaining text after skipping its leading whitespace.

---

### `easyin::inputAvailable()`

```cpp
inline bool inputAvailable(std::istream &in = std::cin);
```

`inputAvailable()` checks whether the input stream is currently in a good state.

Internally, it uses:

```cpp
in.good();
```

Without an argument, it checks `std::cin`:

```cpp
if (easyin::inputAvailable()) {
    // The standard input stream is still in a good state
}
```

You can also check another stream:

```cpp
if (easyin::inputAvailable(file)) {
    // The file stream is still in a good state
}
```

The most useful place for `inputAvailable()` is after a failed read.

For example:

```cpp
int age{};

while (!easyin::input(age)) {
    if (!easyin::inputAvailable()) {
        std::cout << "No more input available.\n";
        return 1;
    }

    std::cout << "Please enter a valid number.\n";
}
```

If the user enters something invalid such as:

```text
hello
```

`input(age)` returns `false`.

EasyIn cleans the invalid input, and if the stream is still usable, `inputAvailable()` returns `true`, allowing another attempt.

If the input has ended or the stream is no longer in a good state, `inputAvailable()` returns `false`.

### What `inputAvailable()` does not mean

`inputAvailable()` does **not** check whether the user has already typed something or whether characters are currently waiting in the keyboard buffer.

It checks the **state of the stream**.

Think of it as:

```text
Can normal input continue from this stream?
```

rather than:

```text
Has the user typed something yet?
```

Because of this, `inputAvailable()` is usually most useful **after a failed `input()` or `inputln()` call**, rather than before every read.

## Handling failed reads

Both `input()` and `inputln()` attempt automatic cleanup after a failed read.

Internally, they use:

```cpp
in.clear();
in.ignore(
    std::numeric_limits<std::streamsize>::max(),
    '\n'
);
```

### `clear()`

When formatted input fails, the stream enters an error state.

For example:

```cpp
int age;
std::cin >> age;
```

fails if the user enters:

```text
hello
```

The stream must be cleared before another normal read can be attempted.

EasyIn does this automatically with:

```cpp
in.clear();
```

### `ignore()`

After clearing the error state, EasyIn discards the remainder of the current line:

```cpp
in.ignore(
    std::numeric_limits<std::streamsize>::max(),
    '\n'
);
```

Using:

```cpp
std::numeric_limits<std::streamsize>::max()
```

means EasyIn does not depend on an arbitrary cleanup limit such as:

```cpp
ignore(1000, '\n');
```

It discards characters until the next newline or until the stream can no longer continue.

### Retrying safely

A simple retry loop such as:

```cpp
while (!input(age)) {
    std::cout << "Please enter a valid number.\n";
}
```

works for ordinary invalid input, but it has a problem.

If the input stream has ended, there may never be another successful read.

The loop could continue trying forever.

`inputAvailable()` allows the program to stop when retrying no longer makes sense:

```cpp
int age{};

while (!easyin::input(age)) {
    if (!easyin::inputAvailable()) {
        std::cout << "Input ended or the stream is unavailable.\n";
        return 1;
    }

    std::cout << "Please enter a valid number.\n";
}
```

This keeps the common retry logic short while still allowing the program to stop when input can no longer continue.

Streams configured to throw exceptions may still throw during reading or cleanup. EasyIn does not catch stream exceptions.

## Reading from other sources

All EasyIn functions can work with another `std::istream`.

That means the same interface can be used with:

* `std::cin`
* `std::ifstream`
* `std::istringstream`
* other compatible input streams

### String streams

A string stream is useful for testing input without typing into a terminal:

```cpp
#include <sstream>
#include "easyin.hpp"

int main() {
    std::istringstream source("25\n");

    int age{};

    if (!easyin::input(age, source)) {
        return 1;
    }

    return age == 25 ? 0 : 1;
}
```

Here:

```cpp
easyin::input(age, source);
```

reads from `source` instead of `std::cin`.

You can also check that stream:

```cpp
easyin::inputAvailable(source);
```

---

## Reading values from a file

Suppose `numbers.txt` contains:

```text
10
20
30
40
```

You can read the integers with:

```cpp
#include <fstream>
#include <iostream>
#include "easyin.hpp"

int main() {
    std::ifstream file("numbers.txt");

    if (!file.is_open()) {
        std::cerr << "Could not open numbers.txt.\n";
        return 1;
    }

    int number{};

    while (easyin::input(number, file)) {
        std::cout << number << '\n';
    }
}
```

The loop continues while each extraction succeeds.

It stops when a read fails, which can happen because of:

* end-of-file
* malformed input
* another stream error

`input()` reads values separated by whitespace, not necessarily one value per physical line.

If malformed input is encountered, EasyIn's cleanup discards the rest of that line.

---

## Reading text lines from a file

Suppose `sample.txt` contains:

```text
Hello, world!
This is a simple text file.
You can read it using C++ fstream.
Each line contains plain text.
Happy coding!
```

To read it line by line:

```cpp
#include <fstream>
#include <print>
#include <string>
#include "easyin.hpp"

using namespace std;
using namespace easyin;

int main() {
    string line;
    ifstream file("sample.txt");

    if (!file.is_open()) {
        println("Could not open sample.txt.");
        return 1;
    }

    while (inputln(line, file)) {
        println("{}", line);
    }

    return 0;
}
```

The output is:

```text
Hello, world!
This is a simple text file.
You can read it using C++ fstream.
Each line contains plain text.
Happy coding!
```

The loop ends when `inputln()` can no longer successfully read another line.

### `input()` vs. `inputln()` with files

| Call with a `std::string` | What it reads                              |
| ------------------------- | ------------------------------------------ |
| `input(text, file)`       | One whitespace-delimited string            |
| `inputln(text, file)`     | One line after skipping leading whitespace |

For example, with:

```text
Hello, world!
```

this:

```cpp
input(text, file);
```

first reads:

```text
Hello,
```

and another call reads:

```text
world!
```

But:

```cpp
inputln(text, file);
```

reads:

```text
Hello, world!
```

If exact indentation or blank lines matter, use standard `std::getline()` instead because `inputln()` uses `std::ws`.

## Example program

The supplied `example.cpp` demonstrates:

* integer input
* line input
* detecting a failed read
* automatic cleanup after invalid input
* retrying invalid input
* `inputAvailable()`

The example uses:

```cpp
#include <print>
```

so it requires C++23 output support.

A retry example from the program is:

```cpp
int age = 0;

while (!input(age)) {
    if (!inputAvailable()) {
        println("No more input available.");
        return 1;
    }

    println("Please enter a valid number.");
}
```

This demonstrates the intended relationship between:

```cpp
input()
```

and:

```cpp
inputAvailable()
```

`input()` attempts the read.

If it fails, `inputAvailable()` can tell the program whether the stream is still in a good state for another attempt.

## Installation and requirements

Copy `easyin.hpp` into your project and include it with:

```cpp
#include "easyin.hpp"
```

There is no separate EasyIn library to compile or link.

All functions are defined as `inline`, so the header can safely be included from multiple `.cpp` files in the same program without causing multiple-definition linker errors.

You can also place it in your own include directory and use:

```cpp
#include <easyin.hpp>
```

Add the directory to the compiler's include search path.

For GCC or Clang:

```sh
-Ipath/to/include
```

For MSVC:

```bat
/I"path\to\include"
```

Use your own include directory rather than modifying the compiler's standard library directories.

## C++ version

EasyIn requires C++20 or newer.

This is because `input()` uses an abbreviated function template:

```cpp
inline bool input(auto &var, std::istream &in = std::cin);
```

The library includes:

```cpp
#include <iostream>
#include <string>
#include <limits>
```

and has no third-party dependencies.

The EasyIn library itself does **not** require C++23.

The supplied `example.cpp` uses:

```cpp
std::print();
std::println();
```

from:

```cpp
#include <print>
```

so that example requires a compiler and standard library with C++23 `<print>` support.

For GCC or Clang:

```sh
g++ -std=c++23 example.cpp -o example
```

For Clang:

```sh
clang++ -std=c++23 example.cpp -o example
```

For MSVC, use a language mode and standard library version that support `<print>`.

Selecting C++23 mode alone does not add `<print>` to an older standard library implementation.

If your compiler does not support `<print>`, use `std::cout` with EasyIn instead.

## Current API

EasyIn currently provides three functions:

```cpp
easyin::input();
easyin::inputln();
easyin::inputAvailable();
```

### Value input

```cpp
input(value);
```

Reads one value using normal stream extraction.

### Line input

```cpp
inputln(text);
```

Reads a line of text after skipping leading whitespace.

### Stream-state check

```cpp
inputAvailable();
```

Checks whether the input stream is still in a good state.

Each function can also operate on another input stream:

```cpp
input(value, file);
inputln(text, file);
inputAvailable(file);
```

## Changes in this update

* Added `easyin::inputAvailable()`.
* `inputAvailable()` uses `std::istream::good()` to check whether the stream is still in a good state.
* Added a safe retry pattern that stops when input can no longer continue.
* `input()` and `inputln()` continue to return a success/failure `bool`.
* `input()` and `inputln()` automatically attempt stream cleanup after failed reads.
* All functions can work with `std::cin` or another `std::istream`.
* All functions are `inline`.
* Failed-read cleanup uses `std::numeric_limits<std::streamsize>::max()` instead of an arbitrary character limit.
* Documentation covers word input, line input, whitespace behaviour, EOF, retrying, partial numeric input, files and other streams.

Existing calls such as:

```cpp
easyin::input(age);
```

and:

```cpp
easyin::inputln(name);
```

continue to work unchanged.

The new helper is optional:

```cpp
easyin::inputAvailable();
```

It is mainly useful when a program wants to retry a failed read without retrying indefinitely after the stream can no longer continue.

See [easyin.hpp](easyin.hpp) for the implementation.

## License

EasyIn is licensed under the MIT License.

See [LICENSE](LICENSE) for details.