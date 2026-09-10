#pragma once
#include <iostream>
#include <string>

// Variable input function for any type
void input(auto &var) {
    std::cin >> var;
}

// String input function that reads a line of text
void inputln(std::string &var) {
    std::getline(std::cin >> std::ws, var);
}