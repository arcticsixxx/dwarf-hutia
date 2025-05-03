#include <node.h>

#include "parser.h"

auto main() -> int
{
  dwarf_service::ConfigParser parser;
  auto uri = parser.getUri();

  if (!uri)
  {
    std::cerr << "unable to get uri\n";
    return 0;
  }

  core::Node node;
  node.run(uri.value());

  return 0;
}
