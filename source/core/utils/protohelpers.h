#include <string>

#pragma once

namespace core::utils
{

template<typename T>
std::string HandleProtobufOneof(const T& message)
{
  switch (message.value_case()) {
    case T::kS:
      return message.s();
    case T::kI:
      return std::to_string(message.i());
    case T::kF:
      return std::to_string(message.f());
    case T::kB:
      return message.b() ? "true" : "false";
    case T::VALUE_NOT_SET:
    default:
      return "";
  }
}

}  // namespace core::utils
