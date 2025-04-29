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

private:
    std::queue<std::string> queue_;
    std::mutex mutex_;

    std::condition_variable cv;

    bool is_running;
};

}
