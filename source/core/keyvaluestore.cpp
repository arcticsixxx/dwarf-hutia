#include <iostream>
#include <mutex>
#include <ostream>
#include <shared_mutex>

#include "keyvaluestore.h"

namespace core
{
KeyValueStore::KeyValueStore() {}

KeyValueStore::~KeyValueStore() {}

void KeyValueStore::Set(const std::string& key, Value value)
{
  std::shared_lock lock {mutex_};
  store_[key] = std::move(value);
}

std::optional<Value> KeyValueStore::Get(const std::string &key) const
{
    std::lock_guard lock {mutex_};

    auto it = store_.find(key);
    if (it == store_.end()) {
      return std::nullopt;
    }

    return {it->second};
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

std::unordered_map<std::string, Value> KeyValueStore::snapshot()
{
  std::lock_guard lock {mutex_};

  return store_;
}
}  // namespace core
