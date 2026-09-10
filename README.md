# EasyIn

EasyIn is a tiny, beginner-friendly input library for modern C++.

It provides simple input functions designed to make console input easier to
read and to pair naturally with C++23's `std::print()` and `std::println()`.

## Features

* `input()` - reads a single value
* `inputln()` - reads an entire line
* Header-only
* No external dependencies
* Simple and beginner-friendly syntax
* Works with standard C++ types supported by `std::cin`

## Example

```cpp id="a0cg5z"
#include <print>
#include <string>
#include <easyin.hpp>

int main() {
    int age;
    std::string name;

    std::print("Enter your age: ");
    input(age);

    std::print("Enter your name: ");
    inputln(name);

    std::println("Hello, {}! You are {} years old.", name, age);

    return 0;
}
```

## Functions

### `input()`

Reads a single value using `std::cin`.

```cpp id="ai808o"
int age;
input(age);
```

It can be used with different types:

```cpp id="qcemu9"
int number;
double price;
std::string word;

input(number);
input(price);
input(word);
```

For strings, `input()` stops reading when it encounters whitespace.

The function uses `auto`, which allows it to accept different types without
needing a separate function for each type.

```cpp id="hjhzne"
void input(auto &var)
```

### `inputln()`

Reads an entire line into a `std::string`.

```cpp id="5ix9li"
std::string name;
inputln(name);
```

Unlike `input()`, this allows spaces:

```text id="4j6x68"
Name Surname
```

The function also handles leftover whitespace from previous `input()` calls.

Because `inputln()` uses `std::ws`, whitespace at the beginning of the entered
line is ignored.

## Installation

EasyIn is a header-only library, so no separate library compilation or linking
is required.

### Option 1 - Add the header to your project

Download `easyin.hpp` and place it inside your project:

```text id="2av33o"
MyProject/
├── main.cpp
└── easyin.hpp
```

Then include it with:

```cpp id="vgjgsg"
#include "easyin.hpp"
```

### Option 2 - Add EasyIn to your compiler include path

Place `easyin.hpp` inside a directory that your compiler searches for headers,
or add your own EasyIn directory to the compiler's include path.

You can then use:

```cpp id="svmqa7"
#include <easyin.hpp>
```

For MSVC, a custom include directory can be added using:

```text id="dkoclr"
/I"path\to\include"
```

For GCC and Clang:

```text id="d68h1x"
-Ipath/to/include
```

It is recommended to use your own library/include directory rather than
modifying the compiler's standard library files.

## Requirements

EasyIn requires:

* A C++20-compatible compiler or newer
* Standard C++ `<iostream>`
* Standard C++ `<string>`

C++20 is required because `input()` uses `auto` in a function parameter:

```cpp id="w5f0ug"
void input(auto &var)
```

The EasyIn library itself does **not** require C++23.

However, examples using:

```cpp id="6r5xk4"
#include <print>

std::print(...);
std::println(...);
```

require C++23 support.

### MSVC

Use C++23 mode for the examples with `<print>`:

```text id="aifqaj"
/std:c++23preview
```

### GCC / Clang

Use:

```text id="qvyd7l"
-std=c++23
```

## Current implementation

```cpp id="bkdwgo"
#pragma once

#include <iostream>
#include <string>

void input(auto &var) {
    std::cin >> var;
}

void inputln(std::string &var) {
    std::getline(std::cin >> std::ws, var);
}
```

## License

EasyIn is licensed under the MIT License.

See the `LICENSE` file for details.