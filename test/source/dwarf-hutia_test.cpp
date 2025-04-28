#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "dwarf-hutia" ? 0 : 1;
}
