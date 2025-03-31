#pragma once

#include <string>
#include <interface/Parameters.hpp>
#include <interface/ValuesOnPorts.hpp>
#include <interface/WorkflowResult.hpp>

#include "Singular/libsingular.h"
#include <buchberger/singular_functions.hpp>

namespace buchberger_module
{
  class Workflow
  {
  public:
    static ParametersDescription options();

    Workflow (Parameters const& parameters);

    ValuesOnPorts inputs() const;

    void  process (WorkflowResult const& result, Parameters const& parameters, leftv res) const;

  private:
    int _N;
    std::string _input;
    std::string _basefilename;
  };
}
