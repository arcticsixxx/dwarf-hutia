#pragma once

#include <queue>

namespace core
{

class CLI
{
public:
    explicit CLI();
    ~CLI();

    void addArg(const std::string& arg);

private:
    void parseInput();
    void parseArg(const std::string& arg);
    void parseCommands(const std::vector<std::string>& tokens);

private:
    bool is_running_;

    std::queue<std::string> queue_;
    std::condition_variable cv_;
    std::mutex mutex_;
};

}
