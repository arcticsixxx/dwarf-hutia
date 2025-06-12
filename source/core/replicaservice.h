#pragma once

#include <thread>
#include <grpcpp/grpcpp.h>

#include "kvstore.grpc.pb.h"

namespace core {
class IStorage;
}
namespace core::network
{
class ReplicaService final : public kvstore::KeyValueService::Service
{
public:
  explicit ReplicaService(IStorage* const storage,
                          const std::string& masterAddress);

  ~ReplicaService();

  grpc::Status Get(grpc::ServerContext* context,
                   const kvstore::GetRequest* request,
                   kvstore::GetResponse* response) override;

  grpc::Status Set(grpc::ServerContext* context,
                   const kvstore::SetRequest* request,
                   kvstore::SetResponse* response) override;

  grpc::Status Delete(grpc::ServerContext* context,
                      const kvstore::DeleteRequest* request,
                      kvstore::DeleteResponse* response) override;

private:
  void syncWithMaster();

  void handleStream();

private:
  IStorage* const storage_;
  std::unique_ptr<kvstore::KeyValueService::Stub> masterStub_;
  std::unique_ptr<grpc::ClientContext> streamContext_;
  std::unique_ptr<grpc::ClientReader<kvstore::SyncEvent>> streamReader_;
  std::thread syncThread_;
  std::atomic<bool> stopSync_{false};
};
} // namespace core::network
