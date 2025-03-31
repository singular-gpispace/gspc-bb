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
std::vector<std::vector<int>> singular_buchberger_get_M_and_init_F(std::string const& base_filename,
                                                                   std::string const& input,
                                                                   GpiMap* runtime)
{
  std::string ids = worker();
  init_singular (config::singularLibrary().string());

  long start_time,stop_time;
  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

  //read ideal from input
  std::pair<int,void*> input_ideal = deserialize(input, ids, false);
  ideal F = (ideal) ((lists) (((lists) input_ideal.second)->m[3]).data)->m[0].data;

  if (TEST_OPT_INTSTRATEGY) {
    for(int i=0; i<F->ncols; i++)
    {
      F->m[i] = p_Cleardenom(F->m[i], currRing);
    }
  }
  else
  {
    for(int i=0; i<F->ncols; i++)
    {
      p_Norm(F->m[i], currRing);
    }
  }

  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "reading input ideal in init"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

  ideal F_sorted = idInit(IDELEMS(F),F->rank);
  intvec *sort   = idSort(F);
  for (int i=0; i<sort->length();++i)
    {F_sorted->m[i] = F->m[(*sort)[i]-1];}

  kStrategy strat=new skStrategy;
  ideal FF = idInit(1,1);
  idInsertPolyOnPos(FF,F_sorted->m[0],0); // insert first polynomial of F
  writePolySSI(F_sorted->m[0], base_filename+"f1");

  for(int i=1; i<F->ncols; i++)
  {
    FF->rank = id_RankFreeModule(FF, currRing, currRing);
    if (FF->rank==0) {FF->rank=1;}

    // take next polynomial, ...
    poly new_f = F_sorted->m[i];

    // update strat
    strat->ak = id_RankFreeModule(FF,currRing);
    strat->kModW=kModW=NULL;
    strat->kHomW=kHomW=NULL;
    initBuchMoraCrit(strat);
    initBuchMoraPos(strat);
    initBba(strat);
    initBuchMora(FF, currRing->qideal,strat);
    //initBuchMora:
    strat->tail = pInit();
    //- set s -
    strat->sl = -1;
    //- set L -
    strat->Lmax = ((IDELEMS(FF)+setmaxLinc-1)/setmaxLinc)*setmaxLinc;
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
    //- init local data struct.----------------------------------------
    strat->P.ecart=0;
    strat->P.length=0;
    strat->P.pLength=0;
    initS(FF, currRing->qideal,strat); //sets also S, ecartS, fromQ
    strat->fromT = FALSE;
    strat->noTailReduction = FALSE;

    int sl=strat->sl;

    // reduce new_f by previously added elements:
    new_f = redNF(new_f,sl,TRUE,strat);

    writePolySSI(new_f, base_filename+"f"+std::to_string(i+1));
    idInsertPolyOnPos(FF,new_f,i);
  }

  if (TEST_OPT_INTSTRATEGY) {
    for(int i=0; i<FF->ncols; i++)
    {
      FF->m[i] = p_Cleardenom(FF->m[i], currRing);
    }
  }
  else
  {
    for(int i=0; i<FF->ncols; i++)
    {
      p_Norm(FF->m[i], currRing);
    }
  }

  //building Mvec
  std::vector<std::vector<int>> Mvec;
  for (int i=0; i<FF->ncols; i++)
  {
    std::vector<int> Mjvec;
    for (int j=1; j<=currRing->N; j++)
    {
      Mjvec.emplace_back(p_GetExp(FF->m[i], j, currRing));
    }
    Mjvec.emplace_back(p_GetComp(FF->m[i],currRing)); // last entry = component
    Mvec.emplace_back(Mjvec);
  }

  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "saving initial ideal elements in init"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  return Mvec;
}


NO_NAME_MANGLING
void singular_buchberger_compute_NF(std::string const& base_filename,
                                    std::list<poly> const& generators,
                                    int r,
                                    int Qback_i,
                                    int Qback_j,
                                    int index_i,
                                    int index_j,
                                    int old_r,
                                    GpiMap* runtime,
                                    GpiList* started_indices_out,
                                    GpiList* finished_indices,
                                    GpiList* NF)
{
	std::string ids = worker();
  std::string save_filename = base_filename+"intermediate_result_"+std::to_string(index_i)+"_"+std::to_string(index_j);

	//// start Singular ////
	init_singular (config::singularLibrary().string());
  long start_time,stop_time;

  poly NF_spoly;

  if(old_r==r && index_i==Qback_i && index_j==Qback_j)
  {
    // (non-zero) reduction result from a previous computation with the same r that now moved to the back of the queue
    // pass straight to place_NF:
    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    NF_spoly = readPolySSI(save_filename, false);
    std::rename(save_filename.c_str(), (base_filename + "f"+std::to_string(r+1)).c_str()); // NF_spoly can't be reduced further, so just rename its file to the new generator f{r+1}
    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "reading partially reduced poly in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

    GpiList m;
    int n = currRing->N; // number of variables
    for(int k=1; k<=n; k++)
    {
      m.emplace_back((int) p_GetExp(NF_spoly,k,currRing));
    }
    m.emplace_back((int) p_GetComp(NF_spoly, currRing));

    (*NF).emplace_back(GpiList({index_i, index_j,m}));

    return;
  }

  //// calculate NF(spoly(F[i],F[j]), F) ////
  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  ideal F = idInit(r,1);
  std::list<poly>::const_iterator gen = generators.begin();
  for(int ii=1; ii<=r; ii++)
  {
    idInsertPolyOnPos(F, *gen, ii-1); ++gen;
  }
  F->rank = id_RankFreeModule(F, currRing, currRing);
  if (F->rank==0) F->rank=1;
  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "reading generators in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  if (old_r == 0)
  {
    LObject Pair;
    Pair.Init();
    Pair.p1=F->m[index_i-1];
    Pair.p2=F->m[index_j-1];
    ksCreateSpoly(&Pair);

    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    if (USE_KNF)
    {
      if (TEST_OPT_INTSTRATEGY) {NF_spoly = kNF(F,currRing->qideal,Pair.p,0,4);}
      else                      {NF_spoly = kNF(F,currRing->qideal,Pair.p);}
    }
    else
    {
      kStrategy strat=new skStrategy;
      strat->ak = id_RankFreeModule(F,currRing);
      strat->kModW=kModW=NULL;
      strat->kHomW=kHomW=NULL;
      initBuchMoraCrit(strat);
      initBuchMoraPos(strat);
      initBba(strat);
      initBuchMora(F, currRing->qideal,strat);
      /*initBuchMora:*/
      strat->tail = pInit();
      /*- set s -*/
      strat->sl = -1;
      /*- set L -*/
      strat->Lmax = ((IDELEMS(F)+setmaxLinc-1)/setmaxLinc)*setmaxLinc;
      strat->Ll = -1;
      strat->L = initL(strat->Lmax);
      /*- set B -*/
      strat->Bmax = setmaxL;
      strat->Bl = -1;
      strat->B = initL();
      /*- set T -*/
      strat->tl = -1;
      strat->tmax = setmaxT;
      strat->T = initT();
      strat->R = initR();
      strat->sevT = initsevT();
      /*- init local data struct -*/
      strat->P.ecart=0;
      strat->P.length=0;
      strat->P.pLength=0;
      initS(F, currRing->qideal,strat);
      strat->fromT = FALSE;
      strat->noTailReduction = FALSE;

      int sl = strat->sl;
      //long start_time_redNF = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      NF_spoly = redNF(Pair.p,sl,TRUE,strat);
      //long stop_time_redNF = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      //(*runtime)[(std::string) "just the call to redNF(..) in NF_of_spoly"] = GpiList({-1L, stop_time_redNF, stop_time_redNF-start_time_redNF, 1L});
    }

    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "applying NF in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});
    }
  else
  {
    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    NF_spoly = readPolySSI(save_filename, true);
    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "reading partially reduced poly in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    if (USE_KNF)
    {
      if (TEST_OPT_INTSTRATEGY) {NF_spoly = kNF(F,currRing->qideal,NF_spoly,0,4);}
      else                      {NF_spoly = kNF(F,currRing->qideal,NF_spoly);}
    }
    else
    {
      kStrategy strat=new skStrategy;
      strat->ak = id_RankFreeModule(F,currRing);
      strat->kModW=kModW=NULL;
      strat->kHomW=kHomW=NULL;
      initBuchMoraCrit(strat); /*set Gebauer, honey, sugarCrit*/
      initBuchMoraPos(strat);
      initBba(strat);
      initBuchMora(F, currRing->qideal,strat);
      /*initBuchMora:*/
      strat->tail = pInit();
      /*- set s -*/
      strat->sl = -1;
      /*- set L -*/
      strat->Lmax = ((IDELEMS(F)+setmaxLinc-1)/setmaxLinc)*setmaxLinc;
      strat->Ll = -1;
      strat->L = initL(strat->Lmax);
      /*- set B -*/
      strat->Bmax = setmaxL;
      strat->Bl = -1;
      strat->B = initL();
      /*- set T -*/
      strat->tl = -1;
      strat->tmax = setmaxT;
      strat->T = initT();
      strat->R = initR();
      strat->sevT = initsevT();
      /*- init local data struct.---------------------------------------- -*/
      strat->P.ecart=0;
      strat->P.length=0;
      strat->P.pLength=0;
      initS(F, currRing->qideal,strat); /*sets also S, ecartS, fromQ */
      strat->fromT = FALSE;
      strat->noTailReduction = FALSE;

      int sl = strat->sl;

      //long start_time_redNF = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      NF_spoly = redNF(NF_spoly,sl,TRUE,strat);
      //long stop_time_redNF = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      //(*runtime)[(std::string) "just the call to redNF(..) in NF_of_spoly"] = GpiList({-1L, stop_time_redNF, stop_time_redNF-start_time_redNF, 1L});
    }

    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "applying NF in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});
  }

  if (NF_spoly==NULL) // reduction to 0 ==> pair is finished (and to be removed from Q)
  {
    (*finished_indices).emplace_back(index_i);
    (*finished_indices).emplace_back(index_j);
  }
  else
  {
    if (index_i==Qback_i && index_j==Qback_j) // element at end of Q ==> add as new GB element
    {
      writePolySSI(NF_spoly, base_filename + "f"+std::to_string(r+1));

      GpiList m;
      int n = currRing->N; // number of variables
      for(int k=1; k<=n; k++)
      {
        m.emplace_back((int) p_GetExp(NF_spoly,k,currRing));
      }
      m.emplace_back((int) p_GetComp(NF_spoly, currRing));

      (*NF).emplace_back(GpiList({index_i, index_j,m}));
    }
    else // element not at end of Q ==> put back to started indices (to be reduced further in future)
    {
      start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      writePolySSI(NF_spoly, save_filename);
      stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      (*runtime)[(std::string) "saving partially reduced poly in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

      (*started_indices_out).emplace_back(index_i);
      (*started_indices_out).emplace_back(index_j);
    }
  }
}


NO_NAME_MANGLING
poly read_generator (std::string const& base_filename,
                     int k)
{
  init_singular (config::singularLibrary().string());
  return readPolySSI(base_filename+"f"+std::to_string(k),false);
}


NO_NAME_MANGLING
void singular_buchberger_reduce_GB (std::string const& base_filename,
                                    std::list<poly> const& generators,
                                    long needed_indices,
                                    int current_index,
                                    int final_r,
                                    long redSB,
                                    GpiMap* runtime)
{
  init_singular (config::singularLibrary().string());

  poly f; // poly ro be reduced and saved in a file

  long start_time,stop_time;

  if (redSB)
  {
    ideal F = idInit(final_r-1,1);
    std::list<poly>::const_iterator gen = generators.begin();
    int ii=0;
    for(int i=0; i<final_r; i++)
    {
      if(i==current_index)
        {f = p_Copy(*gen, currRing, currRing);}
      else
      {
        idInsertPolyOnPos(F, *gen, ii);
       ii++;
      }
      ++gen;
    }
    F->rank = id_RankFreeModule(F, currRing, currRing);
    if (F->rank==0) F->rank=1;

    // start reduction
    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    if (USE_KNF || true)
    {
      if (TEST_OPT_INTSTRATEGY) {f = kNF(F,currRing->qideal,f,0,4);}
      else                      {f = kNF(F,currRing->qideal,f);}
    }
    else
    {
      kStrategy strat=new skStrategy;
      strat->ak = id_RankFreeModule(F,currRing);
      strat->kModW=kModW=NULL;
      strat->kHomW=kHomW=NULL;
      initBuchMoraCrit(strat); /*set Gebauer, honey, sugarCrit*/
      initBuchMoraPos(strat);
      initBba(strat);
      initBuchMora(F, currRing->qideal,strat);
      /*initBuchMora:*/
      strat->tail = pInit();
      /*- set s -*/
      strat->sl = -1;
      /*- set L -*/
      strat->Lmax = ((IDELEMS(F)+setmaxLinc-1)/setmaxLinc)*setmaxLinc;
      strat->Ll = -1;
      strat->L = initL(strat->Lmax);
      /*- set B -*/
      strat->Bmax = setmaxL;
      strat->Bl = -1;
      strat->B = initL();
      /*- set T -*/
      strat->tl = -1;
      strat->tmax = setmaxT;
      strat->T = initT();
      strat->R = initR();
      strat->sevT = initsevT();
      /*- init local data struct -*/
      strat->P.ecart=0;
      strat->P.length=0;
      strat->P.pLength=0;
      initS(F, currRing->qideal,strat);
      strat->fromT = FALSE;
      strat->noTailReduction = FALSE;

      int sl = strat->sl;

      //long start_time_redNF = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      f = redNF(f,sl,TRUE,strat);
      //long stop_time_redNF = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      //(*runtime)[(std::string) "just the call to redNF(..) in reduce_GB"] = GpiList({-1L, stop_time_redNF, stop_time_redNF-start_time_redNF, 1L});
    }
    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "applying NF in reduce_GB"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});
  }
  else
  {
    f = readPolySSI(base_filename+"f"+std::to_string(needed_indices),false);
  }

  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  if (TEST_OPT_INTSTRATEGY)
  {
    f = p_Cleardenom(f, currRing);
  }
  else
  {
    p_Norm(f, currRing);
  }
  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "clearing denominators in reduce_GB"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  writePolySSI(f, base_filename + "g" + std::to_string(current_index+1));
  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "saving GB in files in reduce_GB"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});
}
