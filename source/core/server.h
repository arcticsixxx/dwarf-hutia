#pragma once

#include <string>

#include "istorage.h"
#include "keyvalueservice.h"

namespace core::network {
// blocking for now
inline static void RunServer(const std::string &server_address, std::shared_ptr<IStorage> storage)
{
    KeyValueService service(std::move(storage));

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

} // namespace core::network
