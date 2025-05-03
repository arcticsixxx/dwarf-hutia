#pragma once

#include <boost/program_options.hpp>


namespace po = boost::program_options;

namespace dwarf_service
{

class ConfigParser
{
public:
  explicit ConfigParser();

  std::optional<std::string> getUri() const;

private:
  po::variables_map vm_;
};

} // namespace dwarf_service
