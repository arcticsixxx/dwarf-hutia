#pragma once

#include <memory>

#include "istorage.h"
#include "keyvalueservice.h"
#include "persistancemanager.h"

namespace core
{

// TODO NodeBuilder

class Node
{
public:
  Node();
  void run();

  // set
  // get

private:
  std::unique_ptr<IStorage> storage_;
  // network::KeyValueService service_;
  PersistanceManager dump_;

  // nodeId
  // status eg heart beat
  // InternalNodeStub
};

}  // namespace core
