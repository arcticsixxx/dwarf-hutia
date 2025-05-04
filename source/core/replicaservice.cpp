#include "replicaservice.h"
#include "istorage.h"
#include "utils/variant.h"

namespace core::network {

ReplicaService::ReplicaService(IStorage * const storage,
                               const std::string &masterAddress)
  : storage_{storage}
{
  masterStub_ = kvstore::KeyValueService::NewStub(
      grpc::CreateChannel(masterAddress, grpc::InsecureChannelCredentials()));

  syncWithMaster();
}

ReplicaService::~ReplicaService()
{
  stopSync_ = true;
  if (streamContext_) {
    streamContext_->TryCancel();
  }

  if (syncThread_.joinable()) {
    syncThread_.join();
  }
}

grpc::Status ReplicaService::Get([[maybe_unused]] grpc::ServerContext *context,
                                 const kvstore::GetRequest *request,
                                 kvstore::GetResponse *response)
{
  auto value = storage_->Get(request->key());
  if (!value) {
    return {grpc::StatusCode::NOT_FOUND, "Key not found"};
  }

  std::visit(utils::overloaded {
                 [&response](const std::string& str) { response->set_s(str); },
                 [&response](int i)                { response->set_i(i); },
                 [&response](float f)              { response->set_f(f); },
                 [&response](bool b)               { response->set_b(b); }
             }, value.value());

  return grpc::Status::OK;
}

grpc::Status ReplicaService::Set([[maybe_unused]] grpc::ServerContext *context,
                                 [[maybe_unused]] const kvstore::SetRequest *request,
                                 [[maybe_unused]] kvstore::SetResponse *response)
{
  return { grpc::StatusCode::UNIMPLEMENTED, "SET method not available for replica" };
}

grpc::Status ReplicaService::Delete([[maybe_unused]] grpc::ServerContext *context,
                                    [[maybe_unused]] const kvstore::DeleteRequest *request,
                                    [[maybe_unused]] kvstore::DeleteResponse *response)
{
  return { grpc::StatusCode::UNIMPLEMENTED, "DELETE method not available for replica" };
}

void ReplicaService::syncWithMaster()
{
  kvstore::SyncRequest req;
  kvstore::FullSyncResponse resp;
  grpc::ClientContext ctx;

  grpc::Status status = masterStub_->InitialSync(&ctx, req, &resp);
  if (!status.ok()) {
    std::cerr << "[Replica] InitialSync failed: " << status.error_message() << "\n";
    return;
  }

  for (const auto& entry : resp.entries()) {
    Value value;
    switch (entry.value_case()) {
      case kvstore::KVPair::kS: value = entry.s(); break;
      case kvstore::KVPair::kI: value = entry.i(); break;
      case kvstore::KVPair::kF: value = entry.f(); break;
      case kvstore::KVPair::kB: value = entry.b(); break;
      default: continue;
    }
    storage_->Set(entry.key(), value);
  }

  syncThread_ = std::thread(&ReplicaService::handleStream, this);
}

void ReplicaService::handleStream()
{
  streamContext_ = std::make_unique<grpc::ClientContext>();
  kvstore::SyncRequest request;
  kvstore::SyncEvent event;

  streamReader_ = masterStub_->SyncStream(streamContext_.get(), request);

  while (!stopSync_ && streamReader_->Read(&event)) {
    const auto& pair = event.pair();
    const std::string& key = pair.key();

    switch (event.op()) {
      case kvstore::SET: {
        Value value;
        switch (pair.value_case()) {
          case kvstore::KVPair::kS: value = pair.s(); break;
          case kvstore::KVPair::kI: value = pair.i(); break;
          case kvstore::KVPair::kF: value = pair.f(); break;
          case kvstore::KVPair::kB: value = pair.b(); break;
          default: continue;
        }
        storage_->Set(key, value);
        break;
      }
      case kvstore::DELETE:
        storage_->Delete(key);
        break;
      default:
        break;
    }
  }

  grpc::Status status = streamReader_->Finish();
  if (!status.ok()) {
    std::cerr << "[Replica] SyncStream failed: " << status.error_message() << "\n";
  } else {
    std::cerr << "[Replica] SyncStream completed normally.\n";
  }
}


} // namespace core::network;
