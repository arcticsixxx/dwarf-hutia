#include "Application.h"

#include <grpcpp/grpcpp.h>

namespace dwarf_cli
{

Application::Application()
{
    // std::string server_address("localhost:50051");
    // auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    // auto stub = kvstore::KeyValueService::NewStub(channel);
}

}
