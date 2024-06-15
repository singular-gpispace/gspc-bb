#pragma once

#include <string>
#include <interface/Parameters.hpp>

namespace buchberger_module
{
  Parameters parse_parameters_from_singular
    (ParametersDescription const& execution_options,
     ParametersDescription const& workflow_options,
     std::string const& command_opts
    );
}
