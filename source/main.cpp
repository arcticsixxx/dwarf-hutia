#include <iostream>
#include <string>

#include "lib.hpp"

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ExampleServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

auto main() -> int {
    RunServer();
  return 0;
}
