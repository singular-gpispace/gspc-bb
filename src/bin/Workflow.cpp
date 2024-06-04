#include <interface/Workflow.hpp>

#include <iostream>

namespace buchberger_module
{
  ParametersDescription Workflow::options() // parsing parameters from the "all_opts" string passed to sggspc_buchberger (in buchbergergsbc.lib):
  {
    namespace po = boost::program_options;

    ParametersDescription workflow_opts ("Workflow");
    workflow_opts.add_options()("N", po::value<int>()->required());
    workflow_opts.add_options()("basefilename", po::value<std::string>()->required());
    workflow_opts.add_options()("input", po::value<std::string>()->required());
    workflow_opts.add_options()("libraryname", po::value<std::string>()->required());
    workflow_opts.add_options()("deleteoutputfiles", po::value<bool>()->required());
    workflow_opts.add_options()("redSB", po::value<long>()->required());

    return workflow_opts;
  }

  Workflow::Workflow (Parameters const& args)
    : _N (args.at ("N").as<int>())
    , _input (args.at ("input").as<std::string>())
    , _basefilename (args.at ("basefilename").as<std::string>())
    , _libraryname (args.at ("libraryname").as<std::string>())
    , _redSB (args.at ("redSB").as<long>())
    {}

  ValuesOnPorts Workflow::inputs() const
  {
    ValuesOnPorts::Map values_on_ports; // writing all the input tokens onto the ports specified at the beginning of the .xpnet file:
    for(int i = 1; i <= _N ; ++i)
    {
	    values_on_ports.emplace ("input", _basefilename + _input + std::to_string(i));
    }
    values_on_ports.emplace("library_name", _libraryname);
    values_on_ports.emplace("base_filename", _basefilename);
    values_on_ports.emplace("redSB", _redSB);

    return values_on_ports;
  }

  void Workflow::process (WorkflowResult const& results, Parameters const& parameters, leftv res  ) const // processing the SINGULAR output and writing it to "res"
  {
    results.buchberger_module::WorkflowResult::assert_key_count("output",parameters.at("N").as<int>());
    lists out_list =  static_cast<lists> (omAlloc0Bin (slists_bin));
	out_list->Init (parameters.at("N").as<int>());
	int i {0};
	std::pair<int, lists> entry;
    auto& valuesOnPortsMap = results.buchberger_module::ValuesOnPorts::map();
	for(std::multimap<std::string, pnet::type::value::value_type>::const_iterator it = valuesOnPortsMap.begin(); it != valuesOnPortsMap.end(); it++)
	{
		if( boost::get<std::string>(it->first ) == "output")
		{
			entry = deserialize(boost::get<std::string>(it->second),"Result extraction", parameters.at("deleteoutputfiles").as<bool>());
			out_list->m[i].rtyp = entry.first;
			out_list->m[i].data = entry.second;
			i = i + 1;
		}
	}
	res->rtyp = LIST_CMD;
	res->data = out_list;
  }
}
