#pragma once

#include <optional>
#include <string>

// can be caching storage
// or simple storage or whatever else
namespace core {

class IStorage
{
public:
    virtual ~IStorage() = default;
    virtual void Set(const std::string &key, const std::string &value) = 0;
    virtual std::optional<std::string> Get(const std::string &key) = 0;
    virtual bool Delete(const std::string &key) = 0;
};

} // namespace core
