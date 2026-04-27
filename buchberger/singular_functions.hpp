#pragma once

#include <string>

#include <Singular/libsingular.h>
#include <Singular/links/ssiLink.h> // for ssiInfo etc.
#include <Singular/newstruct.h>
#include <boost/variant.hpp>
#include <vector>
#include <polys/prCopy.h>


// Singular defines this in ssiLink.cc
#define SSI_VERSION 13

#define pSubtractComp(p,c) p_SubtractComp(p,c,currRing)



//#include <libpolys/coeffs/si_gmp.h>
//#include <libpolys/coeffs/coeffs.h>
#include <Singular/ipid.h>   // coeffs_BIGINT
#include <boost/multiprecision/cpp_int.hpp>
using GpiBigint = boost::multiprecision::cpp_int;
number GpiBigint_to_singular_bigint(const GpiBigint& x);
GpiBigint singular_bigint_to_GpiBigint(number n);



//ideal idrCopyR_NoSort(ideal id, ring src_r, ring dest_r);
//poly  prMoveR_NoSort (poly  &p, ring src_r, ring dest_r);

// these are from ssiLink.cc
char* ssiReadString(const ssiInfo *d);

// these are from newstruct.cc
BOOLEAN newstruct_deserialize(blackbox **, void **d, si_link f);
BOOLEAN newstruct_serialize(blackbox *b, void *d, si_link f);

//structs
class ScopedLeftv
{
  public:
    ScopedLeftv ();
    ScopedLeftv (int c, void* data);
    ScopedLeftv (ScopedLeftv& parent, int c, void* data);
    ~ScopedLeftv();
    void add_argument (std::pair<int,void*> singular_data);
    ScopedLeftv (ScopedLeftv const&) = delete;
    ScopedLeftv (ScopedLeftv&&) = delete;
    ScopedLeftv& operator= (ScopedLeftv const&) = delete;
    ScopedLeftv& operator= (ScopedLeftv&&) = delete;
    leftv leftV() const;
  private:
    leftv _;
    bool chained = false;
};

void p_SubtractComp(poly p, unsigned long c, ring r);

void writePolySSI(poly P, std::string out_filename);
poly  readPolySSI  (std::string filename, BOOLEAN delete_file);
void  writeIdealSSI(ideal I, std::string out_filename);
ideal readIdealSSI (std::string filename, BOOLEAN delete_file);
void  writeRingSSI (ring r, std::string out_filename);
ring  readRingSSI  (std::string filename, BOOLEAN delete_file);

BOOLEAN jjRINGLIST(leftv res, leftv v);

int get_struct_cmd();

void init_singular (std::string const&);
void call_singular ( std::string const&);
void call_singular_and_discard (std::string const&);
std::string filename_generator (std::string);
bool register_struct (std::string const&, std::string const&);
si_link ssi_open_for_read (std::string const&);
si_link ssi_open_for_write (std::string const&);
void ssi_close_and_remove (si_link);
void ssi_write_newstruct (si_link, std::string const&, lists);
void ssi_write_newstruct (si_link, int, lists);
std::string serialize (lists, std::string const&, std::string);
si_link ssi_open_for_read (std::string const& );
lists ssi_read_newstruct (si_link , std::string const& );
std::pair<int, lists> deserialize (std::string const& filename, std::string const& ids, bool delete_file=false);
void load_singular_library (std::string const&);


std::vector<std::pair<int, void*>> call_user_proc (std::string const&,
                                      std::string const&,
                                      int,
                                      lists);

std::vector<std::pair<int, void*>> call_user_proc (std::string const&,
                                      std::string const&,
                                      ScopedLeftv&);
std::string worker();
