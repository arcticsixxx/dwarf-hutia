#pragma once

#include "kvstore.grpc.pb.h"
#include <grpcpp/grpcpp.h>

namespace core {
class IStorage;
}
namespace core::network {

class KeyValueService final : public kvstore::KeyValueService::Service
{
public:
    explicit KeyValueService(std::shared_ptr<IStorage> storage);

    grpc::Status Set(grpc::ServerContext *context,
                     const kvstore::SetRequest *request,
                     kvstore::SetResponse *response) override;

    grpc::Status Get(grpc::ServerContext *context,
                     const kvstore::GetRequest *request,
                     kvstore::GetResponse *response) override;

    grpc::Status Delete(grpc::ServerContext *context,
                        const kvstore::DeleteRequest *request,
                        kvstore::DeleteResponse *response) override;

private:
    std::shared_ptr<IStorage> storage_;
};

} // namespace core::network
