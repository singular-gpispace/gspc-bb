#include <Singular/libsingular.h>

#include <interface/buchberger_interface.hpp>

#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <vector>
#include <boost/variant.hpp>
#include <chrono>
#include "config.hpp"
#include "singular_functions.hpp"
//#include <fstream>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>


std::pair<int,void*> make_singular_data(long const& input, [[maybe_unused]] std::string const& ids, [[maybe_unused]] bool const& delete_file);
std::pair<int,void*> make_singular_data(std::string const& input, std::string const& ids, bool const& delete_file);
std::pair<int,void*> make_singular_data(GpiVariant const& input, std::string const& ids, bool const& delete_file);

class return_singular_data : public boost::static_visitor<std::pair<int,void*>>
{
private:
  std::string ids;
  bool delete_file;
public:
  return_singular_data(std::string const& ids, bool const& delete_file)
  {
    this->ids = ids;
    this->delete_file = delete_file;
  }

  std::pair<int,void*> operator() (const bool& data) const
  {
    return make_singular_data((long) data, this->ids, this->delete_file);
  }
  std::pair<int,void*> operator() (const int& data) const
  {
    return make_singular_data((long) data, this->ids, this->delete_file);
  }
  std::pair<int,void*> operator() (const long& data) const
  {
    return make_singular_data(data, this->ids, this->delete_file);
  }
  std::pair<int,void*> operator() (const unsigned int& data) const
  {
    return make_singular_data((long) data, this->ids, this->delete_file);
  }
  std::pair<int,void*> operator() (const long unsigned int& data) const
  {
    return make_singular_data((long) data, this->ids, this->delete_file);
  }
  std::pair<int,void*> operator() (const std::string& data) const
  {
    return make_singular_data(data, this->ids, this->delete_file);
  }
  std::pair<int,void*> operator() (const GpiList& data) const
  {
    lists L=(lists)omAllocBin(slists_bin);
    L->Init(data.size());
    std::pair<int,void*> content;
    int i=0;
    for (GpiVariant const& elem : data) {
      content = make_singular_data(elem, this->ids, this->delete_file); // recursion! (elem will be of type GpiVariant)
      L->m[i].rtyp = content.first;
     L->m[i].data = content.second;
      i++;
    }
    return std::make_pair(LIST_CMD, L);
  }

  template <typename U>
  std::pair<int,void*> operator() (const U&) const
  {
    throw std::runtime_error ("Type not implemented!");
    return std::pair<int,void*>{};
  }
};

std::pair<int,void*> make_singular_data(long const& input, [[maybe_unused]] std::string const& ids, [[maybe_unused]] bool const& delete_file)
{
  return std::make_pair(INT_CMD, (void*) (char*) (input));
}
std::pair<int,void*> make_singular_data(std::string const& input, std::string const& ids, bool const& delete_file)
{
  if(input.front()=='/' && input.back()!='/') // assume, that str_input = (path +) name of ssi-file storing the Singular object
    {return deserialize(input, ids, delete_file);}
  else                                        // pass the string directly to SINGULAR
    {return std::make_pair(STRING_CMD, (void *)omStrDup(input.c_str()));}
}
std::pair<int,void*> make_singular_data(GpiVariant const& input, std::string const& ids, bool const& delete_file)
{
  return boost::apply_visitor(return_singular_data(ids,delete_file), input);
}
std::pair<int,void*> make_singular_data(GpiList const& input, std::string const& ids, bool const& delete_file)
{
  lists L=(lists)omAllocBin(slists_bin);
	L->Init(input.size());
  std::pair<int,void*> content;
  int i=0;
  for (GpiVariant const& elem : input) {
    content = make_singular_data(elem, ids, delete_file); // recursion! (elem will be of type GpiVariant)
    L->m[i].rtyp = content.first;
  	L->m[i].data = content.second;
    i++;
  }
  return std::make_pair(LIST_CMD, L);
}
std::pair<int,void*> make_singular_data(boost::variant<long, std::string, GpiList> const& input, std::string const& ids, bool const& delete_file)
{
  switch(input.which()) {
    case 0: return make_singular_data( (boost::get<long>        (input)), ids, delete_file);
    case 1: return make_singular_data( (boost::get<std::string> (input)), ids, delete_file);
    case 2: return make_singular_data( (boost::get<GpiList>     (input)), ids, delete_file);
  }
	throw std::runtime_error ("Type not implemented!");
}
std::pair<int,void*> make_singular_data(boost::variant<long*, std::string*, GpiList*> const& input, std::string const& ids, bool const& delete_file)
{
  switch(input.which()) {
    case 0: return make_singular_data( *(boost::get<long*>        (input)), ids, delete_file);
    case 1: return make_singular_data( *(boost::get<std::string*> (input)), ids, delete_file);
    case 2: return make_singular_data( *(boost::get<GpiList*>     (input)), ids, delete_file);
  }
	throw std::runtime_error ("Type not implemented!");
}

bool write_singular_output(std::pair<int, void*> const& res, long& out_var)
{
	if(res.first != INT_CMD) {return true;}
	out_var = (long) res.second;
	return false;
}
bool write_singular_output(std::pair<int, void*> const& res, std::string& out_var, std::string const& base_filename, std::string const& singular_function_name)
{
	if(res.first == get_struct_cmd()) { // return a string that is the (path +) name of an ssi-file containing the Singular object
		out_var = serialize((lists) res.second, base_filename, singular_function_name);
		return false;
	}
	else { if(res.first == STRING_CMD) { // return the string directly
		out_var = reinterpret_cast<char*> (res.second);
		return false;
	}}
	return true;
}
bool write_singular_output(std::pair<int, void*> const& res, GpiList& out_var, std::string const& base_filename, std::string const& singular_function_name)
{
  out_var = {};
	if(res.first != LIST_CMD) {return true;}
	bool err=false;
	lists res_list = (lists) res.second;
	int n_results = lSize(res_list) + 1; // lSize gives the length of the list minus 1 !
	std::pair<int, void*> elem_data;
	for(int i=0; i<n_results && err==false; i++) {
		elem_data = std::make_pair(res_list->m[i].Typ(), res_list->m[i].Data());
		if(elem_data.first==INT_CMD) {
			long elem;
			err = write_singular_output(elem_data, elem);
			out_var.emplace_back(elem);
		}
		else { if(elem_data.first==STRING_CMD || elem_data.first==get_struct_cmd()) {
			std::string elem;
			err = write_singular_output(elem_data, elem, base_filename, singular_function_name);
			out_var.emplace_back(elem);
		}
		else { if(elem_data.first==LIST_CMD) {
			GpiList elem;
			err = write_singular_output(elem_data, elem, base_filename, singular_function_name);
			out_var.emplace_back(elem);
		}
		else {
			err = true;
		}}}
	}
	return err;
}
bool write_singular_output(std::pair<int, void*> const& res, boost::variant<long*,std::string*,GpiList*> out_ptr, std::string const& base_filename, std::string const& singular_function_name)
{
	switch(out_ptr.which()) {
		case 0: return write_singular_output(res, *boost::get<long*>        (out_ptr));
		case 1: return write_singular_output(res, *boost::get<std::string*> (out_ptr), base_filename, singular_function_name);
		case 2: return write_singular_output(res, *boost::get<GpiList*>     (out_ptr), base_filename, singular_function_name);
	}
	return true;
}



NO_NAME_MANGLING
void singular_buchberger_compute(std::string const& singular_library_name,
																 std::string const& singular_function_name,
															 	 std::string const& base_filename,
																 std::vector<boost::variant<long,std::string,GpiList>> const& args_read,
																 std::vector<boost::variant<long,std::string,GpiList>> const& args_in,
																 std::vector<boost::variant<long*,std::string*,GpiList*>> &args_inout,
																 std::vector<boost::variant<long*,std::string*,GpiList*>> &out,
																 std::vector<GpiList*> &out_many,
															 	 bool delete_files,
                                 bool silent)
{
	std::pair<int,lists> input;
	std::vector<std::pair<int, void*>> output;
	std::string ids = worker();
	int n_read     = args_read.size();
	int n_in       = args_in.size();
	int n_inout    = args_inout.size();
	int n_out      = out.size();
	int n_out_many = out_many.size();

	// start Singular and load the specified library:
	if(!silent) {std::cout << ids << " in Singular proc " << singular_function_name << std::endl;}
	init_singular (config::singularLibrary().string());
	load_singular_library(singular_library_name);

	// build input argument list:
	ScopedLeftv args;
	int i;
	for(i=0; i<n_read; i++) {
		args.add_argument(make_singular_data(args_read[i], ids, false)); // never delete files from read-only inputs!
	}
	for(i=0; i<n_in; i++) {
		args.add_argument(make_singular_data(args_in[i], ids, delete_files));
	}
	for(i=0; i<n_inout; i++) {
		args.add_argument(make_singular_data(args_inout[i], ids, delete_files));
	}

	// call the SINGULAR procedure:
	output = call_user_proc(singular_function_name, singular_library_name, args);

	// process the output:
	int token_cmd = get_struct_cmd();
	int output_size = output.size();
	if(output_size==1 && output[0].first!=INT_CMD && output[0].first!=token_cmd) // behaviour for a SINGULAR proc with no return ...
		{output_size = 0;}
	if(output_size != n_out + n_out_many + n_inout)
		{throw std::runtime_error (ids + ": wrong number of output pointers given to SINGULAR procedure " + singular_function_name
	                             + ", procedure returns " + std::to_string(output_size) + " arguments, but "

															 + std::to_string(n_out + n_out_many + n_inout) + " pointers given!"); }
	bool err;
	int j=0;
	for(i=0; i<n_inout; i++) {
		err = write_singular_output(output[j], args_inout[i], base_filename, singular_function_name); j++;
		if(err) { throw std::runtime_error (ids + ": wrong 'inout' output pointer given for the " + std::to_string(j) + "-th output of " + singular_function_name); }
	}
	for(i=0; i<n_out; i++) {
		err = write_singular_output(output[j], out[i], base_filename, singular_function_name); j++;
		if(err) { throw std::runtime_error (ids + ": wrong 'out' output pointer given for the " + std::to_string(j) + "-th output of " + singular_function_name); }
	}
	for(i=0; i<n_out_many; i++) {
		err = write_singular_output(output[j], out_many[i], base_filename, singular_function_name); j++;
		if(err) { throw std::runtime_error (ids + ": wrong 'out-many' output pointer given for the " + std::to_string(j) + "-th output of " + singular_function_name); }
	}
}



NO_NAME_MANGLING
std::pair<std::vector<std::vector<int>>,std::string> singular_buchberger_get_M_and_F(std::string const& base_filename,
                                                                                     std::string const& input,
                                                                                     GpiMap* runtime)
{
  std::string ids = worker();
  init_singular (config::singularLibrary().string());

  long start_time,stop_time;
  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  std::pair<int,void*> input_ideal = deserialize(input, ids, false);
  ideal F = (ideal) ((lists) (((lists) input_ideal.second)->m[3]).data)->m[0].data;

  if (TEST_OPT_INTSTRATEGY) {
    for(int i=0; i<F->ncols; i++)
    {
      F->m[i] = p_Cleardenom(F->m[i], currRing);
    }
  }

  writeIdealSSI(F, base_filename+"GB_for_BB_test.ssi");
  ((lists) (((lists) input_ideal.second)->m[3]).data)->m[0].data = (void*) F;
  std::string GB = serialize((lists) input_ideal.second, base_filename, (std::string) "init_GB");
  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "reading ideal for Buchberger test (init)"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  std::vector<std::vector<int>> Mvec;
  for (int i=0; i<F->ncols; i++)
  {
    std::vector<int> Mjvec;
    for (int j=1; j<=currRing->N; j++)
    {
      Mjvec.emplace_back(p_GetExp(F->m[i], j, currRing));
    }
    Mjvec.emplace_back(p_GetComp(F->m[i],currRing)); // last entry = component
    Mvec.emplace_back(Mjvec);
  }

  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "building kStrategy object for Buchberger test (init)"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  return std::make_pair(Mvec,GB);
}


NO_NAME_MANGLING
std::pair<ideal,kStrategy> singular_buchberger_get_Fstrat(std::string const& GB,
                                                          GpiMap* runtime)
{
  std::string ids = worker();
  init_singular (config::singularLibrary().string());

  long start_time,stop_time;
  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  std::pair<int,void*> input_ideal = deserialize(GB, ids, false);
  ideal F = (ideal) ((lists) (((lists) input_ideal.second)->m[3]).data)->m[0].data;
  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "reading ideal for Buchberger test (static)"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  kStrategy strat=new skStrategy;
  strat->ak = id_RankFreeModule(F,currRing);
  strat->kModW=kModW=NULL;
  strat->kHomW=kHomW=NULL;
  initBuchMoraCrit(strat);
  initBuchMoraPos(strat);
  initBba(strat);
  initBuchMora(F, currRing->qideal,strat);
  //initBuchMora:
  strat->tail = pInit();
  //- set s -
  strat->sl = -1;
  //- set L -
  strat->Lmax = ((IDELEMS(F)+setmaxLinc-1)/setmaxLinc)*setmaxLinc;
  strat->Ll = -1;
  strat->L = initL(strat->Lmax);
  //- set B -
  strat->Bmax = setmaxL;
  strat->Bl = -1;
  strat->B = initL();
  //- set T -
  strat->tl = -1;
  strat->tmax = setmaxT;
  strat->T = initT();
  strat->R = initR();
  strat->sevT = initsevT();
  //- init local data struct -
  strat->P.ecart=0;
  strat->P.length=0;
  strat->P.pLength=0;
  initS(F, currRing->qideal,strat);
  strat->fromT = FALSE;
  strat->noTailReduction = FALSE;

  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "building kStrategy object for Buchberger test (static)"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  return std::make_pair(F,strat);
}


NO_NAME_MANGLING
void singular_buchberger_compute_NF(std::pair<ideal,kStrategy> Fstrat,
                                    GpiList const& started_indices,
                                    GpiList* BB_test_fail,
                                    GpiMap* runtime)
{
  ideal F = Fstrat.first;
  kStrategy strat = Fstrat.second;

	//// start Singular and load the specified library ////
  init_singular (config::singularLibrary().string());

  // calculate NF(spoly,F)
  long start_time,stop_time;
  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

  int i = (int) boost::get<long>(started_indices.front());
  int j = (int) boost::get<long>(started_indices.back());

  LObject sPair;
  sPair.Init();
  sPair.p1=F->m[i-1];
  sPair.p2=F->m[j-1];
  ksCreateSpoly(&sPair);

  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "building s-polynomial"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  poly NF_spoly;
  int red_result=1;
  if ((sPair.p == NULL) && (sPair.t_p == NULL))
  {
    red_result = 0;
  }
  else
  {
    sPair.GetP();

    if(USE_KNF)
    {
      //alternative:
      if (TEST_OPT_INTSTRATEGY) {NF_spoly = kNF(F,currRing->qideal,sPair.p,0,4);}
      else                      {NF_spoly = kNF(F,currRing->qideal,sPair.p);}
    }
    else
    {
      // the actual reduction:
      NF_spoly=redNF(sPair.p,strat->sl,TRUE,strat);
    }

    if (NF_spoly==NULL) red_result=0;
  }
  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "reduction of s-polynomial"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  if (red_result!=0) // reduction to non-zero polynomial ==> Buchberger Test fails
  {
    (*BB_test_fail).emplace_back(started_indices);
  }
}
