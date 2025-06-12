#pragma once

#include <optional>
#include <string>

#include "keyvalueclient.h"

namespace core
{

struct GET
{
  std::string key;

  static constexpr std::string_view name() { return "GET"; }

  static std::optional<GET> parse(const std::string& arg)
  {
    if (arg.empty()) return std::nullopt;
    return GET{arg};
  }

  std::error_code operator()(network::KeyValueClient& client) const
  {
    std::cout << client.Get(key).second << "\n";
    return {};
  }
};

struct SET
{
  std::string key;
  std::string value;

  static constexpr std::string_view name() { return "SET"; }

  static std::optional<SET> parse(const std::string& arg)
  {
    if (arg.empty()) return std::nullopt;

    std::string k,v;
    std::istringstream iss {arg};
    iss >> k >> v;

    return SET{k, v};
  }

  std::error_code operator()(network::KeyValueClient& client) const
  {
    client.Set(key, value);
    return {};
  }
};

struct DELETE
{
  std::string key;

  static constexpr std::string_view name() { return "DELETE"; }

  static std::optional<DELETE> parse(const std::string& arg)
  {
    if (arg.empty()) return std::nullopt;
    return DELETE{arg};
  }

  std::error_code operator()(network::KeyValueClient& client) const
  {
    client.Delete(key);
    return {};
  }
};

} // namespace core
