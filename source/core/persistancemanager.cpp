#include <fstream>
#include <iostream>

#include "persistancemanager.h"

#include <nlohmann/json.hpp>
#include <utils/variant.h>

#include "istorage.h"

namespace
{

core::Value jsonToValue(const nlohmann::json& j)
{
  if (j.is_string()) {
    return j.get<std::string>();
  }
  if (j.is_number_integer()) {
    return j.get<int>();
  }
  if (j.is_number_float()) {
    return j.get<float>();
  }
  if (j.is_boolean()) {
    return j.get<bool>();
  }

  return 0;
  // return std::nullopt
  // TODO handle
}

}  // namespace

namespace core
{

PersistanceManager::PersistanceManager(IStorage* const storage,
                                       const std::string& fileName)
    : storage_ {storage}
    , fileName_ {fileName}
{
  assert(storage);
}

PersistanceManager::~PersistanceManager() {}

void PersistanceManager::save()
{
  nlohmann::json j;

  for (const auto& [key, value] : storage_->snapshot()) {
    std::visit(utils::overloaded {[&](const std::string& s) { j[key] = s; },
                                  [&](int i) { j[key] = i; },
                                  [&](float f) { j[key] = f; },
                                  [&](bool b) { j[key] = b; }},
               value);
  }

  std::ofstream file(fileName_);
  file << j.dump();
}

void PersistanceManager::load()
{
  std::ifstream file(fileName_);
  if (not file) {
    std::cerr << "TODO handle me" << std::endl;
    return;
  }

  nlohmann::json j;

  file >> j;

  if (!j.is_object()) {
    // throw std::runtime_error("Expected JSON object");
    // TODO handle
  }

  for (auto& [key, value] : j.items()) {
    storage_->Set(key, jsonToValue(value));
  }
}

}  // namespace core
