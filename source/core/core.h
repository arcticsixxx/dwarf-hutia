#pragma once

#include <mutex>
#include <optional>
#include <unordered_map>

namespace core
{
class KeyValueStore {
public:
    void Set(const std::string& key, const std::string& value);
    std::optional<std::string> Get(const std::string& key);
    bool Delete(const std::string& key);

private:
    std::unordered_map<std::string, std::string> store_;
    std::mutex mutex_;
};
}
