#pragma once

#include <system_error>

namespace core {

struct ICommand
{
  virtual ~ICommand() = default;
  virtual std::error_code execute(const std::string& args) const = 0;
};

} // namespace core
