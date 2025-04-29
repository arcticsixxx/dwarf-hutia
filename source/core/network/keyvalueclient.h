#pragma once

#include "kvstore.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#include <memory>
#include <string>

namespace core::network {

class KeyValueClient
{
public:
    // multiple client can use one channel
    // for example when connecting to a one node
    // maybe it is excessively here

    KeyValueClient(std::shared_ptr<grpc::Channel> channel);

    // TODO should handle grpc fails and internal kv core fails
    bool Set(const std::string &key, const std::string &value);
    std::pair<bool, std::string> Get(const std::string &key);
    bool Delete(const std::string &key);

private:
    std::unique_ptr<kvstore::KeyValueService::Stub> stub_;
};

} // namespace core::network
