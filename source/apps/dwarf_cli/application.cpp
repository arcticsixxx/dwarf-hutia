#include "application.h"

#include <grpcpp/grpcpp.h>

namespace dwarf_cli
{

Application::Application(const std::string& server_uri)
    : client_ {grpc::CreateChannel(server_uri,
                                   grpc::InsecureChannelCredentials())}
{
  cli_.addHandler("GET",
                  [this](const std::string& arg) -> std::error_code
                  {
                    if (arg.empty()) {
                      std::cerr << "Error: Command string is empty!\n";
                      return std::make_error_code(std::errc::invalid_argument);
                    }

                    std::cout << client_.Get(arg).second << "\n";
                    return {};
                  });

  cli_.addHandler("SET",
                  [this](const std::string& arg) -> std::error_code
                  {
                    if (arg.empty()) {
                      std::cerr << "Error: Command string is empty!\n";
                      return std::make_error_code(std::errc::invalid_argument);
                    }

                    std::string key, value;
                    std::istringstream iss {arg};
                    iss >> key >> value;

                    client_.Set(key, value);
                    return {};
                  });

  cli_.addHandler("DELETE",
                  [this](const std::string& arg) -> std::error_code
                  {
                    if (arg.empty()) {
                      std::cerr << "Error: Command string is empty!\n";
                      return std::make_error_code(std::errc::invalid_argument);
                    }

                    client_.Delete(arg);

                    return {};
                  });
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
