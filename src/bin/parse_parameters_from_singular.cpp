#include <interface/parse_parameters_from_singular.hpp>

#include <boost/program_options.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

namespace buchberger_module
{
    Parameters parse_parameters_from_singular
     (
     ParametersDescription const& driver_opts,
     ParametersDescription const& workflow_opts,
     std::string const& command_opts
    )
    {
        namespace po = boost::program_options;
        ParametersDescription options;
        options.add_options()("help", "this message");
        options.add (driver_opts);
        options.add (workflow_opts);

        Parameters parameters;
        po::store ( po::command_line_parser (po::split_unix(command_opts))
              . options (options)
              . run()
              , parameters
              );

        if (parameters.count ("help"))
        {
            std::cout << options << std::endl;
            std::exit (EXIT_SUCCESS);
        }

        parameters.notify();
        return parameters;
  }
}
