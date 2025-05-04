#include <thread>
#include "keyvalueservice.h"

#include "istorage.h"
#include "utils/variant.h"

namespace core::network
{

KeyValueService::KeyValueService(IStorage* const storage)
    : storage_(storage)
{
}

grpc::Status KeyValueService::Set([[maybe_unused]] grpc::ServerContext* context,
                                  const kvstore::SetRequest* request,
                                  kvstore::SetResponse* response)
{
  Value value;
  switch (request->value_case()) {
    case kvstore::SetRequest::kS:
      value = request->s();
      break;
    case kvstore::SetRequest::kI:
      value = request->i();
      break;
    case kvstore::SetRequest::kF:
      value = request->f();
      break;
    case kvstore::SetRequest::kB:
      value = request->b();
      break;
    case kvstore::SetRequest::VALUE_NOT_SET:
    default:
      value = "";
      break;
  }

  storage_->Set(request->key(), value);

  kvstore::SyncEvent event;
  event.set_op(kvstore::SET);

  kvstore::KVPair* entry = event.mutable_pair();
  entry->set_key(request->key());
  std::visit(utils::overloaded {
                 [&entry](const std::string& str) { entry->set_s(str); },
                 [&entry](int i)                { entry->set_i(i); },
                 [&entry](float f)              { entry->set_f(f); },
                 [&entry](bool b)               { entry->set_b(b); }
             }, value);

  writeToStreams(event);

  // guess its not always true
  response->set_success(true);

  return grpc::Status::OK;
}

grpc::Status KeyValueService::Get([[maybe_unused]] grpc::ServerContext* context,
                                  const kvstore::GetRequest* request,
                                  kvstore::GetResponse* response)
{
  auto value = storage_->Get(request->key());

  if (!value) {
    return {grpc::StatusCode::NOT_FOUND, "Value by received key not found"};
  }

  std::visit(utils::overloaded {[&response](const std::string& str)
                                { response->set_s(str); },
                                [&response](int i) { response->set_i(i); },
                                [&response](float f) { response->set_f(f); },
                                [&response](bool b) { response->set_b(b); }},
             value.value());

  return grpc::Status::OK;
}

grpc::Status KeyValueService::Delete(
    [[maybe_unused]] grpc::ServerContext* context,
    const kvstore::DeleteRequest* request,
    kvstore::DeleteResponse* response)
{
  response->set_success(storage_->Delete(request->key()));

  return grpc::Status::OK;
}

grpc::Status KeyValueService::InitialSync([[maybe_unused]] grpc::ServerContext *context,
                                          [[maybe_unused]] const kvstore::SyncRequest *request,
                                          kvstore::FullSyncResponse *response)
{
  std::lock_guard<std::mutex> lock{streamsMutex_};
  for (const auto& [key, value] : storage_->snapshot())
  {
    kvstore::KVPair* entry = response->add_entries();
    entry->set_key(key);

    std::visit(utils::overloaded {
                   [&entry](const std::string& str) { entry->set_s(str); },
                   [&entry](int i)                { entry->set_i(i); },
                   [&entry](float f)              { entry->set_f(f); },
                   [&entry](bool b)               { entry->set_b(b); }
               }, value);
  }

  return grpc::Status::OK;
}

grpc::Status KeyValueService::SyncStream(grpc::ServerContext *context,
                                         [[maybe_unused]] const kvstore::SyncRequest *request,
                                         grpc::ServerWriter<kvstore::SyncEvent> *writer)
{
  std::lock_guard<std::mutex> lock{streamsMutex_};
  streams_.push_back(writer);

  while (!context->IsCancelled()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return grpc::Status::OK;
}

void KeyValueService::writeToStreams(const kvstore::SyncEvent &event)
{
  std::lock_guard<std::mutex> lock{streamsMutex_};
  for (auto* writer : streams_) {
    writer->Write(event);
  }
}

}  // namespace core::network
