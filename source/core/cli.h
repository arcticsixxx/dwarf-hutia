#pragma once

#include <queue>

#include <shared_mutex>
#include <condition_variable>

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
    std::shared_mutex mutex_;

    std::condition_variable_any cv;

    bool is_running;
};

}
