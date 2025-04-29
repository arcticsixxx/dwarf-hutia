#include "keyvaluestore.h"

#include <shared_mutex>

namespace core
{
void KeyValueStore::Set(const std::string &key, const std::string &value)
{
    std::shared_lock<std::shared_mutex> lock{mutex_};
    store_[key] = value;
}

std::optional<std::string> KeyValueStore::Get(const std::string &key)
{
    std::shared_lock<std::shared_mutex> lock{mutex_};

    if (! store_.contains(key))
    {
        return {};
    }

    return { store_[key] };
}

bool KeyValueStore::Delete(const std::string &key)
{
    std::shared_lock<std::shared_mutex> lock{mutex_};

    if (! store_.contains(key) )
    {
        return false;
    }

    store_.erase(key);
    return true;
}
}
