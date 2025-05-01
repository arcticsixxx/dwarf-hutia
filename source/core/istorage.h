#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace core {

using Value = std::variant<std::string, int, float, bool>;

class IStorage
{
public:
  virtual ~IStorage() = default;
  virtual void Set(const std::string& key, Value value) = 0;
  virtual std::optional<Value> Get(const std::string& key) const = 0;
  virtual bool Delete(const std::string& key) = 0;
  virtual std::unordered_map<std::string, Value> snapshot() = 0;
};

} // namespace core
