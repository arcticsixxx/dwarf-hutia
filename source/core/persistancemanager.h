#pragma once

#include <memory>
#include <string>

namespace core
{

class IStorage;

class PersistanceManager
{
public:
  PersistanceManager(std::shared_ptr<IStorage> storage,
                     const std::string& fileName);
  ~PersistanceManager();
  void save();
  void load();

private:
  std::shared_ptr<IStorage> storage_;
  std::string fileName_;
};

}  // namespace core
