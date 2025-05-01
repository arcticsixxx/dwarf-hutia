#include "keyvalueservice.h"

#include "istorage.h"

namespace core::network
{

KeyValueService::KeyValueService(std::shared_ptr<IStorage> storage)
    : storage_(std::move(storage))
{
}

grpc::Status KeyValueService::Set([[maybe_unused]] grpc::ServerContext* context,
                                  const kvstore::SetRequest* request,
                                  kvstore::SetResponse* response)
{
  storage_->Set(request->key(), request->value());
  // guess its not always true
  response->set_success(true);
  return grpc::Status::OK;
}

grpc::Status KeyValueService::Get([[maybe_unused]] grpc::ServerContext* context,
                                  const kvstore::GetRequest* request,
                                  kvstore::GetResponse* response)
{
  auto value = storage_->Get(request->key());
  // response->set_found(value.has_value());
  if (!value) {
    return {grpc::StatusCode::NOT_FOUND, "Value by received key not found"};
  }

  std::visit([&](const auto& v) {
    using T = std::decay_t<decltype(v)>;
    if constexpr (std::is_same_v<T, std::string>) {
      response->set_s(v);
    } else if constexpr (std::is_same_v<T, int>) {
      response->set_i(v);
    } else if constexpr (std::is_same_v<T, float>) {
      response->set_f(v);
    } else if constexpr (std::is_same_v<T, bool>) {
      response->set_b(v);
    }
  }, value.value());

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

}  // namespace core::network
