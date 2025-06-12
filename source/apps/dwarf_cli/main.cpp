#include "application.h"

auto main(int argc, char** argv) -> int
{
  std::string server_uri = "";
  if (argc == 2) {
    server_uri = argv[1];
  }

  std::cout << "connected to: " << server_uri << "\n";

  dwarf_cli::Application app(server_uri);
  app.run();

  return 0;
}
