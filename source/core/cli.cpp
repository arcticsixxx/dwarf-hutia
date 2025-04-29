#include "cli.h"

#include <thread>
#include <set>

namespace core
{

using Commands = std::tuple<std::string, std::string>;

std::set<std::string> keywords = { "/SET", "/GET", "/DELETE" };
const auto space = ' ';

CLI::CLI()
{
    is_running_ = true;

    std::thread cli{&CLI::parseInput, this};
    cli.detach();
}

CLI::~CLI()
{
    {
        std::unique_lock<std::mutex> lock{mutex_};
        is_running_ = false;
    }

    cv_.notify_one();
}

void CLI::addArg(const std::string& arg)
{
    {
        std::unique_lock<std::mutex> lock{mutex_};
        queue_.push(arg);
    }

    cv_.notify_one();
}

void CLI::parseInput()
{
    while (is_running_)
    {
        std::unique_lock<std::mutex> lock{mutex_};
        cv_.wait(lock, [this] { return !queue_.empty() || !is_running_; });

        while (!queue_.empty())
        {
            auto arg = queue_.front();
            queue_.pop();

            lock.unlock();
            parseArg(arg);
            lock.lock();
        }
    }
}

void CLI::parseArg(const std::string& arg)
{
    // /GET test /SET key val
    //          |
    //          V
    // [("/GET", "something"), ("/SET", "key value")]

    std::vector<std::string> tokens;
    size_t start, end = 0;

    while ((start = arg.find_first_not_of(space, end)) != std::string::npos)
    {
        end = arg.find(space, start);
        tokens.push_back(arg.substr(start, end - start));
    }

    parseCommands(tokens);
}

void CLI::parseCommands(const std::vector<std::string> &tokens)
{
    std::string command;
    std::string commandArgs;
    std::vector<Commands> commands;

    for (auto&& tok : tokens)
    {
        if (keywords.contains(tok))
        {
            if (!command.empty())
            {
                commands.emplace_back(std::make_tuple(command, commandArgs));
            }

            command = tok;
            commandArgs.clear();
        }
        else
        {
            if (!commandArgs.empty())
            {
                commandArgs += ' ';
            }
            commandArgs += tok;
        }
    }

    if (!command.empty())
    {
        commands.emplace_back(std::make_tuple(command, commandArgs));
    }
}

}
