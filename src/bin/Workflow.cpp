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

    return workflow_opts;
  }

  Workflow::Workflow (Parameters const& args)
    : _N (args.at ("N").as<int>())
    , _input (args.at ("input").as<std::string>())
    , _basefilename (args.at ("basefilename").as<std::string>())
    , _libraryname (args.at ("libraryname").as<std::string>())
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

    return values_on_ports;
  }


  // types used by GPI-Space if you set the type of a place (or an "out-many" port) to "list", "set" or "map":
  using GpiVariant = pnet::type::value::value_type;
  using GpiStruct  = pnet::type::value::structured_type;
  using GpiList    = std::list<GpiVariant>;
  using GpiSet     = std::set<GpiVariant>;
  using GpiMap     = std::map<GpiVariant,GpiVariant>;

  // visitor functions to convert to proper lists, sets and maps:
  template <typename T>
  class variant_visitor : public boost::static_visitor<T&>
  {
  public:
    T& operator() (T& data) const
    {
      return data;
    }

    template <typename U>
    T& operator() (U&) const
    {
     static T instance{};
     return instance;
    }
  };

  inline GpiList& get_list(GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiList>(), v);}
  inline GpiSet&  get_set (GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiSet >(), v);}
  inline GpiMap&  get_map (GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiMap >(), v);}

  inline GpiList const& get_list(GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiList const>(), v);}
  inline GpiSet const&  get_set (GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiSet  const>(), v);}
  inline GpiMap const&  get_map (GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiMap  const>(), v);}



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

  for(std::multimap<std::string, pnet::type::value::value_type>::const_iterator it = valuesOnPortsMap.begin(); it != valuesOnPortsMap.end(); it++)
	{
		if( boost::get<std::string>(it->first ) == "runtime")
		{
			GpiMap runtime = get_map(it->second);

      GpiMap::const_iterator time_it;
			for (time_it = runtime.begin(); time_it != runtime.end(); time_it++)
			{
				std::string transition = boost::get<std::string>(time_it->first);
				GpiList times = get_list(time_it->second);
        GpiList::const_iterator list_it = times.begin();
        long start    = boost::get<long>(*list_it); list_it++;
        long stop     = boost::get<long>(*list_it); list_it++;
        long duration = boost::get<long>(*list_it); list_it++;
        long count    = boost::get<long>(*list_it);

        lists transition_list = (lists) ((lists) ((lists) ((lists) out_list)->m[0].data)->m[3].data)->m[1].data;
        lists runtimes_list   = (lists) ((lists) ((lists) ((lists) out_list)->m[0].data)->m[3].data)->m[2].data;
        for(int ii=2; ii<=lSize(transition_list); ii++)
        {
          std::string transition_name = reinterpret_cast<char*> (transition_list->m[ii].data);
          if(transition_name==transition)
          {
            lists times_start_stop = (lists) (runtimes_list->m[0].data);
            lists times_sum_total  = (lists) (runtimes_list->m[1].data);
            lists times_sum        = (lists) (runtimes_list->m[ii].data);

            times_sum->m[0].data = (void*) (char*)        ( ((long) times_sum->m[0].data) + duration);
            times_sum->m[1].data = (void*) (char*)        ( ((long) times_sum->m[1].data) + count);
            times_sum->m[2].data = (void*) (char*) std::max(((long) times_sum->m[2].data) , duration);

            if(start!=0)
            {
              times_sum_total->m[0].data = (void*) (char*)        ( ((long) times_sum_total->m[0].data) + duration);
              times_sum_total->m[1].data = (void*) (char*)        ( ((long) times_sum_total->m[1].data) + count);
              times_sum_total->m[2].data = (void*) (char*) std::max(((long) times_sum_total->m[2].data) , duration);

              times_start_stop->m[1].data = (void*) (char*) std::max(((long) times_start_stop->m[1].data) , stop); // count end of last activated transition as ending of the algorithm

              if(transition==((std::string) "TRANSITION init TOTAL")) {
                times_start_stop->m[0].data = (void*) (char*) start; // count start of init transition as beginning of the algorithm
              }
            }

            break;
          }
        }
      }
		}
	}

	res->rtyp = LIST_CMD;
	res->data = out_list;
  }
}
