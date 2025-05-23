#pragma once

#include "cli.h"
#include "keyvalueclient.h"

namespace dwarf_cli
{

class Application
{
public:
    explicit Application();

    void run();

private:
    core::CLI cli_;
    core::network::KeyValueClient client_;
};

} // namespace dwarf_cli
