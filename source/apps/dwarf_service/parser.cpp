#include <fstream>
#include <iostream>

#include "parser.h"

namespace
{
static constexpr const auto cfgPath = "/etc/dwarf_hutia.conf";
}

namespace dwarf_service
{

ConfigParser::ConfigParser()
{
  try {
    po::options_description desc {"Allowed options"};
    desc.add_options()(
        "port", po::value<uint32_t>()->required(), "listening port")(
        "bind",
        po::value<std::string>()->default_value("127.0.0.1"),
        "bind host")("replicaof",
                     po::value<std::string>(),
                     "uri of main instance for replica");

    std::ifstream cfg {cfgPath};
    if (cfg) {
      po::store(po::parse_config_file(cfg, desc), vm_);
    }

    po::notify(vm_);
  } catch (std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
  }
}

std::optional<std::string> ConfigParser::getUri() const
{
  if (!vm_.count("port")) {
    std::cerr << "no port specified in dwarf_hutia.conf\n";
    return {};
  }

  if (!vm_.count("bind")) {
    std::cerr << "no host specified in dwarf_hutia.conf\n";
    return {};
  }

  return {vm_["bind"].as<std::string>() + ":"
          + std::to_string(vm_["port"].as<uint32_t>())};
}

std::optional<std::string> ConfigParser::getReplica() const
{
  if (!vm_.count("replicaof")) {
    return {};
  }

  return {vm_["replicaof"].as<std::string>()};
}

}  // namespace dwarf_service
