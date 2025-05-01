#include <iostream>
#include <mutex>
#include <ostream>
#include <shared_mutex>

#include "keyvaluestore.h"

namespace core
{
KeyValueStore::KeyValueStore() {}

KeyValueStore::~KeyValueStore() {}

void KeyValueStore::Set(const std::string& key, const std::string& value)
{
  std::shared_lock lock {mutex_};
  store_[key] = value;
}

std::optional<std::string> KeyValueStore::Get(const std::string& key)
{
  std::lock_guard lock {mutex_};

  if (!store_.contains(key)) {
    return {};
  }

  return {store_[key]};
}

bool KeyValueStore::Delete(const std::string& key)
{
  std::lock_guard lock {mutex_};

  if (!store_.contains(key)) {
    return false;
  }

  store_.erase(key);
  return true;
}

std::unordered_map<std::string, std::string> KeyValueStore::snapshot()
{
  std::lock_guard lock {mutex_};

  return store_;
}
}  // namespace core
