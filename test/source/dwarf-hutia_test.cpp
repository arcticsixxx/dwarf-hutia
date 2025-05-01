#include <assert.h>
#include <keyvaluestore.h>
#include <persistancemanager.h>

int main()
{
  auto storage = std::make_unique<core::KeyValueStore>();
  core::PersistanceManager manager(storage.get(), "testdump.json");
  storage->Set("gol", "lol");
  manager.save();
  storage->Delete("gol");
  manager.load();

  auto val = storage->Get("gol");
  assert(val && std::get<std::string>(*val) == "lol");

  return 0;
}
