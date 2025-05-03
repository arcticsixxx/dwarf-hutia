#include <thread>

#include "node.h"

#include "keyvaluestore.h"
#include "server.h"

namespace core
{

Node::Node()
    : storage_ {std::make_unique<KeyValueStore>()}
    , dump_ {storage_.get(), "dump.json"}
{
}

void Node::run(const std::string& addr_uri)
{
  bool isSaving {true};
  dump_.load();
  std::thread autosave_thread(
      [&]()
      {
        while (isSaving) {
          std::this_thread::sleep_for(std::chrono::seconds(5));
          dump_.save();
        }
      });
  autosave_thread.detach();

  network::RunServer(addr_uri, storage_.get());
  isSaving = false;
}

}  // namespace core
