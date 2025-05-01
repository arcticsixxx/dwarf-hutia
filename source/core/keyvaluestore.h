#pragma once

#include "istorage.h"

#include <shared_mutex>
#include <unordered_map>

namespace core {

class KeyValueStore : public IStorage
{
public:  
  KeyValueStore();
  ~KeyValueStore();
  void Set(const std::string& key, Value value);
  std::optional<Value> Get(const std::string& key) const;
  bool Delete(const std::string& key);
  std::unordered_map<std::string, Value> snapshot();

private:
  std::unordered_map<std::string, Value> store_;
  mutable std::shared_mutex mutex_;
};
} // namespace core
