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
                   long* nworkers,
                   GpiList* degBounds,
                   long* target_time,
                   long* max_batch_size,
                   double* head_size_factor,
                   std::pair<unsigned int,unsigned int>* si_opt,
                   int* prev_r,
                   long* syz_comp,
                   long* red_syz,
                   int* rank,
                   GpiMap* runtime)
{
  std::string ids = worker();
  init_singular (config::singularLibrary().string());

  double start_time,stop_time;
  start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  long force_continue;

  // read ideal from input
  std::pair<int,void*> input_token = deserialize(input, ids, true);
  lists inputList = (lists) (((lists) input_token.second)->m[3]).data;
  ideal F = (ideal) inputList->m[0].data;
  // read nworkers
  if( write_singular_output(std::make_pair((inputList->m[1]).Typ(), static_cast<void*> ((inputList->m[1]).Data())), nworkers, base_filename, "init") )
  { throw std::runtime_error (ids + ": error reading nworkers in init!"); }
  // read degree bound(s)
  if( write_singular_output(std::make_pair((inputList->m[2]).Typ(), static_cast<void*> ((inputList->m[2]).Data())), degBounds, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading degree bounds list in init!"); }
  // read force_continue
  if( write_singular_output(std::make_pair((inputList->m[3]).Typ(), static_cast<void*> ((inputList->m[3]).Data())), &force_continue, base_filename, "init") )
  { throw std::runtime_error (ids + ": error reading force_continue in init!"); }
  // read target_time
  if( write_singular_output(std::make_pair((inputList->m[4]).Typ(), static_cast<void*> ((inputList->m[4]).Data())), target_time, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading target_time in init!"); }
  // read max_batch_size
  if( write_singular_output(std::make_pair((inputList->m[5]).Typ(), static_cast<void*> ((inputList->m[5]).Data())), max_batch_size, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading max_batch_size in init!"); }
  // read head_size_factor
  long head_size_factor_long;
  if( write_singular_output(std::make_pair((inputList->m[6]).Typ(), static_cast<void*> ((inputList->m[6]).Data())), &head_size_factor_long, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading head_size_factor in init!"); }
  (*head_size_factor) = static_cast<double>(head_size_factor_long)/100.0;
  if( write_singular_output(std::make_pair((inputList->m[7]).Typ(), static_cast<void*> ((inputList->m[7]).Data())), syz_comp, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading syz_comp in init!"); }
  if( write_singular_output(std::make_pair((inputList->m[8]).Typ(), static_cast<void*> ((inputList->m[8]).Data())), red_syz, base_filename, "init") )
    { throw std::runtime_error (ids + ": error reading red_syz in init!"); }


  if(*red_syz<0)
    *red_syz=0;
  if(*red_syz>2)
    *red_syz=2;
  if(*syz_comp<0)
    *syz_comp=0;

  if(*syz_comp>0) {
    //k=si_max(id_RankFreeModule(mod,currRing),id_RankFreeModule(submod,currRing));
    //k=si_max(k,(int)mod->rank);



    ring orig_ring=currRing;
    writeRingSSI(orig_ring, base_filename + "basering");

    ring syz_ring=rAssure_SyzOrder(orig_ring,TRUE);
    rSetSyzComp(*syz_comp,syz_ring);

    if (orig_ring != syz_ring) {
      rChangeCurrRing(syz_ring);
      ideal F_tmp = F;
      //F = idrCopyR_NoSort(F,orig_ring,syz_ring);
      F = idrCopyR(F,orig_ring,syz_ring);
      id_Delete(&F_tmp, orig_ring);
      rDelete(orig_ring);
    }
/*
    if (TEST_OPT_RETURN_SB || *red_syz) {
      *syz_comp *= -1;
      // Deactivate syzComp so the computation will be done in the changed ring,
      // but syzygies will be treated like normal GB elements. Thus these syzygy
      // elements will be a GB of the syzygy module and the other elements form
      // a GB of the input ideal/module.
      // Same if only red_syz is set (though then the syzygies will ONLY be
      // used as reducers, so the syzygies will be interreduced but no GB).
    }
*/
  }


  if(*syz_comp>0) {*rank = (int) *syz_comp;}
  else            {*rank = (int) F->nrows;}


  // SINGULAR options (result of 'option(get);')
  si_opt->first  = si_opt_1;
  si_opt->second = si_opt_2;

  // check if a previous computation with the same inout used the same directory and, if so, continue from there, else abort.
  // see if there is a checksum file, if so compare to input:
  std::hash<std::string> string_hash_fct;
  std::string degBounds_string = (std::string) lString((lists) ((inputList->m[2]).data), true, 1);
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

  if (force_continue!=2) // remove incomplete files from previous computation(s)
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

  stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "reading input ideal in init"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});

  start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
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
      //std::cout<<"-----> 1 <-----"<<std::endl;
      //std::cout<<"-----> 2 <-----"<<std::endl;

      #ifdef OLD_REDTAIL
      new_f = kNF(FF, currRing->qideal, new_f, *red_syz==0 ? *syz_comp : 0, 4*TEST_OPT_INTSTRATEGY+(1-TEST_OPT_REDTAIL));
      #else
      new_f = kNF(FF, currRing->qideal, new_f, *red_syz==0 ? *syz_comp : 0, 4*TEST_OPT_INTSTRATEGY);
      #endif

      //std::cout<<"-----> 3 <-----"<<std::endl;


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
  //std::cout<<"-----> 4 <-----"<<std::endl;
  for (int i=0; i<FF->ncols; i++)
  {
    std::vector<int> Mjvec;
    std::vector<int> Mjvec2;
    std::vector<int> Mjvec_extra;

    //std::cout<<"-----> 5 <-----"<<std::endl;
    poly first  = FF->m[i];     // first term
    //while (p_GetComp(first, currRing) > *syz_comp)
    //  pIter(first);

    for (int j=1; j<=currRing->N; j++)
    {
      Mjvec.emplace_back(p_GetExp(first, j, currRing));
    }
    Mjvec.emplace_back(p_GetComp(first, currRing)); // last entry = component


    //std::cout<<"-----> 6 <-----"<<std::endl;
    poly second = first->next;  // second term
    //std::cout<<"-----> 6.1 <-----"<<std::endl;
    //while (second!=NULL && p_GetComp(second, currRing) > *syz_comp) {
    //  //std::cout<<"-----> 6.2 <-----"<<std::endl;
    //  pIter(second);
    //}
    //std::cout<<"-----> 7 <-----"<<std::endl;
    if(second==NULL || (*red_syz<2 && isSyzygy(second, *syz_comp))) {
      for (int j=1; j<=currRing->N; j++)
        {Mjvec2.emplace_back(0);}
      Mjvec2.emplace_back(-1); // last entry = component
    }
    else  {
      for (int j=1; j<=currRing->N; j++)
        {Mjvec2.emplace_back(p_GetExp(second, j, currRing));}
      Mjvec2.emplace_back(p_GetComp(second, currRing)); // last entry = component
    }
    //std::cout<<"-----> 8 <-----"<<std::endl;

    int len=0;
    Mjvec_extra.emplace_back((int) currRing->pLDeg(first, &len, currRing)); // degree     // degree will only be correct for a degree ordering !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    Mjvec_extra.emplace_back((int) len);                                    // length
    //Mjvec_extra.emplace_back(...);                                        // ...
    //...
    std::vector<std::vector<int>> Mjvec_entry = {Mjvec, Mjvec2, Mjvec_extra};
    (*Mvec).emplace_back(Mjvec_entry);
  }
  //std::cout<<"-----> 9 <-----"<<std::endl;

  id_Delete(&F, currRing);

  //id_Delete(&F_sorted, currRing);
  long elems = (long) F_sorted->nrows * (long) F_sorted->ncols;
  if (elems>0) {omFreeSize((ADDRESS) (F_sorted->m),sizeof(poly)*elems);}
  omFreeBin((ADDRESS) F_sorted, sip_sideal_bin);

  id_Delete(&FF, currRing);

  omUpdateInfo();
  long max_mem = om_Info.MaxBytesSystem / 1024;
  (*runtime)[(std::string) "memory used in NF_of_spoly"] = GpiList({-1.0, -1.0, -1.0, max_mem});
  double current_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[ids] = GpiList({-1.0, -1.0, current_time, max_mem});

  stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "saving initial ideal elements in init"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});
}


NO_NAME_MANGLING
void singular_buchberger_compute_NF(std::string const& base_filename,
                                    std::list<poly> const& generators,
                                    int r,
                                    int index_i,
                                    int index_j,
                                    int old_r,
                                    int syzygy,
                   [[maybe_unused]] GpiList const& M,
                                    long syz_comp,
                                    long red_syz,
                                    GpiMap* runtime,
                                    GpiList* NF)
{

	std::string ids = worker();
  std::string save_filename = base_filename+"temporary_files/intermediate_result_"+std::to_string(index_i)+"_"+std::to_string(index_j);


	//// start Singular ////
	init_singular (config::singularLibrary().string());
  double start_time,stop_time;


  //// init ideal F of reducers ////
  start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  ideal F = idInit(r,1);
  std::list<poly>::const_iterator gen = generators.begin();
  for(int ii=1; ii<=r; ii++)
  {
    idInsertPolyOnPos(F, *gen, ii-1); ++gen;
  }
  F->rank = id_RankFreeModule(F, currRing, currRing);
  if (F->rank==0) F->rank=1;
  stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "reading generators in NF_of_spoly"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});

  //// compute NF(spoly(F[i],F[j]), F) ////
  poly NF_spoly;
  if (old_r == 0) // new reduction
  {
    poly spoly;
    LObject Pair;
    //std::cout<<"----- NF_spoly -----> 1 <-----"<<std::endl;
    //if(TEST_syz_comp_ARG==0) {
    Pair.Init();
    Pair.p1=F->m[index_i-1];
    Pair.p2=F->m[index_j-1];
    start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    if (syzygy<2) { // 0: spair of GB elements, 1: spair of syzygies, 2: product criterion pair
      ksCreateSpoly(&Pair);
      spoly = Pair.p;
      /*
      }
      else
      {
        // get lead exponents
        GpiList::const_iterator itM = std::next(M.begin(), index_i-1);
        std::vector<int> m1 = list2vec(get_list(get_list(*itM).front()));
        itM = std::next(itM, index_j-index_i);
        std::vector<int> m2 = list2vec(get_list(get_list(*itM).front()));
        std::vector<int>::const_iterator it1 = m1.begin();
        std::vector<int>::const_iterator it2 = m2.begin();
        std::vector<int> a1,a2;
        int nvars = m1.size()-1;
        //std::cout<<"----- NF_spoly -----> 2 <-----"<<std::endl;
        for(int k=0; k<nvars; ++it1, ++it2, k++)
        {
          a1.emplace_back(std::max(0,*it2-*it1));
          a2.emplace_back(std::max(0,*it1-*it2));
        }
        poly p1 = p_Copy(F->m[index_i-1], currRing);
        poly p2 = p_Copy(F->m[index_j-1], currRing);
        //std::cout<<"----- NF_spoly -----> 3 <-----"<<std::endl;
        // so spoly = p1*a1 - p2*a2  (interpreting a1,a2 as monomials)
        poly p1_term=p1;
        poly p2_term=p2;
        for (; p1_term!=NULL; pIter(p1_term)) {
          for (int k=0; k<nvars; k++)
          p1_term->exp[k] += a1[k];
        }
        for (; p2_term!=NULL; pIter(p2_term)) {
          for (int k=0; k<nvars; k++)
          p2_term->exp[k] += a2[k];
        }
        //std::cout<<"----- NF_spoly -----> 4 <-----"<<std::endl;
        spoly = p_Add_q(p1, p_Neg(p2, currRing), currRing);
      }
      */
      //std::cout<<p_String(spoly, currRing, currRing)<<std::endl;
      //std::cout<<"----- NF_spoly -----> 5 <-----"<<std::endl;

      #ifdef DEBUG_BBA
      std::cout << "spoly("<<index_i<<","<<index_j<<"):" << std::endl;
      //std::cout << p_String(Pair.p, currRing, currRing) << std::endl;
      std::cout << p_String(spoly, currRing, currRing) << std::endl;
      #endif
      //if (TEST_OPT_INTSTRATEGY) {NF_spoly = kNF(F,currRing->qideal,Pair.p,syz_comp,4);}
      //else                      {NF_spoly = kNF(F,currRing->qideal,Pair.p,syz_comp);}
      //std::cout<<"----- NF_spoly -----> 6 <-----"<<std::endl;

      #ifdef OLD_REDTAIL
      NF_spoly = kNF(F, currRing->qideal, spoly, red_syz==0 ? syz_comp : 0, 4*TEST_OPT_INTSTRATEGY+(1-TEST_OPT_REDTAIL));
      #else
      NF_spoly = kNF(F, currRing->qideal, spoly, red_syz==0 ? syz_comp : 0, 4*TEST_OPT_INTSTRATEGY);
      #endif

      //std::cout<<"----- NF_spoly -----> 7 <-----"<<std::endl;

      #ifdef DEBUG_BBA
      std::cout << "NF(spoly("<<index_i<<","<<index_j<<"),G_"<<r<<"):" << std::endl;
      //std::cout << "RESULT OF NFSPOLY (may be the new " << r+1 << "-th element)" << std::endl;
      std::cout << p_String(NF_spoly, currRing, currRing) << std::endl;
      #endif
    }
    else { // directly construct spoly from product criterion
      poly last;

      poly p1_poly=p_Copy(Pair.p1, currRing);
      poly p1_lift = p1_poly;
      while (p1_lift!=NULL && p_GetComp(p1_lift, currRing)==1) {
        __p_GetComp(p1_lift,currRing) = 0;
        last = p1_lift;
        p1_lift = pNext(p1_lift);
      }
      pNext(last) = NULL;

      poly p2_poly=p_Copy(Pair.p2, currRing);
      poly p2_lift = p2_poly;
      while (p2_lift!=NULL && p_GetComp(p2_lift, currRing)==1) {
        __p_GetComp(p2_lift,currRing) = 0;
        last = p2_lift;
        p2_lift = pNext(p2_lift);
      }
      pNext(last) = NULL;

      spoly = p_Sub( p_Mult_q(p1_poly, p2_lift, currRing), p_Mult_q(p2_poly, p1_lift, currRing), currRing);

      #ifdef OLD_REDTAIL
      NF_spoly = kNF(F, currRing->qideal, spoly, red_syz==0 ? syz_comp : 0, 4*TEST_OPT_INTSTRATEGY+(1-TEST_OPT_REDTAIL));
      #else
      NF_spoly = kNF(F, currRing->qideal, spoly, red_syz==0 ? syz_comp : 0, 4*TEST_OPT_INTSTRATEGY);
      #endif

      /*
      if (TEST_OPT_INTSTRATEGY)
      {
        //!!f = p_Cleardenom(f, currRing);
        number c;
        p_Cleardenom_n(NF_spoly, currRing, c);
        n_Delete(&c, currRing->cf);
      }
      else
      {
        p_Norm(NF_spoly, currRing);
      }
      */
    }

    //std::cout<<"----- NF_spoly -----> 8 <-----"<<std::endl;

    Pair.Delete();

    stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "applying NF in NF_of_spoly"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});
  }
  else // continue a previous reduction (now with more reducers)
  {
    //std::cout<<"----- re-reduction -----> 1 <-----"<<std::endl;
    start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    poly prev_result = readPolySSI(save_filename, true);
    stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "reading partially reduced poly in NF_of_spoly"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});

    //std::cout<<"----- re-reduction -----> 2 <-----"<<std::endl;
    start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    //std::cout<<"----- re-reduction -----> 3 <-----"<<std::endl;
    
    #ifdef OLD_REDTAIL
    NF_spoly = kNF(F, currRing->qideal, prev_result, red_syz==0 ? syz_comp : 0, 4*TEST_OPT_INTSTRATEGY+(1-TEST_OPT_REDTAIL));
    #else
    NF_spoly = kNF(F, currRing->qideal, prev_result, red_syz==0 ? syz_comp : 0, 4*TEST_OPT_INTSTRATEGY);
    #endif

    //std::cout<<"----- re-reduction -----> 4 <-----"<<std::endl;

    //std::cout<<"----- re-reduction -----> 5 <-----"<<std::endl;

    p_Delete(&prev_result, currRing);
    //std::cout<<"----- re-reduction -----> 6-8 <-----"<<std::endl;

    stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "applying NF in NF_of_spoly"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});
  }
  //std::cout<<"----- NF_spoly -----> 9 <-----"<<std::endl;


  // normalizing to lead coeff 1 or content 1 depending on option(intStrategy)
  if (NF_spoly!=NULL)
  {
    if (TEST_OPT_INTSTRATEGY)
    {
      //!!f = p_Cleardenom(f, currRing);
      number c;
      p_Cleardenom_n(NF_spoly, currRing, c);
      n_Delete(&c, currRing->cf);
    }
    else
    {
      p_Norm(NF_spoly, currRing);
    }
  }


  long elems = (long) F->nrows * (long) F->ncols;
  if (elems>0) {omFreeSize((ADDRESS) (F->m),sizeof(poly)*elems);}
  omFreeBin((ADDRESS) F, sip_sideal_bin);

  //std::cout<<"----- NF_spoly -----> 10 <-----"<<std::endl;

  //// save result ////
  bool is_syz = red_syz==0 && isSyzygy(NF_spoly, syz_comp);
  //std::cout<<"----- NF_spoly -----> 10.01 <-----"<<std::endl;
  if (is_syz) // 0-reduction and non-trivial syzygy must be saved (if red_syz>0 this will be done in a later step)
  {
    //std::cout<<"----- NF_spoly -----> 10.02 <-----"<<std::endl;
    //writePolySSI(NF_spoly, save_filename);
    //std::cout<<"----- NF_spoly -----> 10.03 <-----"<<std::endl;

    ring syz_ring = currRing;
    ring orig_ring = readRingSSI(base_filename + "basering", false);
    //std::cout<<"----- NF_spoly -----> 10.04 <-----"<<std::endl;

    pSubtractComp(NF_spoly,syz_comp);

    //std::cout<<"----- NF_spoly -----> 10.05 <-----"<<std::endl;
    poly syzygy = NF_spoly;
    if(syz_ring!=orig_ring)
    {
      //std::cout<<"----- NF_spoly -----> 10.06 <-----"<<std::endl;
      rChangeCurrRing(orig_ring);
      syzygy = prMoveR(NF_spoly, syz_ring, orig_ring);
      //std::cout<<"----- NF_spoly -----> 10.07 <-----"<<std::endl;
      //rDelete(syz_ring);
    }
    //std::cout<<"----- NF_spoly -----> 10.08 <-----"<<std::endl;

    writePolySSI(syzygy, save_filename); // save the syzygy in the original basering
    //std::cout<<"----- NF_spoly -----> 10.09 <-----"<<std::endl;

    //change back to prevent error
    if(syz_ring!=orig_ring) {
      //if (TEST_OPT_REDSB)
      p_Delete(&syzygy, currRing);
      rChangeCurrRing(syz_ring);
      rDelete(orig_ring);
    }

  }
  //std::cout<<"----- NF_spoly -----> 10.1 <-----"<<std::endl;
  if (NF_spoly==NULL || is_syz) // 0-reduction
  {
    //std::cout<<"----- NF_spoly -----> 10.2 <-----"<<std::endl;
    GpiVariant result;
    poke("i",result, index_i);
    poke("j",result, index_j);
    poke("old_r",result, r);
    poke("lead_data", result, GpiList({}));
    NF->push_back(result);
    //std::cout<<"----- NF_spoly -----> 10.3 <-----"<<std::endl;
  }
  else // did NOT reduce to 0
  {
    //std::cout<<"----- NF_spoly -----> 10.4 <-----"<<std::endl;
    GpiList m;
    GpiList m2;
    GpiList m_extra;
    int n = currRing->N; // number of variables
    poly NF_spoly_lead = NF_spoly;
    //while (p_GetComp(NF_spoly_lead, currRing) > syz_comp)
    //  pIter(NF_spoly_lead);
    for(int k=1; k<=n; k++)
    {
      m.emplace_back((int) p_GetExp(NF_spoly_lead,k,currRing));
    }
    m.emplace_back((int) p_GetComp(NF_spoly_lead, currRing));

    //int is_syzygy = (int) (syz_comp<0 && p_GetComp(NF_spoly_lead, currRing)>-syz_comp); // element is actually a syzygy

    poly NF_spoly_second_lead = NF_spoly_lead->next;
    //while (NF_spoly_second_lead!=NULL && p_GetComp(NF_spoly_second_lead, currRing) > syz_comp)
    //  pIter(NF_spoly_second_lead);
    if(NF_spoly_second_lead==NULL || (red_syz<2 && isSyzygy(NF_spoly_second_lead,syz_comp))) {
      for(int k=1; k<=n; k++)
        {m2.emplace_back((int) 0);}
      m2.emplace_back((int) -1);
    }
    else {
      for(int k=1; k<=n; k++)
        {m2.emplace_back((int) p_GetExp(NF_spoly_second_lead,k,currRing));}
      m2.emplace_back((int) p_GetComp(NF_spoly_second_lead, currRing));
    }

    int len;
    m_extra.emplace_back((int) currRing->pLDeg(NF_spoly_lead, &len, currRing)); // degree will only be correct for a degree ordering !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    m_extra.emplace_back((int) len);
    //m_extra.emplace_back(is_syzygy);


    GpiVariant result;
    poke("i", result, index_i);
    poke("j", result, index_j);
    poke("old_r", result, r);
    poke("lead_data", result, GpiList({m,m2,m_extra}));
    NF->push_back(result);
    start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    writePolySSI(NF_spoly, save_filename);
    stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "saving partially reduced poly in NF_of_spoly"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});
    //std::cout<<"----- NF_spoly -----> 10.5 <-----"<<std::endl;
  }
  //std::cout<<"----- NF_spoly -----> 10.6 <-----"<<std::endl;

  p_Delete(&NF_spoly, currRing);

  //std::cout<<"----- NF_spoly -----> 11 <-----"<<std::endl;

  //std::cout<<"----- NF_spoly -----> 11.1 <-----"<<std::endl;
  //omUpdateInfo(); // MEMORY measurement broken
  //std::cout<<"----- NF_spoly -----> 11.2 <-----"<<std::endl;
  //long max_mem = om_Info.MaxBytesSystem / 1024; // MEMORY measurement broken
  long max_mem = 1L;
  //std::cout<<"----- NF_spoly -----> 11.3 <-----"<<std::endl;
  (*runtime)[(std::string) "memory used in NF_of_spoly"] = GpiList({-1.0, -1.0, -1.0, max_mem});
  //std::cout<<"----- NF_spoly -----> 11.4 <-----"<<std::endl;
  double current_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  //std::cout<<"----- NF_spoly -----> 11.5 <-----"<<std::endl;
  (*runtime)[ids] = GpiList({-1.0, -1.0, current_time, max_mem});

  //std::cout<<"----- NF_spoly -----> 12 <-----"<<std::endl;
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
                                    int generator_name,
                                    int generator_index,
                                    int save_index,
                                    int is_syzygy,
                                    int ngens,
                                    long syz_comp,
                                    long red_syz,
                                    GpiMap* runtime)
{
  init_singular (config::singularLibrary().string());

  poly f; // poly to be reduced and saved in a file

  double start_time,stop_time;

  if (TEST_OPT_REDSB)
  {
    ideal F = idInit(ngens-1,1);
    std::list<poly>::const_iterator gen = generators.begin();
    int ii=0;
    for(int i=0; i<ngens; i++)
    {
      if(i==generator_index)
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
    start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    f = kNF(F, currRing->qideal, f, red_syz==0 ? syz_comp : 0, 4*TEST_OPT_INTSTRATEGY);

    //id_Delete(&F, currRing);
    long elems = (long) F->nrows * (long) F->ncols;
    if (elems>0) {omFreeSize((ADDRESS) (F->m),sizeof(poly)*elems);}
    omFreeBin((ADDRESS) F, sip_sideal_bin);

    stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    (*runtime)[(std::string) "applying NF in reduce_GB"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});
  }
  else
  {
    f = readPolySSI(base_filename+"intermediate_files/f"+std::to_string(generator_name),false);
  }
  start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
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
  stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "clearing denominators in reduce_GB"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});
  
  
  ring syz_ring, orig_ring;
  if(syz_comp>0) {
    syz_ring = currRing;
    orig_ring = readRingSSI(base_filename + "basering", false);
    
    poly f_tail = f;
    poly f_last;
    while (f_tail!=NULL && p_GetComp(f_tail, currRing)<=syz_comp) {
      f_last = f_tail;
      f_tail = pNext(f_tail);
    }
    if(!is_syzygy) {
      pNext(f_last) = NULL; // use p_Split instead?
    }
    
    pSubtractComp(f_tail,syz_comp);
    
    poly f_orig = f;
    poly f_tail_orig = f_tail;
    if(syz_ring!=orig_ring)
    {
      rChangeCurrRing(orig_ring);
      if(!is_syzygy) {
        f_orig      = prMoveR(f     , syz_ring, orig_ring);
      }
      f_tail_orig = prMoveR(f_tail, syz_ring, orig_ring);
      //rDelete(syz_ring);
    }
    
    
    start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    if(is_syzygy) {
      writePolySSI(f_tail_orig, base_filename + "result/s" + std::to_string(save_index));
    }
    else {
      writePolySSI(f_orig     , base_filename + "result/g" + std::to_string(save_index));
      writePolySSI(f_tail_orig, base_filename + "result/l" + std::to_string(save_index));
    }
    
    
    //change back to prevent error
    if(syz_ring!=orig_ring) {
      //if (TEST_OPT_REDSB)
      if(!is_syzygy) {
        p_Delete(&f_orig     , currRing);
      }
      p_Delete(&f_tail_orig, currRing);
      rChangeCurrRing(syz_ring);
      rDelete(orig_ring);
    }
    if(!is_syzygy) {
      p_Delete(&f     , currRing);
    }
    p_Delete(&f_tail, currRing);
    
  }
  else {
    start_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    writePolySSI(f, base_filename + "result/g" + std::to_string(save_index));
    //if (TEST_OPT_REDSB)
    p_Delete(&f, currRing);
  }
  
  
  stop_time = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  (*runtime)[(std::string) "saving GB in files in reduce_GB"] = GpiList({-1.0, stop_time, stop_time-start_time, 1L});
}

void tail_reduce(std::list<poly>  * generators,
                 std::string const& from_filename,
                 std::string const& to_filename)
{
  init_singular (config::singularLibrary().string());

  // read from file (and delete file)
  poly f = readPolySSI(from_filename, true);

  // build reducer ideal
  int ngens = generators->size();
  ideal F = idInit(ngens,1);
  std::list<poly>::const_iterator gen = generators->begin();
  for(int i=0; i<ngens; i++)
  {
    idInsertPolyOnPos(F, *gen, i);
    ++gen;
  }
  F->rank = id_RankFreeModule(F, currRing, currRing);
  if (F->rank==0) F->rank=1;

  // (tail-)reduce
  f = kNF(F, currRing->qideal, f, 0, 4*TEST_OPT_INTSTRATEGY);

  // normalize
  if (TEST_OPT_INTSTRATEGY)
  {
    number c;
    p_Cleardenom_n(f, currRing, c);
    n_Delete(&c, currRing->cf);
  }
  else
  {
    p_Norm(f, currRing);
  }
  
  // write to file and add to update_Q's generator list
  writePolySSI(f, to_filename);
  generators->push_back(f);
}