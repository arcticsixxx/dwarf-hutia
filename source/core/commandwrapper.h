#pragma once

#include <iostream>

#include "icommand.h"

namespace core
{

namespace network
{
class KeyValueClient;
}

template <typename CmdType>
class CommandWrapper : public ICommand
{
public:
  explicit CommandWrapper(core::network::KeyValueClient& client)
          : client_(client) {}

  std::error_code execute(const std::string& arg) const override
  {
    auto parsed = CmdType::parse(arg);
    if (!parsed)
    {
      std::cerr << "Parse error\n";
      return std::make_error_code(std::errc::invalid_argument);
    }
    return (*parsed)(client_);
  }

private:
  core::network::KeyValueClient& client_;
};

} // namespace core
