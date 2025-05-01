#include "keyvalueclient.h"

#include <grpcpp/grpcpp.h>

namespace core::network
{

// auto channel = grpc::CreateChannel("localhost:50051",
// grpc::InsecureChannelCredentials()); ChannelRegistry
KeyValueClient::KeyValueClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(kvstore::KeyValueService::NewStub(channel))
{
}

bool KeyValueClient::Set(const std::string& key, const std::string& value)
{
  kvstore::SetRequest request;
  request.set_key(key);
  request.set_value(value);

  kvstore::SetResponse response;
  grpc::ClientContext context;

  grpc::Status status = stub_->Set(&context, request, &response);

  if (!status.ok()) {
    std::cerr << "Set RPC failed: " << status.error_message() << std::endl;
    return false;
  }

  return response.success();
}

std::pair<bool, std::string> KeyValueClient::Get(const std::string& key)
{
  kvstore::GetRequest request;
  request.set_key(key);

  kvstore::GetResponse response;
  grpc::ClientContext context;

  grpc::Status status = stub_->Get(&context, request, &response);

  if (!status.ok()) {
    std::cerr << "Get RPC failed: " << status.error_message() << std::endl;
    return {false, ""};
  }

  return {response.found(), response.value()};
}

bool KeyValueClient::Delete(const std::string& key)
{
  kvstore::DeleteRequest request;
  request.set_key(key);

  kvstore::DeleteResponse response;
  grpc::ClientContext context;

  grpc::Status status = stub_->Delete(&context, request, &response);

  if (!status.ok()) {
    std::cerr << "Delete RPC failed: " << status.error_message() << std::endl;
    return false;
  }

  return response.success();
}

}  // namespace core::network
