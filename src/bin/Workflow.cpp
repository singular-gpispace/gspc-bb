#define PROT            0

#include <interface/Workflow.hpp>

#include <iostream>
#include <fstream>
#include <cmath>

namespace buchberger_module
{
  ParametersDescription Workflow::options() // parsing parameters from the "all_opts" string passed to sggspc_buchberger (in buchbergergsbc.lib):
  {
    namespace po = boost::program_options;

    ParametersDescription workflow_opts ("Workflow");
    //workflow_opts.add_options()("N", po::value<int>()->required());
    workflow_opts.add_options()("input", po::value<std::string>()->required());
    workflow_opts.add_options()("basefilename", po::value<std::string>()->required());
    workflow_opts.add_options()("installdir", po::value<std::string>()->required());
    workflow_opts.add_options()("deleteoutputfiles", po::value<bool>()->required());
    //workflow_opts.add_options()("nworkers", po::value<long>()->required());
    //workflow_opts.add_options()("redSB", po::value<long>()->required());
    //workflow_opts.add_options()("degBound", po::value<long>()->required());

    return workflow_opts;
  }

  Workflow::Workflow (Parameters const& args)
    : //_N (args.at ("N").as<int>()),
      _input (args.at ("input").as<std::string>())
    , _basefilename (args.at ("basefilename").as<std::string>())
    , _installdir (args.at ("installdir").as<std::string>())
    //, _nworkers (args.at ("nworkers").as<long>())
    //, _redSB (args.at ("redSB").as<long>())
    //, _degBound (args.at ("degBound").as<long>())
    {}

  ValuesOnPorts Workflow::inputs() const
  {
    ValuesOnPorts::Map values_on_ports; // writing all the input tokens onto the ports specified at the beginning of the .xpnet file:
    //for(int i = 1; i <= _N ; ++i)
    //{
	  values_on_ports.emplace ("input", _basefilename + _input);// + std::to_string(i));
    //}
    values_on_ports.emplace("base_filename", _basefilename);
    values_on_ports.emplace("installdir", _installdir);
    //values_on_ports.emplace("nworkers", _nworkers);
    //values_on_ports.emplace("redSB", _redSB);
    //values_on_ports.emplace("degBound", _degBound);

    return values_on_ports;
  }

  // types used by GPI-Space if you set the type of a place (or an "out-many" port) to "list", "set" or "map":
  using bitset     = gspc::pnet::type::bitsetofint::type;
  using bytearray  = gspc::we::type::bytearray;
  using GpiVariant = gspc::pnet::type::value::value_type;
  using GpiStruct  = gspc::pnet::type::value::structured_type;
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
    //results.buchberger_module::WorkflowResult::assert_key_count("output",parameters.at("N").as<int>());
    lists out_list =  static_cast<lists> (omAlloc0Bin (slists_bin));
  	out_list->Init (1); //(parameters.at("N").as<int>());
  	//int i {0};
  	std::pair<int, lists> entry;
    auto& valuesOnPortsMap = results.buchberger_module::ValuesOnPorts::map();
  	for(std::multimap<std::string, gspc::pnet::type::value::value_type>::const_iterator it = valuesOnPortsMap.begin(); it != valuesOnPortsMap.end(); it++)
  	{
  		if( it->first == "output")
  		{
  			entry = deserialize(boost::get<std::string>(it->second),"Result extraction", parameters.at("deleteoutputfiles").as<bool>());
  			out_list->m[0].rtyp = entry.first;
  			out_list->m[0].data = entry.second;
  			//i = i + 1;
  		}
  	}


    // summarize runtimes (measured in ms since start of algorithm; will cause int overflow only on 32-bit systems and after 24 days of runtime)

    lists transition_list = (lists) ((lists) ((lists) ((lists) out_list)->m[0].data)->m[3].data)->m[1].data;
    lists runtimes_list   = (lists) ((lists) ((lists) ((lists) out_list)->m[0].data)->m[3].data)->m[2].data;
    lists times_start_stop = (lists) (runtimes_list->m[0].data);
    lists times_sum_total  = (lists) (runtimes_list->m[1].data);

    double algorithm_starttime = 0L;
    bool prot;
    for(std::multimap<std::string, gspc::pnet::type::value::value_type>::const_iterator it = valuesOnPortsMap.begin(); it != valuesOnPortsMap.end(); it++)
  	{
      if( it->first == "singular_options")
      {
        prot = boost::get<bitset>(it->second).is_element(PROT);
      }
  		if( it->first == "runtime")
  		{
  			GpiMap runtime = get_map(it->second);

        GpiMap::const_iterator time_it;
  			for (time_it = runtime.begin(); time_it != runtime.end(); time_it++)
  			{
  				std::string transition = boost::get<std::string>(time_it->first);
          if(transition==((std::string) "TRANSITION init TOTAL"))
          {
            algorithm_starttime = boost::get<double>(get_list(time_it->second).front()); // count start of init transition as beginning of the algorithm
          }
        }
      }
    }

    long prod_crit_counter  = 0L;
    long diff_comp_counter  = 0L;
    long chain_crit_counter = 0L;
    std::map<std::string,std::map<long,long>> memory;
    for(std::multimap<std::string, gspc::pnet::type::value::value_type>::const_iterator it = valuesOnPortsMap.begin(); it != valuesOnPortsMap.end(); it++)
    {
      if( it->first == "runtime")
      {
        GpiMap runtime = get_map(it->second);

        GpiMap::const_iterator time_it;
        for (time_it = runtime.begin(); time_it != runtime.end(); time_it++)
        {
          std::string transition = boost::get<std::string>(time_it->first);
          GpiList times = get_list(time_it->second);
          GpiList::const_iterator list_it = times.begin();

          long start    = static_cast<long>(std::llround(boost::get<double>(*list_it) - algorithm_starttime)); list_it++;
          long stop     = static_cast<long>(std::llround(boost::get<double>(*list_it) - algorithm_starttime)); list_it++;
          long duration = static_cast<long>(std::llround(boost::get<double>(*list_it))); list_it++;
          long count    = boost::get<long>(*list_it);

          if(stop==-1L && duration>=0) // memory measurement (here start, duration and count will instead store the workers id, current time and current memory usage)
          {
            memory[transition][duration] = count;
          }
          else
          {
            for(int ii=2; ii<=lSize(transition_list); ii++)
            {
              std::string transition_name = reinterpret_cast<char*> (transition_list->m[ii].data);
              if(transition_name==transition)
              {
                lists times_sum        = (lists) (runtimes_list->m[ii].data);

                if(stop>=0) // for all timings:
                {
                  times_sum->m[0].data = (void*) (char*)        ( ((long) times_sum->m[0].data) + duration);
                  times_sum->m[1].data = (void*) (char*)        ( ((long) times_sum->m[1].data) + count);
                  times_sum->m[2].data = (void*) (char*) std::max(((long) times_sum->m[2].data) , duration);

                  if(start>=0) // total timings of transitions:
                  {
                    times_sum_total->m[0].data = (void*) (char*)        ( ((long) times_sum_total->m[0].data) + duration);
                    times_sum_total->m[1].data = (void*) (char*)        ( ((long) times_sum_total->m[1].data) + count);
                    times_sum_total->m[2].data = (void*) (char*) std::max(((long) times_sum_total->m[2].data) , duration);

                    times_start_stop->m[1].data = (void*) (char*) std::max(((long) times_start_stop->m[1].data) , stop); // count end of last activated transition as ending of the algorithm
                  }
                }
                else
                {
                  if (duration==-1L) // for counts, like PC, CC
                  {
                    times_sum->m[0].data = (void*) (char*)        (-1L);
                    times_sum->m[1].data = (void*) (char*)        ( ((long) times_sum->m[1].data) + count);
                    times_sum->m[2].data = (void*) (char*) std::min(((long) times_sum->m[2].data) , -1-count);
                    //times_sum->m[2].data = (void*) (char*)        (-1L);
                  }
                }
                break;
              }
            }
          }
          if(prot)
          {
            if(transition==((std::string) "PRODUCT CRITERION"))
            {
              prod_crit_counter += count;
            }
            if(transition==((std::string) "DIFFERENT COMPONENT"))
            {
              diff_comp_counter += count;
            }
            if(transition==((std::string) "CHAIN CRITERION"))
            {
              chain_crit_counter += count;
            }
          }
        }
      }
    }

    if (prot) {
      std::ofstream prot_file(_basefilename+"prot.txt", std::ios::app);
      if (diff_comp_counter==0)
        prot_file << "\nproduct criterion:" << prod_crit_counter <<  " chain criterion:" << chain_crit_counter;
      else
        prot_file << "\nproduct criterion:" << prod_crit_counter <<  " chain criterion:" << chain_crit_counter << " different component:" << diff_comp_counter;
      prot_file.close();
    }

    for (std::map<std::string,std::map<long,long>>::const_iterator worker_it = memory.begin(); worker_it != memory.end(); ++worker_it)
    {
      std::string worker_id = worker_it->first;
      std::map<long,long> memory_usage = worker_it->second;

      std::string filename = _basefilename + "memory/memory_usage__worker_"+worker_id+".csv";
      std::ofstream mem_file(filename);
      for (std::map<long,long>::const_iterator it = memory_usage.begin(); it != memory_usage.end(); ++it)
      {
        mem_file << it->first << "," << it->second << "\n";
      }
      mem_file.close();
    }

  	res->rtyp = LIST_CMD;
  	res->data = out_list;
  }
}
