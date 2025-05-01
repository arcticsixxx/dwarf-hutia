#pragma once

#include <memory>
#include <string>

namespace core
{

class IStorage;

class PersistanceManager
{
public:
  explicit PersistanceManager(IStorage* const storage,
                              const std::string& fileName);
  ~PersistanceManager();
  void save();
  void load();

private:
  IStorage* const storage_;
  std::string fileName_;
};

}  // namespace core
