#include <thread>

#include "cli.h"

namespace core
{

constexpr const auto slashKey = '/';

CLI::CLI()
{
  is_running_ = true;

  std::thread cli {&CLI::parseInput, this};
  cli.detach();
}

CLI::~CLI()
{
  is_running_ = false;

  cv_.notify_one();
}

void CLI::addArg(const std::string& arg)
{
  {
    std::unique_lock<std::mutex> lock {mutex_};
    queue_.push(arg);
  }

  cv_.notify_one();
}

void CLI::addHandler(const std::string& cmd, CmdHandler&& handler)
{
  auto newCmd = std::string {slashKey + cmd};

  handlers_[newCmd] = std::move(handler);
  keywords_.insert(newCmd);
}

void CLI::parseInput()
{
  while (is_running_) {
    std::unique_lock<std::mutex> lock {mutex_};
    cv_.wait(lock, [this] { return !queue_.empty() || !is_running_; });

    while (!queue_.empty()) {
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
  // [("/GET something"), ("/SET key value")]

  std::vector<std::string> cmds;
  std::istringstream iss {arg};

  std::string word;
  std::string current;

  while (iss >> word) {
    if (!word.empty() && word[0] == slashKey) {
      if (!current.empty()) {
        cmds.push_back(current);
        current.clear();
      }
      current += word;
    } else {
      current += " " + word;
    }
  }
  if (!current.empty()) {
    cmds.push_back(current);
  }

  proccessCmds(cmds);
}

void CLI::proccessCmds(const std::vector<std::string>& cmds)
{
  if (cmds.empty()) {
    return;
  }

  for (auto&& cmd : cmds) {
    bool commandMatched = false;

    for (auto&& kw : keywords_) {
      if (cmd.starts_with(kw)) {
        commandMatched = true;
        break;
      }
    }

    if (!commandMatched) {
      continue;
    }

    size_t pos = cmd.find(' ');
    std::string c, args;

    if (pos != std::string::npos) {
      c = cmd.substr(0, pos);
      args = cmd.substr(pos + 1);
    } else {
      c = cmd;
    }

    auto it = handlers_.find(c);
    if (it == handlers_.end()) {
      continue;
    }

    it->second(args);
  }
}
}  // namespace core
