#include <fstream>
#include <iostream>

#include "persistancemanager.h"

#include <nlohmann/json.hpp>

#include "istorage.h"

namespace core
{

PersistanceManager::PersistanceManager(std::shared_ptr<IStorage> storage,
                                       const std::string& fileName)
    : storage_ {storage}
    , fileName_ {fileName}
{
}

PersistanceManager::~PersistanceManager() {}

void PersistanceManager::save()
{
  // nlohmann::json json(storage_->snapshot());

  // std::ofstream file(fileName_);
  // file << json.dump();
}

void PersistanceManager::load()
{
  std::ifstream file(fileName_);
  if (not file) {
    std::cerr << "TODO handle me" << std::endl;
    return;
  }
  nlohmann::json json;

  file >> json;

  // for (auto& [key, value] : json.items()) {
  //   storage_->Set(key, value);
  // }
}

}  // namespace core
