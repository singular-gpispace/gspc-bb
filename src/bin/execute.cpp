#include <interface/execute.hpp>
#include <interface/Workflow.hpp>
#include <installation.hpp>

#include <drts/client.hpp>
#include <drts/drts.hpp>
#include <drts/scoped_rifd.hpp>

#include <util-generic/executable_path.hpp>

#include <boost/filesystem/path.hpp>

#include <string>

namespace buchberger_module
{
  namespace execution
  {
    ParametersDescription options()
    {
      namespace po = boost::program_options;

      ParametersDescription driver_opts ("Worker Topology");
      driver_opts.add_options()("topology", po::value<std::string>()->required());      
      driver_opts.add (gspc::options::drts());
      driver_opts.add (gspc::options::logging());
      driver_opts.add (gspc::options::scoped_rifd());

      return driver_opts;
    }
  }

  WorkflowResult execute (Parameters parameters, Workflow const& workflow)
  {
    singular_buchberger::installation installation_obj;
    gspc::installation const installation
     (installation_obj.gspc_installation (parameters));
    gspc::scoped_rifds rifds(gspc::rifd::strategy {parameters},
                             gspc::rifd::hostnames {parameters},
                             gspc::rifd::port {parameters},
                             installation);


    gspc::scoped_runtime_system drts (parameters,
                                      installation,
                                      parameters.at ("topology").as<std::string>(),
                                      rifds.entry_points());

    gspc::workflow const workflow_obj
      (installation_obj.workflow_all());

    return gspc::client {drts}.put_and_run (workflow_obj, workflow.inputs().map());
  }
}
