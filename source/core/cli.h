#pragma once

#include <queue>
#include <map>

#include "commandwrapper.h"


namespace core
{

namespace network
{
class KeyValueClient;
}

using Commands = std::tuple<std::string, std::string>;

class CLI
{
public:
    explicit CLI();
    ~CLI();

    template<typename CmdType>
    void registerCommand(network::KeyValueClient& client)
    {
      handlers_[std::string{CmdType::name()}] = std::make_unique<CommandWrapper<CmdType>>(client);
    }

    void addArg(const std::string& arg);

private:
    void parseInput();
    void parseArg(const std::string& arg);
    void proccessCmds(const std::vector<std::string>& cmds);

private:
    std::atomic<bool> is_running_;

    std::queue<std::string> queue_;
    std::condition_variable cv_;
    std::mutex mutex_;

    std::map<std::string, std::unique_ptr<ICommand>> handlers_;
};

}
