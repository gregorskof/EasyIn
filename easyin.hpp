#pragma once
#include <iostream>
#include <string>
#include <limits>

namespace easyin {
    // Reads a single value - returns false if the read failed
    bool input(auto &var, std::istream &in = std::cin) {
        bool ok = static_cast<bool>(in >> var);
        
        if (!ok) {
            in.clear(); // Clear the error state
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
        }
        return ok;
    }

    // Reads an entire line - returns false if the read failed
    bool inputln(std::string &var, std::istream &in = std::cin) {
        bool ok = static_cast<bool>(std::getline(in >> std::ws, var));
        
        // Cleans up the stream the same way as input()
        if (!ok) {
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return ok;
    }
}