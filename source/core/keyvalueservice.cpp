#include "keyvalueservice.h"

#include "istorage.h"

namespace core::network {

KeyValueService::KeyValueService(std::shared_ptr<IStorage> storage)
    : storage_(std::move(storage))
{}

grpc::Status KeyValueService::Set([[maybe_unused]] grpc::ServerContext *context,
                                  const kvstore::SetRequest *request,
                                  kvstore::SetResponse *response)
{
    storage_->Set(request->key(), request->value());
    // guess its not always true
    response->set_success(true);
    return grpc::Status::OK;
}

grpc::Status KeyValueService::Get([[maybe_unused]] grpc::ServerContext *context,
                                  const kvstore::GetRequest *request,
                                  kvstore::GetResponse *response)
{
    auto value = storage_->Get(request->key());
    // response->set_found(value.has_value());
    if (!value) {
        return { grpc::StatusCode::NOT_FOUND, "Value by recived key not found" };
    }

    response->set_value(value.value());
    return grpc::Status::OK;
}

grpc::Status KeyValueService::Delete([[maybe_unused]] grpc::ServerContext *context,
                                     const kvstore::DeleteRequest *request,
                                     kvstore::DeleteResponse *response)
{
    response->set_success(storage_->Delete(request->key()));

    return grpc::Status::OK;
}

} // namespace core::network
