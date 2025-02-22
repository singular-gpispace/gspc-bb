#pragma once

#define NO_NAME_MANGLING extern "C"

#include <string>
#include <config.hpp>
#include <boost/variant.hpp>

#include <iostream>

#include <we/type/literal/control.hpp>
#include <we/type/bitsetofint.hpp>
#include <we/type/bytearray.hpp>
#include <we/type/value.hpp>
#include <map>

#define SLEEP_TIME_MS 0
#define CONTROL_TOKEN we::type::literal::control{}

#define NF_IS_ZERO            1
#define STARTED              -1
#define DIFFERENT_COMPONENTS -2
#define CHAIN_CRITERION      -3
#define PRODUCT_CRITERION    -4
#define UNNECESSARY_ELEMENT  -5

#define RESOLVE_INTERFACE_FUNCTION(function) \
      (fhg::util::scoped_dlhandle \
      (config::implementation(), \
      RTLD_GLOBAL | RTLD_NOW | RTLD_DEEPBIND) \
      .sym<decltype(function)> \
      (BOOST_PP_STRINGIZE(function)))







// type used by GPI-Space if you set the type of a place or an "out-many" port to "list":
using GpiVariant = pnet::type::value::value_type;
using GpiList = std::list<GpiVariant>;
using GpiSet = std::set<GpiVariant>;
using GpiMap = std::map<GpiVariant,GpiVariant>;

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
    //return T{};
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

  template <typename U>
  void operator() (const U&) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "type not implemented" << std::endl;
  }
};

inline void print_variant(GpiVariant const& v, int depth=0) {boost::apply_visitor(print_variant_visitor(depth), v);}

// helper functions:
inline GpiList lcm_monom(GpiVariant const& m1, GpiVariant const& m2)
{
  GpiList M1 = get_list(m1);
  GpiList M2 = get_list(m2);
  GpiList res;
  if(M1.size()!=M2.size()) {throw std::runtime_error ("exponent vectors have different lengths");}
  GpiList::iterator it1 = M1.begin();
  GpiList::iterator it2 = M2.begin();
  for(; it1 != M1.end(); ++it1, ++it2)
  {
    res.emplace_back(std::max(boost::get<long>(*it1), boost::get<long>(*it2)));
  }
  return res;
}
inline GpiList lcm(GpiVariant const& t1, GpiVariant const& t2)
{
  GpiList T1 = get_list(t1);
  GpiList T2 = get_list(t2);
  GpiVariant m1 = T1.front();
  GpiVariant m2 = T2.front();
  long T1_comp = boost::get<long>(*std::next(T1.begin()));
  long T2_comp = boost::get<long>(*std::next(T2.begin()));
  GpiList res;
  if(T1_comp==T2_comp)
  {
    res.emplace_back(lcm_monom(m1,m2));
    res.emplace_back(T1_comp);
  }
  return res; // will be empty if T1,T2 are in different components!
}
inline long deg(GpiVariant const& m)
{
  GpiList M = get_list(get_list(m).front());
  if(M.size()==0) {return(-1);}
  long d=0;
  for(GpiList::iterator it = M.begin(); it != M.end(); ++it)
  {
    d += boost::get<long>(*it);
  }
  return d;
}
inline bool coprime_monom(GpiVariant const& m1, GpiVariant const& m2)
{
  GpiList M1 = get_list(m1);
  GpiList M2 = get_list(m2);
  if(M1.size()!=M2.size()) {throw std::runtime_error ("exponent vectors have different lengths");}
  GpiList::iterator it1 = M1.begin();
  GpiList::iterator it2 = M2.begin();
  for(; it1 != M1.end(); ++it1, ++it2)
  {
    if(boost::get<long>(*it1) && boost::get<long>(*it2)) {return false;}
  }
  return true;
}
inline bool divides_monom(GpiVariant const& m1, GpiVariant const& m2)
{
  GpiList M1 = get_list(m1);
  GpiList M2 = get_list(m2);
  if(M1.size()!=M2.size()) {throw std::runtime_error ("exponent vectors have different lengths");}
  GpiList::iterator it1 = M1.begin();
  GpiList::iterator it2 = M2.begin();
  for(; it1 != M1.end(); ++it1, ++it2)
  {
    if(boost::get<long>(*it1) > boost::get<long>(*it2)) {return false;}
  }
  return true;
}
inline bool dp_larger_equal(GpiVariant const& t1, GpiVariant const& t2)
{
  GpiList T1 = get_list(t1);
  GpiList T2 = get_list(t2);

  GpiList M1 = get_list(T1.front());
  GpiList M2 = get_list(T2.front());
  if(M1.size()!=M2.size()) {throw std::runtime_error ("exponent vectors have different lengths");}
  //std::cout << "      nvars=" << M1.size() << std::endl;
  long d1 = deg(t1);
  long d2 = deg(t2);
  //std::cout << "      d1=" << d1 << std::endl;
  //std::cout << "      d2=" << d2 << std::endl;
  if(d1>d2) {return true;}
  if(d1<d2) {return false;}

  GpiList::reverse_iterator it1 = M1.rbegin();
  GpiList::reverse_iterator it2 = M2.rbegin();
  for(; it1 != M1.rend(); ++it1, ++it2)
  {
    int exp1 = boost::get<long>(*it1);
    int exp2 = boost::get<long>(*it2);
    //std::cout << "       " << exp1 << "  " << exp2 << std::endl;
    if(exp1<exp2) {return true;}
    if(exp1>exp2) {return false;}
  }

  long T1_comp = boost::get<long>(*std::next(T1.begin()));
  long T2_comp = boost::get<long>(*std::next(T2.begin()));
  if(T1_comp>T2_comp) {return true;}
  if(T1_comp<T2_comp) {return false;}

  return true;
}

// call like: chain_criterion(i, j, A, kill_indices, M, get_list(*std::next(M.begin(),i)), get_list(*std::next(M.begin(),j)));

inline void chain_criterion(int const& i, int const& j, GpiMap& A, GpiList& kill_indices, GpiList const& M, GpiList const& Mi, GpiList const& Mj)
{
  long Mi_comp = boost::get<long>(*std::next(Mi.begin()));
  long Mj_comp = boost::get<long>(*std::next(Mj.begin()));
  if(Mi_comp==Mj_comp)
  {
    int r = M.size();
    int entry;
    GpiList::const_iterator it = M.begin();
    for(int k=1; k<=r; k++, ++it)
    {
      GpiList Mk = get_list(*it);
      long Mk_comp = boost::get<long>(*std::next(Mk.begin()));
      if(k!=i && k!=j && Mi_comp==Mk_comp)
      {
        //row
        entry = boost::get<int>(get_list(A[GpiSet({i,k})]).front());
        if(entry>=2 || entry==NF_IS_ZERO || entry==PRODUCT_CRITERION || entry==CHAIN_CRITERION || entry==DIFFERENT_COMPONENTS)
        {
          if(divides_monom(Mi.front(), get_list(*std::next(get_list(A[GpiSet({k,j})]).begin())).front()))  // chain-criterion: if (i,k) finished as well and mi divides lcm(mj,mk) then (k,j) can be discarded
          {
            entry = boost::get<int>(get_list(A[GpiSet({k,j})]).front());
            if(entry == STARTED)
            {
              if(k>j) {kill_indices.emplace_back(std::to_string(k)+","+std::to_string(j));}
              else    {kill_indices.emplace_back(std::to_string(j)+","+std::to_string(k));}
            }
            if(entry == STARTED || entry == 0)
            {
              get_list(A[GpiSet({k,j})]).front() = CHAIN_CRITERION;
              chain_criterion(k, j, A, kill_indices, M, Mk, Mj);
            }
          }
        }
        //column
        entry = boost::get<int>(get_list(A[GpiSet({k,j})]).front());
        if(entry>=2 || entry==NF_IS_ZERO || entry==PRODUCT_CRITERION || entry==CHAIN_CRITERION || entry==DIFFERENT_COMPONENTS)
        {
          if(divides_monom(Mj.front(), get_list(*std::next(get_list(A[GpiSet({i,k})]).begin())).front()))  // chain-criterion: if (i,k) finished as well and mi divides lcm(mj,mk) then (k,j) can be discarded
          {
            entry = boost::get<int>(get_list(A[GpiSet({i,k})]).front());
            if(entry == STARTED)
            {
              if(i>k) {kill_indices.emplace_back(std::to_string(i)+","+std::to_string(k));}
              else    {kill_indices.emplace_back(std::to_string(k)+","+std::to_string(i));}
            }
            if(entry == STARTED || entry == 0)
            {
              get_list(A[GpiSet({i,k})]).front() = CHAIN_CRITERION;
              chain_criterion(i, k, A, kill_indices, M, Mi, Mk);
            }
          }
        }
      }
    }
  }
}

inline int product_criterion(GpiList const& Mi, GpiList const& Mj)
{
  long Mi_comp = boost::get<long>(*std::next(Mi.begin()));
  long Mj_comp = boost::get<long>(*std::next(Mj.begin()));
  if(Mi_comp==0) // ideals & polynomials
  {
    if(coprime_monom(Mi.front(), Mj.front()))  // chain-criterion: if (i,k) finished as well and mi divides lcm(mj,mk) then (k,j) can be discarded
    {
      return PRODUCT_CRITERION;
    }
  }
  else // modules & vectors
  {
    if(Mi_comp!=Mj_comp)
    {
      return DIFFERENT_COMPONENTS;
    }
  }
  return 0;
}

inline void queue_insert(GpiList& Q, GpiList const& data)
{
  //GpiList::const_iterator state = std::next(data.begin(),2);
  //std::cout << " IN Q_INSERT:   state: " << (*state).which() << std::endl;

  if(Q.size()==0)
  {
    Q.push_back(data);
    return;
  }
  int i=0;
  for(GpiList::iterator it=Q.begin(); it!=Q.end(); ++it)
  {
    i++; //std::cout << "    i=" << i << std::endl;
    if(dp_larger_equal(data.back(), get_list(*it).back()))
    {
      //std::cout << "     inserted!" << std::endl;
      Q.insert(it, data);
      return;
    }
  }
  Q.push_back(data);
}

inline void queue_remove(GpiList& Q, int i, int j) // order of i,j does not matter! // SLOW! (O(r^2))
{
  if(i<j) {queue_remove(Q,j,i);}
  for(GpiList::iterator it=Q.begin(); it!=Q.end(); ++it)
  {
    if(boost::get<int>(get_list(*it).front())==i && boost::get<int>(*std::next(get_list(*it).begin()))==j)
    {
      Q.erase(it);
      return;
    }
  }
  throw std::runtime_error ("tried to remove ("+std::to_string(i)+","+std::to_string(j)+")-element, but it was not was not in queue");
}

inline void update_queue(GpiList& Q, GpiMap& bookkeeping, GpiList& Qback) // remove all finished (i,j) from Q
{
  int k=1;
  int r = Q.size();
  for (GpiList::iterator Qk=Q.begin(); k<=r; k++)
  {
    int i = boost::get<int>(get_list(*Qk).front());
    int j = boost::get<int>(*std::next(get_list(*Qk).begin()));
    GpiVariant tmpvar = (bookkeeping[GpiSet({i,j})]);
    int state = boost::get<int>(get_list(tmpvar).front());
    if(state!=0 && state!=STARTED)
    {
      Q.erase(Qk++);
    }
    else
    {
      ++Qk;
    }
  }
  Qback = GpiList({get_list(Q.back()).front(), *std::next(get_list(Q.back()).begin())});
}







NO_NAME_MANGLING

//std::string filename_gen(std::string const&);

void singular_buchberger_compute(std::string const& singular_library_name,
																 std::string const& singular_function_name,
															 	 std::string const& base_filename,
																 std::vector<boost::variant<long,std::string,GpiList>> const& args_read,
																 std::vector<boost::variant<long,std::string,GpiList>> const& args_in,
																 std::vector<boost::variant<long*,std::string*,GpiList*>> &args_inout,
																 std::vector<boost::variant<long*,std::string*,GpiList*>> &out,
																 std::vector<GpiList*> &out_many,
															 	 bool delete_files=true,
                                 bool silent=true);
