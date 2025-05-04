#pragma once

#include <grpcpp/grpcpp.h>

#include "kvstore.grpc.pb.h"

namespace core {
class IStorage;
}
namespace core::network {

class KeyValueService final : public kvstore::KeyValueService::Service
{
public:
  explicit KeyValueService(IStorage* const storage);

  grpc::Status Set(grpc::ServerContext* context,
                   const kvstore::SetRequest* request,
                   kvstore::SetResponse* response) override;

  grpc::Status Get(grpc::ServerContext* context,
                   const kvstore::GetRequest* request,
                   kvstore::GetResponse* response) override;

  grpc::Status Delete(grpc::ServerContext* context,
                      const kvstore::DeleteRequest* request,
                      kvstore::DeleteResponse* response) override;

  grpc::Status InitialSync(grpc::ServerContext* context,
                           const kvstore::SyncRequest* request,
                           kvstore::FullSyncResponse* response) override;

  grpc::Status SyncStream(grpc::ServerContext* context,
                          const kvstore::SyncRequest* request,
                          grpc::ServerWriter<kvstore::SyncEvent>* writer) override;

private:
  void writeToStreams(const kvstore::SyncEvent& event);

private:
  IStorage* const storage_;

  std::mutex streamsMutex_;
  std::vector<grpc::ServerWriter<kvstore::SyncEvent>*> streams_;
};

} // namespace core::network
