#include "core/istorage.h"
#include <string>

#pragma once

namespace core::network {
// blocking for now
static void RunServer(const std::string &server_address, std::shared_ptr<IStorage> storage);

} // namespace core::network
