#pragma once

#include <string>

#include "istorage.h"
#include "keyvalueservice.h"
#include "replicaservice.h"

namespace core::network {
// blocking for now
inline static void RunServer(const std::string& server_address,
                             IStorage* const storage)
{
  KeyValueService service(storage);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

inline static void RunReplicaServer(const std::string& server_address,
                                    const std::string& master_address,
                                    IStorage* const storage)
{
  ReplicaService service(storage, master_address);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();

}

} // namespace core::network
