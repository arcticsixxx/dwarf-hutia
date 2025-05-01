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
      value = std::to_string(request->i());
      break;
    case kvstore::SetRequest::kF:
      value = std::to_string(request->f());
      break;
    case kvstore::SetRequest::kB:
      value = request->b() ? "true" : "false";
      break;
    case kvstore::SetRequest::VALUE_NOT_SET:
    default:
      value = "";
      break;
  }

  storage_->Set(request->key(), value);

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

}  // namespace core::network
