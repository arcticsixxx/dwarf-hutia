#include <iostream>
#include <memory>
#include <string>

#include "example.grpc.pb.h"
#include <grpcpp/grpcpp.h>

using example::ExampleService;
using example::HelloRequest;
using example::HelloResponse;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class ExampleServiceClient {
public:
  ExampleServiceClient(std::shared_ptr<Channel> channel)
      : stub_(ExampleService::NewStub(channel)) {}

  std::string SayHello(const std::string &name) {
    HelloRequest request;
    request.set_name(name);

    HelloResponse reply;
    ClientContext context;

    Status status = stub_->SayHello(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << "RPC failed: " << status.error_message() << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<ExampleService::Stub> stub_;
};

int main(int argc, char **argv) {
  std::string target_str("localhost:50051");

  // Instantiate the client
  ExampleServiceClient client(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  std::string name("World");
  if (argc > 1) {
    name = argv[1];
  }

  std::string reply = client.SayHello(name);
  std::cout << "Received: " << reply << std::endl;

  return 0;
}
