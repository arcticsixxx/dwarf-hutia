#include <assert.h>
#include <keyvaluestore.h>
#include <persistancemanager.h>

int main()
{
  auto storage = std::make_shared<core::KeyValueStore>();
  core::PersistanceManager manager(storage, "testdump.json");
  storage->Set("gol", "lol");
  manager.save();
  storage->Delete("gol");
  manager.load();

  assert(storage->Get("gol") == "lol");

  return 0;
}
