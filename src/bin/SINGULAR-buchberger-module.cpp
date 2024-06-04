#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

#include <installation.hpp>

#include <util-generic/executable_path.hpp>
#include <util-generic/print_exception.hpp>
#include <util-generic/read_lines.hpp>

#include "Singular/libsingular.h"
#include <buchberger/singular_functions.hpp>

#include <interface/parse_parameters_from_singular.hpp>
#include <interface/execute.hpp>
#include <interface/Parameters.hpp>
#include <interface/Workflow.hpp>

void sggspc_print_current_exception (std::string s)
{
    WerrorS (("singular_buchberger: (" + s + ") " +
     fhg::util::current_exception_printer (": ").string()).c_str());
}

BOOLEAN sggspc_buchberger (leftv res, leftv args)
try {
    std::string command_opts = reinterpret_cast<char*> (args->Data());

    // (1) loading configuration options
    auto const parameters = buchberger_module::parse_parameters_from_singular
     (buchberger_module::execution::options(),
     buchberger_module::Workflow::options(),
     command_opts
     );

    // (2) initializing a workflow
    buchberger_module::Workflow const workflow (parameters);

    // (3) executing the workflow
    auto const results = buchberger_module::execute (parameters, workflow);

    // (4) evaluating the workflow result
    workflow.process (results, parameters, res);

    return FALSE;
}
catch (...)
{
    // need to check which resources must be tidied up
    sggspc_print_current_exception (std::string ("in sggspc_buchberger"));
    return TRUE;
}

extern "C" int mod_init (SModulFunctions* psModulFunctions)
{
    // TODO: explicit check if module has already been loaded?
    //PrintS ("DEBUG: in mod_init\n");

    psModulFunctions->iiAddCproc
     ((currPack->libname ? currPack->libname : ""),
      "sggspc_buchberger", FALSE, sggspc_buchberger);

  return MAX_TOK;
}
