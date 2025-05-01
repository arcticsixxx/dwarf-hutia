#include <thread>

#include <grpcpp/grpcpp.h>
#include <keyvalueservice.h>
#include <keyvaluestore.h>
#include <persistancemanager.h>
#include <server.h>

auto main() -> int
{
  bool isSaving {true};
  auto storage = std::make_shared<core::KeyValueStore>();
  core::PersistanceManager autosave(storage, "dump.json");
  autosave.load();
  std::thread autosave_thread(
      [&]()
      {
        while (isSaving) {
          std::this_thread::sleep_for(std::chrono::seconds(5));
          autosave.save();
        }
      });
  autosave_thread.detach();

  core::network::RunServer("0.0.0.0:50051", storage);
  isSaving = false;

  return 0;
}
