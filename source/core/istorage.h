#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace core {

class IStorage
{
public:
  virtual ~IStorage() {};
  virtual void Set(const std::string& key, const std::string& value) = 0;
  virtual std::optional<std::string> Get(const std::string& key) = 0;
  virtual bool Delete(const std::string& key) = 0;
  virtual std::unordered_map<std::string, std::string> snapshot() = 0;
};

} // namespace core
