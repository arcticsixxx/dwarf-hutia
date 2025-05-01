#include <grpcpp/grpcpp.h>
#include <keyvalueservice.h>
#include <keyvaluestore.h>
#include <server.h>

auto main() -> int
{
    auto storage = std::make_shared<core::KeyValueStore>();
    core::network::RunServer("0.0.0.0:50051", storage);

    return 0;
}
