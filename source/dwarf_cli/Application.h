#pragma once

#include "core/cli.h"

#include "kvstore.grpc.pb.h"

namespace dwarf_cli
{

class Application
{
public:
    explicit Application();

private:
    core::CLI cli_;
};

}
