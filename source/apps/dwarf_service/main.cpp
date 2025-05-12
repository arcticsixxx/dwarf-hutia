#include <node.h>

#include "parser.h"

auto main() -> int
{
  dwarf_service::ConfigParser parser;
  auto uri = parser.getUri();

  if (!uri) {
    std::cerr << "unable to get uri\n";
    return 0;
  }

  core::Node node;
  auto replica = parser.getReplica();
  if (replica) {
    std::cerr << "run replica for instance " << replica.value() << "\n";
    node.run(uri.value(), replica.value());
    return 0;
  }

  node.run(uri.value());

  return 0;
}
