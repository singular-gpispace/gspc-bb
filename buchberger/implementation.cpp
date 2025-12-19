#include <Singular/libsingular.h>

#include <interface/buchberger_interface.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unistd.h>
#include <vector>
#include <boost/variant.hpp>
#include <chrono>
#include "config.hpp"
#include "singular_functions.hpp"
#include "sys/stat.h"
#include "sys/types.h"
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
void singular_init(std::string const& base_filename,
                   std::string const& input,
                   bool prev_queue_had_started,
                   std::vector<std::vector<std::vector<int> > >* Mvec,
                   GpiList* degBounds,
                   long* redSB,
                   long* nworkers,
                   int* prev_r,
                   GpiMap* runtime)
{
  std::string ids = worker();
  init_singular (config::singularLibrary().string());

  long start_time,stop_time;
  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

  // read ideal from input
  std::pair<int,void*> input_token = deserialize(input, ids, true);
  lists inputList = (lists) (((lists) input_token.second)->m[3]).data;
  ideal F = (ideal) inputList->m[0].data;
  // read degree bound(s)
  if( write_singular_output(std::make_pair((inputList->m[1]).Typ(), static_cast<void*> ((inputList->m[1]).Data())), degBounds, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading degree bounds list in init!"); }
  // read redSB
  if( write_singular_output(std::make_pair((inputList->m[2]).Typ(), static_cast<void*> ((inputList->m[2]).Data())), redSB, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading redSB in init!"); }
  // read nworkers
  if( write_singular_output(std::make_pair((inputList->m[3]).Typ(), static_cast<void*> ((inputList->m[3]).Data())), nworkers, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading nworkers in init!"); }
  // read force_continue
  long force_continue=0;
  if( write_singular_output(std::make_pair((inputList->m[4]).Typ(), static_cast<void*> ((inputList->m[4]).Data())), &force_continue, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading force_continue in init!"); }


  // check if a previous computation with the same inout used the same directory and, if so, continue from there, else abort.
  // see if there is a checksum file, if so compare to input:
  std::hash<std::string> string_hash_fct;
  std::string degBounds_string = (std::string) lString((lists) ((inputList->m[1]).data), true, 1);
  size_t degBounds_checksum = string_hash_fct (degBounds_string);
  //long degBounds_checksum_long = static_cast<long int>(degBounds_checksum % static_cast<size_t>(LONG_MAX));

  std::string poly_str="";
  for(int i=0; i<F->ncols; i++)
  {
    std::string poly_string = std::to_string(i) + ": " + ((std::string) p_String(F->m[i], currRing, currRing));
    poly_str = poly_str + "," + std::to_string(string_hash_fct (poly_string));
  }
  leftv cur_ring = (leftv) omAlloc0Bin(sleftv_bin); cur_ring->rtyp=LIST_CMD; cur_ring->data=currRing;
  leftv res      = (leftv) omAlloc0Bin(sleftv_bin); res->rtyp=LIST_CMD;
  jjRINGLIST(res,cur_ring);
  lists rlist = (lists) res->data;
  std::string rlist_str     = (std::string) lString(rlist, true, 1);
  omFreeBin (cur_ring, sleftv_bin);
  omFreeBin (res, sleftv_bin);
  std::size_t poly_checksum = string_hash_fct (poly_str + rlist_str);
  //long poly_checksum_long = static_cast<long int>(poly_checksum % static_cast<size_t>(LONG_MAX));

  #ifdef DEBUG_BBA
  std::cout << "checksum poly     : " << poly_checksum      << std::endl;
  std::cout << "checksum degBounds: " << degBounds_checksum << std::endl;
  std::cout << "force_continue    : " << force_continue     << std::endl;
  #endif


  if (force_continue==2) // overwrite files from previous computation(s)
  {
    int nfiles=0;
    // delete everything and continue as if gspc_buchberger is called for the first time
    std::remove((base_filename+"checksum").c_str());
    std::remove((base_filename+"LOG.txt").c_str());
    for (int i=1; ; i++)
    {
      std::ifstream fFileIn(base_filename+"intermediate_files/f"+std::to_string(i));
      if (fFileIn.good())
      {
        std::remove((base_filename+"intermediate_files/f"+std::to_string(i)).c_str());
      }
      else
      {
        std::remove((base_filename+"intermediate_files/f"+std::to_string(i)+"_incomplete").c_str());
        nfiles = i;
        break;
      }
    }
    for (int i=1; i<=nfiles; i++)
    {
      std::remove((base_filename+"result/g"+std::to_string(i)).c_str());
      std::remove((base_filename+"result/g"+std::to_string(i)+"_incomplete").c_str());
      for(int j=i+1; j<=nfiles; j++)
      {
        std::remove((base_filename+"queue/started/"          +std::to_string(i)+"_"+std::to_string(j)).c_str());
        std::remove((base_filename+"queue/zero_reduction/"   +std::to_string(i)+"_"+std::to_string(j)).c_str());
        std::remove((base_filename+"queue/new_element/"      +std::to_string(i)+"_"+std::to_string(j)).c_str());
        std::remove((base_filename+"queue/chain_criterion/"  +std::to_string(i)+"_"+std::to_string(j)).c_str());
        std::remove((base_filename+"queue/product_criterion/"+std::to_string(i)+"_"+std::to_string(j)).c_str());
        std::remove((base_filename+"queue/cancelled_s_pair/" +std::to_string(i)+"_"+std::to_string(j)).c_str());
        std::remove((base_filename+"queue/cancelled_element/"+std::to_string(i)+"_"+std::to_string(j)).c_str());
        std::remove((base_filename+"queue/degree_bound/"     +std::to_string(i)+"_"+std::to_string(j)).c_str());
        std::remove((base_filename+"temporary_files/intermediate_result_"+std::to_string(i)+"_"+std::to_string(j)).c_str());
        std::remove((base_filename+"temporary_files/intermediate_result_"+std::to_string(i)+"_"+std::to_string(j)+"_incomplete").c_str());
      }
    }
    (*prev_r)=0;
  }
  else
  {
    for (int i=1; ; i++)
    {
      std::ifstream fFileIn(base_filename+"intermediate_files/f"+std::to_string(i));
      if (!fFileIn.good())
      {
        (*prev_r) = i-1;
        break;
      }
    }
  }

  // compare to previous input using a checksum and throw error if they do not agree
  std::ifstream csFileIn(base_filename+"checksum");
  if (csFileIn.good() && force_continue!=2) // file exists
  {
    std::string oldLine1, oldLine2;
    std::getline(csFileIn, oldLine1);
    std::getline(csFileIn, oldLine2);
    csFileIn.close();

    if (oldLine1!=std::to_string(poly_checksum))
    {
      if(prev_queue_had_started) {std::remove((base_filename+"queue/NOT_STARTED").c_str());}
      throw std::runtime_error (ids + ": Directory contains files from a previous computation with different input ideal!");
    }
    if (oldLine2!=std::to_string(degBounds_checksum))
    {
      if (force_continue==1) // continue even with different degree bound
        {
          std::ofstream csFile(base_filename+"checksum",std::ios::trunc);
          csFile << poly_checksum << '\n';
          csFile << degBounds_checksum;
          csFile.close();
        }
      else
        {
          if(prev_queue_had_started) {std::remove((base_filename+"queue/NOT_STARTED").c_str());}
          throw std::runtime_error (ids + ": Directory contains files from a previous computation with different degree bound(s)!");
        }
    }
  }
  else
  {
    csFileIn.close();
    std::ofstream csFile(base_filename+"checksum",std::ios::trunc);
    csFile << poly_checksum << '\n';
    csFile << degBounds_checksum;
    csFile.close();
  }

  if (force_continue!=2) // remove previous incomplete files from previous computation(s)
  {
    int nfiles = (*prev_r) + 1;
    for (int i=1; i<=nfiles; i++)
    {
      std::remove((base_filename+"result/g"+std::to_string(i)+"_incomplete").c_str());
      for(int j=i+1; j<=nfiles; j++)
      {
        std::remove((base_filename+"temporary_files/intermediate_result_"+std::to_string(i)+"_"+std::to_string(j)+"_incomplete").c_str());
      }
    }
  }

  // From now on we can assume, that the files f1,f2,..., f{prev_r} exist from a previous computation.

  if (!prev_queue_had_started || (*prev_r)==0)
  {
    if (TEST_OPT_INTSTRATEGY) {
      for(int i=0; i<F->ncols; i++)
      {
        //!!F->m[i] = p_Cleardenom(F->m[i], currRing);
        number c;
        p_Cleardenom_n(F->m[i], currRing, c);
        n_Delete(&c, currRing->cf);
      }
    }
    else
    {
      for(int i=0; i<F->ncols; i++)
      {
        p_Norm(F->m[i], currRing);
      }
    }
  }

  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "reading input ideal in init"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  ideal F_sorted = idInit(IDELEMS(F),F->rank);
  ideal FF = idInit(1,1);
  if ((*prev_r)>0 && prev_queue_had_started) // read in from files f1,f2,...
  {
    idInsertPolyOnPos(FF,readPolySSI(base_filename+"intermediate_files/f1",false),0); // insert first polynomial of F

    for(int i=1; i<(*prev_r); i++)
    {
      FF->rank = id_RankFreeModule(FF, currRing, currRing);
      if (FF->rank==0) {FF->rank=1;}

      // take next polynomial...
      poly new_f = readPolySSI(base_filename+"intermediate_files/f"+std::to_string(i+1),false);

      idInsertPolyOnPos(FF,new_f,i);
    }
  }
  else // build f1,f2,... from input ideal
  {
    intvec *sort   = idSort(F);
    for (int i=0; i<sort->length();++i)
      {F_sorted->m[i] = F->m[(*sort)[i]-1];} //!!
      //{F_sorted->m[i] = F->m[i];}
    delete sort;

    idInsertPolyOnPos(FF,p_Copy(F_sorted->m[0], currRing),0); // insert first polynomial of F
    writePolySSI(F_sorted->m[0], base_filename+"intermediate_files/f1");

    for(int i=1; i<F->ncols; i++)
    {
      FF->rank = id_RankFreeModule(FF, currRing, currRing);
      if (FF->rank==0) {FF->rank=1;}

      // take next polynomial...
      poly new_f = F_sorted->m[i];
      if (USE_KNF)
      {
        if (TEST_OPT_INTSTRATEGY) {new_f = kNF(FF,currRing->qideal,new_f,0,4);}
        else                      {new_f = kNF(FF,currRing->qideal,new_f);}
      }
      else
      {
        kStrategy strat=new skStrategy;
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

        delete(strat);
      }

      if (TEST_OPT_INTSTRATEGY) {
        //!!FF->m[i] = p_Cleardenom(FF->m[i], currRing);
        number c;
        p_Cleardenom_n(new_f, currRing, c);
        n_Delete(&c, currRing->cf);
      }
      else
      {
        p_Norm(new_f, currRing);
      }

      writePolySSI(new_f, base_filename+"intermediate_files/f"+std::to_string(i+1));
      idInsertPolyOnPos(FF,new_f,i);
    }
  }


/*
  if (TEST_OPT_INTSTRATEGY) {
    for(int i=0; i<FF->ncols; i++)
    {
      //!!FF->m[i] = p_Cleardenom(FF->m[i], currRing);
      number c;
      p_Cleardenom_n(FF->m[i], currRing, c);
      n_Delete(&c, currRing->cf);
    }
  }
  else
  {
    for(int i=0; i<FF->ncols; i++)
    {
      p_Norm(FF->m[i], currRing);
    }
  }
*/

  //building Mvec
  //std::vector<std::vector<int>> Mvec;
  for (int i=0; i<FF->ncols; i++)
  {
    poly first  = FF->m[i];     // first term
    poly second = first->next;  // second term

    std::vector<int> Mjvec;
    std::vector<int> Mjvec2;
    std::vector<int> Mjvec_extra;

    for (int j=1; j<=currRing->N; j++)
    {
      Mjvec.emplace_back(p_GetExp(first, j, currRing));
    }
    Mjvec.emplace_back(p_GetComp(first,currRing)); // last entry = component

    if(second==NULL) {
      for (int j=1; j<=currRing->N; j++)
        {Mjvec2.emplace_back(0);}
      Mjvec2.emplace_back(-1); // last entry = component
    }
    else  {
      for (int j=1; j<=currRing->N; j++)
        {Mjvec2.emplace_back(p_GetExp(second, j, currRing));}
      Mjvec2.emplace_back(p_GetComp(second,currRing)); // last entry = component
    }

    int len=0;
    Mjvec_extra.emplace_back((int) currRing->pLDeg(first, &len, currRing)); // degree
    Mjvec_extra.emplace_back((int) len);                                    // length
    //Mjvec_extra.emplace_back(...);                                        // ...
    //...
    std::vector<std::vector<int>> Mjvec_entry = {Mjvec, Mjvec2, Mjvec_extra};
    (*Mvec).emplace_back(Mjvec_entry);
  }

  id_Delete(&F, currRing);

  //id_Delete(&F_sorted, currRing);
  long elems = (long) F_sorted->nrows * (long) F_sorted->ncols;
  if (elems>0) {omFreeSize((ADDRESS) (F_sorted->m),sizeof(poly)*elems);}
  omFreeBin((ADDRESS) F_sorted, sip_sideal_bin);

  id_Delete(&FF, currRing);

  omUpdateInfo();
  long max_mem = om_Info.MaxBytesSystem / 1024;
  (*runtime)[(std::string) "memory used in NF_of_spoly"] = GpiList({-1L, -1L, -1L, max_mem});
  long current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[ids] = GpiList({-1L, -1L, current_time, max_mem});

  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "saving initial ideal elements in init"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});
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
                                    GpiList* finished_indices,
                                    GpiList* NF)
{
  #ifdef DEBUG_BBA
  std::cout << "current Qback: ("<<Qback_i<<","<<Qback_j<<")"<< std::endl;
  #endif

	std::string ids = worker();
  std::string save_filename = base_filename+"temporary_files/intermediate_result_"+std::to_string(index_i)+"_"+std::to_string(index_j);

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
    std::rename(save_filename.c_str(), (base_filename + "intermediate_files/f"+std::to_string(r+1)).c_str()); // NF_spoly can't be reduced further, so just rename its file to the new generator f{r+1}
    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "reading partially reduced poly in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

    GpiList m;
    GpiList m2;
    GpiList m_extra;
    int n = currRing->N; // number of variables
    for(int k=1; k<=n; k++)
    {
      m.emplace_back((int) p_GetExp(NF_spoly,k,currRing));
    }
    m.emplace_back((int) p_GetComp(NF_spoly, currRing));

    poly NF_spoly_second = NF_spoly->next;
    if(NF_spoly_second==NULL) {
      for(int k=1; k<=n; k++)
      {
        m2.emplace_back((int) 0);
      }
      m2.emplace_back((int) -1);
    }
    else {
      for(int k=1; k<=n; k++)
      {
        m2.emplace_back((int) p_GetExp(NF_spoly_second,k,currRing));
      }
      m2.emplace_back((int) p_GetComp(NF_spoly_second, currRing));
    }

    int len;
    m_extra.emplace_back((int) currRing->pLDeg(NF_spoly, &len, currRing));
    m_extra.emplace_back((int) len);


    (*NF).emplace_back(GpiList({index_i, index_j, GpiList({m,m2,m_extra,r})}));

    p_Delete(&NF_spoly, currRing);

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
    /*
    std::cout << "CHECKPOINT1 0" << std::endl;
    std::cout << "CHECKPOINT1 0a (" << index_i << "," << index_j << ")" <<  std::endl;
    std::cout << p_String(F->m[index_i-1], currRing, currRing) << std::endl;
    std::cout << p_String(F->m[index_j-1], currRing, currRing) << std::endl;
    */
    LObject Pair;
    Pair.Init();
    Pair.p1=F->m[index_i-1];
    Pair.p2=F->m[index_j-1];
    ksCreateSpoly(&Pair);

    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    if (USE_KNF)
    {
      /*
      std::cout << p_String(Pair.p1, currRing, currRing) << std::endl;
      std::cout << p_String(F->m[index_i-1], currRing, currRing) << std::endl;
      std::cout << p_String(Pair.p2, currRing, currRing) << std::endl;
      std::cout << p_String(F->m[index_j-1], currRing, currRing) << std::endl;
      */
      #ifdef DEBUG_BBA
      std::cout << "spoly("<<index_i<<","<<index_j<<"):" << std::endl;
      std::cout << p_String(Pair.p, currRing, currRing) << std::endl;
      #endif
      if (TEST_OPT_INTSTRATEGY) {NF_spoly = kNF(F,currRing->qideal,Pair.p,0,4);}
      else                      {NF_spoly = kNF(F,currRing->qideal,Pair.p);}

      #ifdef DEBUG_BBA
      std::cout << "NF(spoly("<<index_i<<","<<index_j<<"),G_"<<r<<"):" << std::endl;
      //std::cout << "RESULT OF NFSPOLY (may be the new " << r+1 << "-th element)" << std::endl;
      std::cout << p_String(NF_spoly, currRing, currRing) << std::endl;
      #endif
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
      delete(strat);
    }

    Pair.Delete();

    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "applying NF in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});
    }
  else
  {
    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    poly prev_result = readPolySSI(save_filename, true);
    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "reading partially reduced poly in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    if (USE_KNF)
    {
      if (TEST_OPT_INTSTRATEGY) {NF_spoly = kNF(F,currRing->qideal,prev_result,0,4);}
      else                      {NF_spoly = kNF(F,currRing->qideal,prev_result);}
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
      NF_spoly = redNF(prev_result,sl,TRUE,strat);
      //long stop_time_redNF = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      //(*runtime)[(std::string) "just the call to redNF(..) in NF_of_spoly"] = GpiList({-1L, stop_time_redNF, stop_time_redNF-start_time_redNF, 1L});
      delete(strat);
    }

    p_Delete(&prev_result, currRing);

    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "applying NF in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});
  }

  long elems = (long) F->nrows * (long) F->ncols;
  if (elems>0) {omFreeSize((ADDRESS) (F->m),sizeof(poly)*elems);}
  omFreeBin((ADDRESS) F, sip_sideal_bin);

  if (NF_spoly==NULL) // reduction to 0 ==> pair is finished (and to be removed from Q)
  {
    (*finished_indices).emplace_back(index_i);
    (*finished_indices).emplace_back(index_j);
  }
  else // did NOT reduce to 0
  {
    /*
    if (index_i==Qback_i && index_j==Qback_j) // element at end of Q ==> add as new GB element
    {
    */

      // do this later, in update_Q by renaming a file!
      //writePolySSI(NF_spoly, base_filename + "intermediate_files/f"+std::to_string(r+1));

      GpiList m;
      GpiList m2;
      GpiList m_extra;
      int n = currRing->N; // number of variables
      for(int k=1; k<=n; k++)
      {
        m.emplace_back((int) p_GetExp(NF_spoly,k,currRing));
      }
      m.emplace_back((int) p_GetComp(NF_spoly, currRing));

      poly NF_spoly_second = NF_spoly->next;
      if(NF_spoly_second==NULL) {
        for(int k=1; k<=n; k++)
          {m2.emplace_back((int) 0);}
        m2.emplace_back((int) -1);
      }
      else {
        for(int k=1; k<=n; k++)
          {m2.emplace_back((int) p_GetExp(NF_spoly_second,k,currRing));}
        m2.emplace_back((int) p_GetComp(NF_spoly_second, currRing));
      }

      int len;
      m_extra.emplace_back((int) currRing->pLDeg(NF_spoly, &len, currRing));
      m_extra.emplace_back((int) len);


      (*NF).emplace_back(GpiList({index_i, index_j, GpiList({m,m2,m_extra,r})}));
    /*
    }
    else // element not at end of Q ==> put back to started indices (to be reduced further in future)
    {
    */
      start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      writePolySSI(NF_spoly, save_filename);
      stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
      (*runtime)[(std::string) "saving partially reduced poly in NF_of_spoly"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});

      //(*started_indices_out).emplace_back(index_i);
      //(*started_indices_out).emplace_back(index_j);
    /*
    }
    */

    p_Delete(&NF_spoly, currRing);
  }

  omUpdateInfo();
  long max_mem = om_Info.MaxBytesSystem / 1024;
  (*runtime)[(std::string) "memory used in NF_of_spoly"] = GpiList({-1L, -1L, -1L, max_mem});
  long current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[ids] = GpiList({-1L, -1L, current_time, max_mem});

  #ifdef DEBUG_BBA
  std::cout << "memory(2): " << max_mem << std::endl;
  #endif
}


NO_NAME_MANGLING
poly read_generator (std::string const& base_filename,
                     int k)
{
  init_singular (config::singularLibrary().string());
  return readPolySSI(base_filename+"intermediate_files/f"+std::to_string(k),false);
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

  poly f; // poly to be reduced and saved in a file

  long start_time,stop_time;

  if (redSB)
  {
    ideal F = idInit(final_r-1,1);
    std::list<poly>::const_iterator gen = generators.begin();
    int ii=0;
    for(int i=0; i<final_r; i++)
    {
      if(i==current_index)
      {
        //!!f = p_Copy(*gen, currRing, currRing);
        f = *gen;
      }
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
    if (USE_KNF)
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
      delete(strat);
    }

    //id_Delete(&F, currRing);
    long elems = (long) F->nrows * (long) F->ncols;
    if (elems>0) {omFreeSize((ADDRESS) (F->m),sizeof(poly)*elems);}
    omFreeBin((ADDRESS) F, sip_sideal_bin);

    stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "applying NF in reduce_GB"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});
  }
  else
  {
    f = readPolySSI(base_filename+"intermediate_files/f"+std::to_string(needed_indices),false);
  }

  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  if (TEST_OPT_INTSTRATEGY)
  {
    //!!f = p_Cleardenom(f, currRing);
    number c;
    p_Cleardenom_n(f, currRing, c);
    n_Delete(&c, currRing->cf);
  }
  else
  {
    p_Norm(f, currRing);
  }
  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "clearing denominators in reduce_GB"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});


  start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  writePolySSI(f, base_filename + "result/g" + std::to_string(current_index+1));
  if (redSB)
  {
    p_Delete(&f, currRing);
  }
  stop_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "saving GB in files in reduce_GB"] = GpiList({-1L, stop_time, stop_time-start_time, 1L});
}
