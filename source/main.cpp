#include <iostream>
#include <string>

#include "core/cli.h"

auto main() -> int {
    core::CLI cli;

    core::CmdHandler alwaysSuccess = [](const std::string& str) -> std::error_code {
        std::cout << "Command: " << str << " processed successfully!" << std::endl;
        return std::error_code{};
    };

    cli.addHandler("SET", std::move(alwaysSuccess));

    core::CmdHandler checkNonEmptyString = [](const std::string& str) -> std::error_code {
        if (str.empty()) {
            std::cerr << "Error: Command string is empty!" << std::endl;
            return std::make_error_code(std::errc::invalid_argument);
        }
        std::cout << "Command: " << str << " processed successfully!" << std::endl;
        return std::error_code{};
    };

    cli.addHandler("GET", std::move(checkNonEmptyString));

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "/EXIT") break;
        cli.addArg(str);
    }

    return 0;
}
