#include <iostream>
#include <string>

#include "core/cli.h"

auto main() -> int {
    core::CLI cli;

    std::string str;

    while (std::getline(std::cin, str)) {
        if (str == "/EXIT") break;
        cli.addArg(str);
    }

    return 0;
}
