#pragma once

// #include <string>

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "example.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using example::ExampleService;
using example::HelloRequest;
using example::HelloResponse;

class ExampleServiceImpl final : public ExampleService::Service {
    Status SayHello(ServerContext* context, const HelloRequest* request,
                    HelloResponse* reply) override {
        std::string prefix("Hello, ");
        reply->set_message(prefix + request->name());
        std::cout << "Sending response: " << reply->message() << std::endl;
        return Status::OK;
    }
};


/**
 * @brief The core implementation of the executable
 *
 * This class makes up the library part of the executable, which means that the
 * main logic is implemented here. This kind of separation makes it easy to
 * test the implementation for the executable, because the logic is nicely
 * separated from the command-line logic implemented in the main function.
 */
// struct library {
  /**
   * @brief Simply initializes the name member to the name of the project
   */
  // library();

  // std::string name;
// };
