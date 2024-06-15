#pragma once

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace buchberger_module{
    using ParametersDescription = boost::program_options::options_description;
    using Parameters = boost::program_options::variables_map;
}