# EasyIn

EasyIn is a tiny, beginner-friendly input library for modern C++.

It provides two simple input functions that report whether a read succeeded
and attempt to recover after invalid input. Use it with C++20 output such as
`std::cout`, or pair it with C++23's `std::print()` and `std::println()`.

## Features

- `easyin::input()` reads a value using stream extraction (`>>`); for strings,
  it reads one whitespace-delimited word.
- `easyin::inputln()` reads a line after skipping leading whitespace.
- Both return `true` on success and `false` on a failed read.
- Both clear the stream's error flags and discard through the next newline
  after a failed read, unless the stream ends or encounters another error.
- Both accept an optional `std::istream`, including files and string streams.
- Header-only, with no external dependencies.

## Quick start

Place `easyin.hpp` next to your `main.cpp`. This complete example uses C++20:

```cpp
#include <iostream>
#include <string>
#include "easyin.hpp"

using easyin::input;
using easyin::inputln;

int main() {
    int age{};
    std::string name;

    std::cout << "Enter your age: ";
    if (!input(age)) {
        std::cerr << "Could not read an age.\n";
        return 1;
    }

    std::cout << "Enter your name: ";
    if (!inputln(name)) {
        std::cerr << "Could not read a name.\n";
        return 1;
    }

    std::cout << "Hello, " << name << "! You are " << age << " years old.\n";
}
```

Build with GCC or Clang:

```sh
g++ -std=c++20 main.cpp -o myprogram
```

For Clang, replace `g++` with `clang++`. For MSVC, use a Developer Command Prompt:

```bat
cl /EHsc /std:c++20 main.cpp
```

The `using` declarations allow the short names `input` and `inputln`.
Alternatively, omit those declarations and write `easyin::input(age)` and
`easyin::inputln(name)`. Keeping the namespace qualification avoids introducing
those function names into your own scope.

## Functions

### `easyin::input()`

```cpp
bool input(auto &var, std::istream &in = std::cin);
```

Reads into `var` using `in >> var`. It accepts types with a compatible stream
extraction operator, such as `int`, `double`, and `std::string`.

- Uses `std::cin` when you omit the second argument.
- Returns `true` when extraction succeeds.
- Returns `false` when extraction fails, after attempting cleanup.
- For a string, reads one whitespace-delimited word.

This is stream extraction, not validation of an entire entry. For example,
reading an `int` from `12abc` succeeds with the value `12` and leaves `abc`
for a later read. Checks such as whether an age is positive belong in your
application.

### `easyin::inputln()`

```cpp
bool inputln(std::string &var, std::istream &in = std::cin);
```

Uses `std::getline(in >> std::ws, var)` to read text into a string. It returns
`true` on success and `false` on failure, with the same cleanup attempt as
`input()`.

The `std::ws` step skips leading whitespace, including leftover newlines,
indentation, and blank lines. Internal and trailing spaces in the resulting
line are preserved; the ending newline is consumed but not stored.

This makes it convenient to read a name after a number, but it cannot capture
an intentionally empty line or preserve indentation. Use `std::getline()`
directly when you need that behaviour.

If `input()` leaves other text on the same line, `inputln()` reads that text
after skipping its leading whitespace; it does not automatically jump to the
next line.

## Handling failed reads

Check the return value before using the result. A failed read may change the
destination or leave it unchanged, so it should not be treated as valid input.
Initializing variables is helpful, but does not replace checking success.

After a failed read, both functions call:

```cpp
in.clear();
in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
```

This removes the fixed 1,000-character cleanup limit. For ordinary malformed
input, the unread remainder of the line is discarded, including its newline,
so the next attempt can read a new line.

Cleanup does not guarantee that another read will succeed. End-of-file (EOF)
means there is no more input, and cleanup can also encounter an I/O error.
Never retry indefinitely without checking whether the stream is still usable.

Here is a complete console retry example that stops when cleanup leaves the
stream at EOF or in an error state:

```cpp
#include <iostream>
#include "easyin.hpp"

int main() {
    int age{};

    for (;;) {
        std::cout << "Enter your age: ";
        if (easyin::input(age)) {
            std::cout << "Age: " << age << '\n';
            return 0;
        }

        if (!std::cin.good()) {
            std::cerr << "Input ended or the stream is unavailable.\n";
            return 1;
        }

        std::cerr << "Could not read an integer. Please try again.\n";
    }
}
```

Use the returned `bool` to detect failure: cleanup clears the original stream
error flags. EasyIn does not return a separate error code for each cause.
Streams configured to throw exceptions may throw during reading or cleanup;
EasyIn does not catch those exceptions.

## Reading from other sources

Pass a stream as the second argument to either function. You can use a string
stream to try inputs without typing into a terminal:

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

### Reading values from a file

To read integers from a file:

```cpp
#include <fstream>
#include <iostream>
#include "easyin.hpp"

int main() {
    std::ifstream file("answers.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open answers.txt.\n";
        return 1;
    }

    int score{};
    while (easyin::input(score, file)) {
        std::cout << score << '\n';
    }
}
```

The loop stops at the first failed extraction, including EOF, malformed input,
or an I/O error. It reads values separated by whitespace, not necessarily one
value per line. Cleanup after malformed input discards the rest of that line,
which can include other values.

### Reading text lines from a file

To read text one line at a time, pass a `std::ifstream` to `inputln()`. This
example uses C++23's `<print>` and requires a compiler and standard library
that support `std::println()`.

Create `sample.txt` in the program's working directory with this content:

```text
Hello, world!
This is a simple text file.
You can read it using C++ fstream.
Each line contains plain text.
Happy coding!
```

Place `easyin.hpp` next to your source file and use:

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

The program prints the same five lines, preserving spaces within each line.
The loop stops when `inputln()` reports a failed read, including EOF or an
I/O error.

Choose the function based on how much text you want to read:

| Call with a `std::string` | What it reads |
| --- | --- |
| `input(line, file)` | One word, stopping at whitespace such as a space, tab, or newline. |
| `inputln(line, file)` | One line after skipping leading whitespace, preserving spaces within the line. |

For example, `input()` reads `Hello, world!` as `Hello,` and then `world!`.
If you call `println()` after each read, each word appears on its own line.
`println()` adds those newlines; reading one word at a time is the intended
behaviour of `input()` for strings.

`inputln()` skips blank lines and indentation because it uses `std::ws`.
Use `std::getline(file, line)` directly when you need to preserve them.

## Installation and requirements

Copy `easyin.hpp` into your project and include it with:

```cpp
#include "easyin.hpp"
```

There is no separate EasyIn library to compile or link. In the current
implementation, include the header in only one `.cpp` file per executable.
`inputln()` is defined in the header without `inline`, so including it from
multiple source files can cause a multiple-definition linker error.

You can also place it in your own include directory and use
`#include <easyin.hpp>`. Add that directory to the compiler's search path with
`-Ipath/to/include` for GCC/Clang or `/I"path\to\include"` for MSVC. Use your own
include directory rather than modifying the compiler's standard library files.

EasyIn requires C++20 or newer because `input()` uses `auto` in a function
parameter. It includes the standard headers `<iostream>`, `<limits>`, and
`<string>`; no third-party dependencies are required.

The library itself does not require C++23. The text-file example above and the
supplied [example.cpp](example.cpp) use `<print>` and require a compiler and
standard library that implement C++23 `std::print()` and `std::println()`.

The supplied example checks the integer read in its failure demonstration;
its other reads assume valid input. The README examples above show how to
check each read before using its result.

For a supporting GCC/Clang toolchain, compile that example in C++23 mode:

```sh
g++ -std=c++23 example.cpp -o example
```

For MSVC, enable a supported C++23 language mode. Selecting a language mode
alone does not add `<print>` support to an older standard library. If your
toolchain lacks it, use the C++20 quick start above.

## Changes in this update

- Both input functions now return a success/failure `bool`.
- Both accept an optional stream argument while keeping `std::cin` as the default.
- Failed reads attempt automatic stream cleanup without a fixed character limit.
- The header includes `<limits>` for the stream cleanup limit.
- README examples check failed reads, and the documentation explains EOF,
  whitespace, and partial numeric input.

Existing direct calls such as `easyin::input(age)` still compile, but callers
should check the new return value. Failed reads now trigger cleanup. Code that
depends on the old exact function signatures, such as function pointers, needs
updating.

See [easyin.hpp](easyin.hpp) for the implementation.

## License

EasyIn is licensed under the MIT License. See [LICENSE](LICENSE) for details.