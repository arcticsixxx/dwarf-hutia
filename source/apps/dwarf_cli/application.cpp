#include "application.h"

#include <grpcpp/grpcpp.h>

#include "commands.h"

namespace dwarf_cli
{

Application::Application(const std::string& server_uri)
    : client_ {
          grpc::CreateChannel(server_uri, grpc::InsecureChannelCredentials())}
{
  cli_.registerCommand<core::GET>(client_);
  cli_.registerCommand<core::SET>(client_);
  cli_.registerCommand<core::DELETE>(client_);
}

void Application::run()
{
  std::string str;
  while (std::getline(std::cin, str)) {
    if (str == "/EXIT") {
      break;
    }
    cli_.addArg(str);
  }
}

}  // namespace dwarf_cli
