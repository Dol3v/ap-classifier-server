
#include <iostream>
#include "standardio.hpp"

std::string io::StandardIo::read() {
    std::string input;
    std::getline(std::cin, input);
    input = input.append("\n");
    return input;
}

void io::StandardIo::write(const std::string &to_write) {
    std::cout << to_write;
}
