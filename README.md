# EasyIn

EasyIn is a tiny, beginner-friendly input library for modern C++.

It provides simple input functions designed to make console input easier to
read and to pair naturally with C++23's `std::print()` and `std::println()`.

## Features

- `input()` - reads a single value
- `inputln()` - reads an entire line
- Header-only
- No external dependencies
- Simple and beginner-friendly syntax
- Works with standard C++ types supported by `std::cin`

## Example

```cpp
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

```cpp
int age;
input(age);
```

It can be used with different types:

```cpp
int number;
double price;
std::string word;

input(number);
input(price);
input(word);
```

For strings, `input()` stops reading when it encounters whitespace.

### `inputln()`

Reads an entire line into a `std::string`.

```cpp
std::string name;
inputln(name);
```

Unlike `input()`, this allows spaces:

```text example
Name Surname
```

The function also handles leftover whitespace from previous `input()` calls.

## Installation

EasyIn is a header-only library, so no compilation or linking is required.

### Option 1 - Add the header to your project

Download `easyin.hpp` and place it inside your project:

```text
MyProject/
├── main.cpp
└── easyin.hpp
```

Then include it with:

```cpp
#include "easyin.hpp"
```

### Option 2 - Add EasyIn to your compiler include path

Place `easyin.hpp` inside a directory that your compiler searches for headers,
or add your own EasyIn directory to the compiler's include path.

You can then use:

```cpp
#include <easyin.hpp>
```

For MSVC, a custom include directory can be added using:

```text
/I"path\to\include"
```

For GCC and Clang:

```text
-Ipath/to/include
```

It is recommended to use your own library/include directory rather than
modifying the compiler's standard library files.

## Requirements

EasyIn requires:

- A C++20-compatible compiler or newer
- Standard C++ `<iostream>`
- Standard C++ `<string>`

C++20 is required because `input()` uses an abbreviated function template:

```cpp
void input(auto &var)
```

The EasyIn library itself does **not** require C++23.

However, examples using:

```cpp
#include <print>

std::print(...);
std::println(...);
```

require C++23 support.

### MSVC

Use C++23 mode for the examples with `<print>`:

```text
/std:c++23preview
```

### GCC / Clang

Use:

```text
-std=c++23
```

## Current implementation

```cpp
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