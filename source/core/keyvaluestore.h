#pragma once

#include "istorage.h"

#include <shared_mutex>
#include <unordered_map>

namespace core {

class KeyValueStore : public IStorage
{
public:
  // IStorage interface
  KeyValueStore();
  ~KeyValueStore();
  void Set(const std::string& key, const std::string& value) override;
  std::optional<std::string> Get(const std::string& key) override;
  bool Delete(const std::string& key) override;
  std::unordered_map<std::string, std::string> snapshot() override;

private:
  std::unordered_map<std::string, std::string> store_;
  std::shared_mutex mutex_;
};
} // namespace core
