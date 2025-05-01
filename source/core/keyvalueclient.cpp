#include "keyvalueclient.h"

#include <grpcpp/grpcpp.h>
#include <utils/variant.h>

namespace core::network
{

KeyValueClient::KeyValueClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(kvstore::KeyValueService::NewStub(channel))
{
}

bool KeyValueClient::Set(const std::string& key, Value value)
{
  kvstore::SetRequest request;
  request.set_key(key);

  std::visit(
      utils::overloaded {
          [&request](const std::string& str) { request.set_s(str); },
          [&request](int i) { request.set_i(i); },
          [&request](float f) { request.set_f(f); },
          [&request](bool b) { request.set_b(b); },
      },
      value);

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

  std::string value;
  switch (response.value_case()) {
    case kvstore::GetResponse::kS:
      value = response.s();
      break;
    case kvstore::GetResponse::kI:
      value = std::to_string(response.i());
      break;
    case kvstore::GetResponse::kF:
      value = std::to_string(response.f());
      break;
    case kvstore::GetResponse::kB:
      value = response.b() ? "true" : "false";
      break;
    case kvstore::GetResponse::VALUE_NOT_SET:
    default:
      value = "";
      break;
  }

  return {true, value};
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
