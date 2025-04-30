#include <core/network/keyvalueservice.h>
#include <core/network/server.h>
#include <grpcpp/grpcpp.h>
#include <keyvaluestore.h>

auto main() -> int
{
    auto storage = std::make_shared<core::KeyValueStore>();
    core::network::RunServer("0.0.0.0:50051", storage);

    return 0;
}
