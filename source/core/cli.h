#pragma once

#include <queue>
#include <map>
#include <set>

namespace core
{

using CmdHandler = std::function<std::error_code(const std::string&)>;
using Commands = std::tuple<std::string, std::string>;

class CLI
{
public:
    explicit CLI();
    ~CLI();

    void addArg(const std::string& arg);
    void addHandler(const std::string& cmd, CmdHandler&& handler);

private:
    void parseInput();
    void parseArg(const std::string& arg);
    void proccessCmds(const std::vector<std::string>& cmds);

private:
    bool is_running_;

    std::queue<std::string> queue_;
    std::condition_variable cv_;
    std::mutex mutex_;

    std::map<std::string, CmdHandler> handlers_;
    std::set<std::string> keywords_;
};

}
