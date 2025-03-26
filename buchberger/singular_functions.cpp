#include "singular_functions.hpp"


#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <vector>


const std::string STRUCT_NAME = "token";
const std::string STRUCT_DESC = "list fieldnames, list data";

void writePolySSI(poly P, std::string out_filename)
{
  si_link f  = ssi_open_for_write (out_filename);
  sleftv data;
  data.Init();
  if (p_GetComp(P,currRing)==0)
    data.rtyp = POLY_CMD;
  else
    data.rtyp = VECTOR_CMD;
  data.data = (void*) P;
  if (ssiWrite(f,&data))
  {
    throw std::runtime_error ("saving polynomial to ssi failed");
  }
  ssi_close_and_remove (f);
}

poly readPolySSI(std::string filename, BOOLEAN delete_file)
{
  si_link f  = ssi_open_for_read (filename);
  leftv data = ssiRead1(f);
  if (data->rtyp != POLY_CMD && data->rtyp != VECTOR_CMD)
  {
    throw std::runtime_error ("reading polynomial from ssi failed");
  }
  ssi_close_and_remove (f);
  if(delete_file) {std::remove(filename.c_str());}
  return (poly) data->data;
}


int get_struct_cmd()
{
  if (!(register_struct (STRUCT_NAME, STRUCT_DESC)))
	{
		 throw std::runtime_error ("could not register structs");
	}
  int tok;
  blackboxIsCmd (STRUCT_NAME.c_str(), tok);
  return tok;
}


void call_singular (std::string const& command)
{
  int err = iiAllStart
    (NULL, const_cast<char*> (command.c_str()), BT_proc, 0);
  if (err)
  {
    errorreported = 0;
    throw std::runtime_error ("Singular returned an error ...");
  }
}

void call_singular_and_discard (std::string const& command)
{
  SPrintStart();
  call_singular (command);
  char* result_ptr = SPrintEnd();
  omFree (result_ptr);
}

std::string filename_generator(std::string singular_function_name)
{
  char hst[65];
  uint64_t current_time;
  gethostname(hst,64);
  hst[64] = '\0';
  std::string filename (hst);
  current_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  filename = filename + "__time_" + std::to_string(current_time);
  filename = filename + "__proc_" + singular_function_name;
  filename = filename + "__worker_" +  std::to_string(getpid());
  return  filename;
}

si_link ssi_open_for_write (std::string const& ssi_filename)
{
  si_link l = static_cast<si_link> (omAlloc0Bin (sip_link_bin));
  l->name = omStrDup (ssi_filename.c_str());
  l->mode = omStrDup ("w");
  l->ref = 1;
  si_link_extension ns = static_cast<si_link_extension>
    (omAlloc0Bin (s_si_link_extension_bin));
  ns = slInitSsiExtension (ns);
  l->m = ns;

  BOOLEAN res = l->m->Open (l, SI_LINK_WRITE, NULL);
  // this already writes the parameter line "98 ..."

  if (res)
  {
    throw std::runtime_error ("could not open ssi file " + ssi_filename
                              + " for write");
  }

  return l;
}

void ssi_close_and_remove (si_link l)
{
  // it seems that PrepClose is not used (not set) for ssi links
  BOOLEAN res = l->m->Close (l); // this frees l->d
  if (res)
  {
    throw std::runtime_error ("closing ssi link failed");
  }
  omFree (static_cast<ADDRESS> (l->name));
  omFree (static_cast<ADDRESS> (l->mode));
  omFreeBin (l->m, s_si_link_extension_bin);
  omFreeBin (l, sip_link_bin);
}

void ssi_write_newstruct (si_link l, std::string const& struct_name, lists lst)
{
  int type;
  blackboxIsCmd (struct_name.c_str(), type); // TODO check for success
  ssi_write_newstruct (l, type, lst);
}

void ssi_write_newstruct (si_link l, int type, lists lst)
{
  ssiInfo *d = static_cast<ssiInfo*> (l->data);
  blackbox *b = getBlackboxStuff (type); // TODO check for success
  fputs ("20 ", d->f_write);
  newstruct_serialize (b, lst, l);
}

std::string serialize (lists lst , std::string const& base_filename, std::string singular_function_name)
{
	std::string out_filename = filename_generator (singular_function_name);
	out_filename = base_filename +  out_filename;
	si_link l  = ssi_open_for_write (out_filename);
	ssi_write_newstruct (l, STRUCT_NAME, lst);
	ssi_close_and_remove (l);
	return out_filename;
}

bool register_struct (std::string const& name, std::string const& desc)
{
  int tok;
  int cmd_result = blackboxIsCmd (name.c_str(), tok);
  if (cmd_result == ROOT_DECL)
  {
    return true; // already exists
  }

  newstruct_desc d = newstructFromString (desc.c_str());
  if (d == NULL)
  {
    return false;
  }
  newstruct_setup (name.c_str(), d); // is void function
  return true;
}

si_link ssi_open_for_read (std::string const& ssi_filename)
{
  si_link l = static_cast<si_link> (omAlloc0Bin (sip_link_bin));
  l->name = omStrDup (ssi_filename.c_str());
  l->mode = omStrDup ("r");
  l->ref = 1;
  si_link_extension ns = static_cast<si_link_extension>
    (omAlloc0Bin (s_si_link_extension_bin));
  ns = slInitSsiExtension (ns);
  l->m = ns; // originally, Singular has a global list of "extensions"
             // we use a private copy for now

  BOOLEAN res = l->m->Open (l, SI_LINK_READ, NULL);
  if (res)
  {
    throw std::runtime_error ("could not open ssi file " + ssi_filename);
  }

  ssiInfo *d = static_cast<ssiInfo*> (l->data);
  int t = s_readint (d->f_read); // SSI version info in first line
  if (t != 98)
  {
    std::string error_msg = "wrong token, got " + std::to_string (t);
    throw std::runtime_error (error_msg);
  }
  int n98_v, n98_m;
  BITSET n98_o1, n98_o2;
  n98_v = s_readint (d->f_read);
  n98_m = s_readint (d->f_read);
  n98_o1 = s_readint (d->f_read);
  n98_o2 = s_readint (d->f_read);
  if ((n98_v != SSI_VERSION) || (n98_m != MAX_TOK))
  {
    std::string error_msg = "incompatible versions of ssi: expected "
      + std::to_string (SSI_VERSION) + '/' + std::to_string (MAX_TOK)
      + " got " + std::to_string (n98_v) + '/' + std::to_string (n98_m);
    throw std::runtime_error (error_msg);
  }
  si_opt_1 = n98_o1; // do I want to set these global options?
  si_opt_2 = n98_o2;

  return l;
}

lists ssi_read_newstruct (si_link l, std::string const& struct_name)
{
  ssiInfo *d = static_cast<ssiInfo*> (l->data);
  int t = s_readint (d->f_read);
  if (t != 20)
  {
    std::string error_msg = "wrong token, expected 20 got "
      + std::to_string (t);
    throw std::runtime_error (error_msg);
  }
  /*int ignore =*/ s_readint (d->f_read);
  char* name = ssiReadString (d);
  if (struct_name.compare (name) != 0)
  {
    std::string error_msg = "wrong blackbox name, expected " + struct_name
      + " got " + std::string (name);
    throw std::runtime_error (error_msg);
  }
  int tok;
  blackboxIsCmd (name, tok);
  omFree(name);
  if (tok <= MAX_TOK)
	{
		std::string error_msg = "token " + std::to_string (tok)
		 + " is not larger than MAX_TOX " + std::to_string (MAX_TOK);
		throw std::runtime_error (error_msg);
	}
	lists li;
	newstruct_deserialize (NULL, reinterpret_cast<void**> (&li), l);
	return li;
}

std::pair<int, lists> deserialize (std::string const& filename, std::string const& ids, bool delete_file)
{
  //std::cout << "deserializing " << filename << std::endl;
	if (!(register_struct (STRUCT_NAME, STRUCT_DESC)))
	{
		 throw std::runtime_error (ids + ": could not register structs");
	}
	int type;
	blackboxIsCmd (STRUCT_NAME.c_str(), type);
	si_link l = ssi_open_for_read (filename);
	lists lst = ssi_read_newstruct (l, STRUCT_NAME);
	ssi_close_and_remove (l);
  if(delete_file) {std::remove(filename.c_str());}
	return {type, lst};
}

void load_singular_library (std::string const& library_name)
{
  char *s = omStrDup (library_name.c_str());
  BOOLEAN res = iiGetLibStatus (s);
  if (res == TRUE) // already loaded
  {
    omFree (s);
    return;
  }
  res = iiLibCmd (s, TRUE, FALSE, FALSE); // also frees s
  if (res)
  {
    throw std::runtime_error ("could not load library " + library_name);
  }
}

idhdl symbol (std::string const& lib, std::string const& fun)
{
  load_singular_library (lib);
  idhdl h = ggetid (fun.c_str());
  if (h == NULL)
  {
    throw std::runtime_error ("procedure " + fun + " not available in " + lib);
  }
  return h;
}


ScopedLeftv::ScopedLeftv ()
: _ (static_cast<leftv> (omAlloc0Bin (sleftv_bin)))
{ }
ScopedLeftv::ScopedLeftv (int c, void* data)
: _ (static_cast<leftv> (omAlloc0Bin (sleftv_bin)))
{
  _->rtyp = c;
  _->data = data;
}
  ScopedLeftv::ScopedLeftv (ScopedLeftv& parent, int c, void* data)
: ScopedLeftv (c, data)

{
  chained = true;

  leftv current = parent._;
  while (current->next != NULL) {
      current = current->next;
  }
  current->next = _;
}
void ScopedLeftv::add_argument (std::pair<int,void*> singular_data)
{
  ScopedLeftv new_argument (*this, singular_data.first, singular_data.second);
}
ScopedLeftv::~ScopedLeftv()
{
  if (!chained) {
    _->CleanUp();
    omFreeBin (_, sleftv_bin);
  }
};

leftv ScopedLeftv::leftV() const
{
  return _;
}


void init_singular (std::string const& library_path)
 {
   if (currPack == NULL) // use this to check if this instance has already been
                         // initializied
   {
     mp_set_memory_functions (omMallocFunc, omReallocSizeFunc, omFreeSizeFunc);
     siInit (const_cast<char*> (library_path.c_str()));
     currentVoice = feInitStdin (NULL);
     errorreported = 0;
     myynest = 1;
   }
 }

template<typename R> std::vector<std::pair<int, R>> proc (idhdl h, ScopedLeftv const& arg)
{
  BOOLEAN const res (iiMake_proc (h, NULL, arg.leftV()->next)); // start with second argument in the chained list as the first is the list head (declared as "ScopedLeftv args;")

  if (res)
  {
    throw std::runtime_error ("call to procedure " + std::string (h->id)
                              + " failed");
  }

  std::vector<std::pair<int, R>> v;
  leftv current = &iiRETURNEXPR;
  while (current != NULL) {
      v.push_back(std::make_pair(current->Typ(), static_cast<R> (current->Data())));
      current = current->next;
  }

  //iiRETURNEXPR.data = NULL;
  //iiRETURNEXPR.CleanUp();

  return v;
}

std::vector<std::pair<int, void*>> call_user_proc (std::string const& function_name,
  std::string const& needed_library, int in_type, lists in_lst)
{
  ScopedLeftv arg (in_type, lCopy (in_lst));
  return proc<void*> (symbol (needed_library, function_name), arg);
}

std::vector<std::pair<int, void*>> call_user_proc (std::string const& function_name,
  std::string const& needed_library, ScopedLeftv& u_arg)
{
  return proc<void*> (symbol (needed_library, function_name), u_arg);
}

std::string worker()
{
	char hstn[65];
	gethostname (hstn, 64);
	hstn[64] = '\0';
	std::string ids (hstn);
	ids = ids + " " + std::to_string (getpid());
	return ids;
}
