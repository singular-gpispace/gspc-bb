#pragma once

#define DEBUG_BBA

#define USE_KNF true

#define NO_NAME_MANGLING extern "C"

#include <string>
#include <config.hpp>
#include <boost/variant.hpp>
//#include <vector>

#include <iostream>

#include <we/type/literal/control.hpp>
#include <we/type/bitsetofint.hpp>
#include <we/type/bytearray.hpp>
#include <we/type/value.hpp>
#include <map>

#define CONTROL_TOKEN we::type::literal::control{}

#define RESOLVE_INTERFACE_FUNCTION(function) \
      (fhg::util::scoped_dlhandle \
      (config::implementation(), \
      RTLD_GLOBAL | RTLD_NOW | RTLD_DEEPBIND) \
      .sym<decltype(function)> \
      (BOOST_PP_STRINGIZE(function)))



extern int Kstd1_deg;


// types needed from SINGULAR

typedef struct spolyrec   *      poly;
typedef struct sip_sideal *     ideal;
typedef struct skStrategy * kStrategy;


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


// for debugging

class print_variant_visitor : public boost::static_visitor<void>
{
private:
  int depth;
public:
  print_variant_visitor(int const& depth=0)
  {
    this->depth = depth;
  }
  void operator() (const we::type::literal::control&) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "CONTROL_TOKEN";
  }
  void operator() (const GpiList& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "List of " << data.size() << " elements:" << std::endl;
    for(GpiList::const_iterator it=data.begin(); it!=data.end(); ++it)
      {boost::apply_visitor(print_variant_visitor(this->depth + 1), *it);}
  }
  void operator() (const GpiSet& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "Set of " << data.size() << " elements:" << std::endl;
    for(GpiSet::const_iterator it=data.begin(); it!=data.end(); ++it)
      {boost::apply_visitor(print_variant_visitor(this->depth + 1), *it);}
  }
  void operator() (const GpiMap& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "Map of " << data.size() << " elements:" << std::endl;
    for(GpiMap::const_iterator it=data.begin(); it!=data.end(); ++it) {
      boost::apply_visitor(print_variant_visitor(this->depth + 1), it->first);
      boost::apply_visitor(print_variant_visitor(this->depth + 1), it->second);
    }
  }
  void operator() (const bool& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << data << std::endl;
  }
  void operator() (const int& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << data << "   (int)" << std::endl;
  }
  void operator() (const long& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << data << "   (long)" << std::endl;
  }
  void operator() (const std::string& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << data << "   (string)" << std::endl;
  }

  template <typename U>
  void operator() (const U&) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "type not implemented" << std::endl;
  }
};

inline void print_variant(GpiVariant const& v, int depth=0) {boost::apply_visitor(print_variant_visitor(depth), v);}


// helper functions for handling lead monomials:

inline bool dp_larger_equal(GpiList const& T1, GpiList const& T2, int d1, int d2)
{
  if(T1.size()!=T2.size()) {throw std::runtime_error ("exponent vectors have different lengths in dp_larger_equal");}
  if(d1>d2) {return true;}
  if(d1<d2) {return false;}

  GpiList::const_reverse_iterator it1 = T1.rbegin();
  GpiList::const_reverse_iterator it2 = T2.rbegin();
  int T1_comp = boost::get<int>(*it1); ++it1;
  int T2_comp = boost::get<int>(*it2); ++it2;
  for(; it1 != T1.rend(); ++it1, ++it2)
  {
    int exp1 = boost::get<int>(*it1);
    int exp2 = boost::get<int>(*it2);
    if(exp1<exp2) {return true;}
    if(exp1>exp2) {return false;}
  }
  if(T1_comp>T2_comp) {return true;}
  if(T1_comp<T2_comp) {return false;}

  return true;
}

inline bool equal(std::vector<int> const& m1, std::vector<int> const& m2)
{
  std::vector<int>::const_iterator it1 = m1.begin();
  std::vector<int>::const_iterator it2 = m2.begin();
  std::vector<int> res;
  for(; it1 != m1.end(); ++it1, ++it2)
  {
    if(*it1!=*it2) {return false;}
  }
  return true;
}

inline bool divides_monom(std::vector<int> const& m1, std::vector<int> const& m2)
{
  if (m1.back()!=m2.back()) {return false;} // different components
  std::vector<int>::const_iterator it1 = m1.begin();
  std::vector<int>::const_iterator it2 = m2.begin();
  std::vector<int> res;
  for(; it1 != m1.end(); ++it1, ++it2)
  {
    if(*it1>*it2) {return false;}
  }
  return true;
}

inline bool coprime_monom(std::vector<int> const& m1, std::vector<int> const& m2)
{
  if (m1.back()!=m2.back()) {return false;} // different components
  std::vector<int>::const_iterator it1 = m1.begin();
  std::vector<int>::const_iterator it2 = m2.begin();
  std::vector<int> res;
  for(; it1 != m1.end(); ++it1, ++it2)
  {
    if(*it1!=0 && *it2!=0) {return false;}
  }
  return true;
}

inline int deg(std::vector<int> const& monom)
{
  int d = 0;
  int n = monom.size()-1;
  for(std::vector<int>::const_iterator it = monom.begin(); n>0; ++it)
  {
    d += (*it);
    n--;
  }
  return d;
}

inline std::vector<int> lcm(std::vector<int> const& m1, std::vector<int> const& m2)
{
  std::vector<int>::const_iterator it1 = m1.begin();
  std::vector<int>::const_iterator it2 = m2.begin();
  std::vector<int> res;
  for(; it1 != m1.end(); ++it1, ++it2)
  {
    res.emplace_back(std::max(*it1, *it2));
  }
  return res;
}


// vector <--> list

inline GpiList vec2list(std::vector<int> vec)
{
  GpiList L;
  for (std::vector<int>::iterator it=vec.begin(); it!=vec.end(); ++it)
  {
    L.emplace_back(*it);
  }
  return L;
}

inline std::vector<int> list2vec(GpiList L)
{
  std::vector<int> vec;
  for (GpiList::iterator it=L.begin(); it!=L.end(); ++it)
  {
    vec.emplace_back(boost::get<int>(*it));
  }
  return vec;
}

inline std::vector<int> list2vec(GpiVariant L)
{
  return list2vec(get_list(L));
}

inline std::vector<int> list2vec_long(GpiList L)
{
  std::vector<int> vec;
  for (GpiList::iterator it=L.begin(); it!=L.end(); ++it)
  {
    vec.emplace_back((int) boost::get<long>(*it));
  }
  return vec;
}


// Product and Chain Criterion

inline bool test_PC(std::vector<int> const& Mi, std::vector<int> const& Mj)
{
  int Mi_comp = Mi.back();
  int Mj_comp = Mj.back();
  if(Mi_comp==0) // ideals & polynomials
  {
    return coprime_monom(Mi, Mj); // product-criterion: if mi and mj are coprime then (i,j) can be discarded
  }
  else // modules & vectors
  {
    return (Mi_comp!=Mj_comp); // for modules: test instead if the leads lie in different components (thus spoly=0 trivially)
  }
}

inline bool test_CC(std::vector<int> const& lcm_i_j, std::vector<int> const& Mi, std::vector<int> const& Mj, std::vector<int> const& Mk)
{
  int Mi_comp = Mi.back();
  int Mj_comp = Mj.back();
  int Mk_comp = Mk.back();

  if(Mi_comp==Mj_comp && Mi_comp==Mk_comp)
  {
    return (divides_monom(Mk, lcm_i_j));  // chain-criterion: if mk divides lcm(mi,mj) then (i,j) can be discarded
  }
  return false;
}


// helper functions for the queue

inline void queue_insert(GpiList& Q, int i, int j, int deg_lcm, GpiList const& lcm)
{
  GpiList data = {i, j, deg_lcm, lcm};
  if(Q.size()==0)
  {
    Q.push_back(data);
    return;
  }
  int k=0;
  for(GpiList::iterator it=Q.begin(); it!=Q.end(); ++it)
  {
    k++; //std::cout << "    k=" << k << std::endl;
    GpiList entry = get_list(*it);
    if(dp_larger_equal(lcm, get_list(entry.back()), deg_lcm, boost::get<int>(*std::next(entry.begin(),2))))
    {
      Q.insert(it, data);
      return;
    }
  }
  Q.push_back(data);
}

inline void queue_delete_i_j(GpiList& Q, int i, int j) // remove index (i,j) from Q
{
  for (GpiList::iterator Qk=Q.begin(); Qk!=Q.end(); )
  {
    int ii = boost::get<int>(get_list(*Qk).front());
    int jj = boost::get<int>(*std::next(get_list(*Qk).begin()));
    if(ii==i && jj==j)
    {
      Qk = Q.erase(Qk);
      break;
    }
    else
    {
      ++Qk;
    }
  }
}

inline void queue_delete_i(GpiList& Q, int i) // remove indices (i,j) and (j,i) from Q (for all j)
{
  for (GpiList::iterator Qk=Q.begin(); Qk!=Q.end(); )
  {
    int ii = boost::get<int>(get_list(*Qk).front());
    int jj = boost::get<int>(*std::next(get_list(*Qk).begin()));
    if(ii==i || jj==i)
    {
      Qk = Q.erase(Qk);
    }
    else
    {
      ++Qk;
    }
  }
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
                                 bool silent);

NO_NAME_MANGLING
std::vector<std::vector<int>> singular_buchberger_get_M_and_init_F(std::string const& base_filename,
                                                                   std::string const& input,
                                                                   GpiMap* runtime);

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
                                    GpiList* NF);

NO_NAME_MANGLING
poly read_generator (std::string const& base_filename,
                     int k);

NO_NAME_MANGLING
void singular_buchberger_reduce_GB(std::string const& base_filename,
                                   std::list<poly> const& generators,
                                   long needed_indices,
                                   int current_index,
                                   int final_r,
                                   long redSB,
                                   GpiMap* runtime);
