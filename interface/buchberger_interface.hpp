#pragma once

//#define DEBUG_BBA
#define OLD_REDTAIL

#define HEAD_SIZE_FACTOR 1.0
#define TIME_SCALE_FACTOR 2

#define TRACE 0

#define isSyzygy(p, syzComp) (syzComp>0 && (p!=NULL && p_GetComp(p, currRing) > syzComp))

#define SLEEP_MS(t) std::this_thread::sleep_for( std::chrono::milliseconds(t) );

#define TYPE_ID(T) GpiVariant(T()).which()

#define NO_NAME_MANGLING extern "C"

#define sel_strat_larger_equal dp_larger_equal
#define Q_larger_equal         dp_larger_equal
//#define sel_strat_larger_equal posInL110_larger_equal
//#define Q_larger_equal         posInL110_larger_equal


// defines for singular options bitset
// std options (si_opt_1)
#define PROT            0
#define REDSB           1
#define NOT_BUCKETS     2
#define NOT_SUGAR       3
#define INTERRUPT       4
#define SUGARCRIT       5
#define DEBUG           6
#define REDTHROUGH      7
#define NO_SYZ_MINIM    8
#define RETURN_SB       9
#define FASTHC         10
#define OLDSTD         20
#define REDTAIL_SYZ    21
#define STAIRCASEBOUND 22
#define MULTBOUND      23
#define DEGBOUND       24
#define REDTAIL        25
#define INTSTRATEGY    26
#define FINDET         27
#define INFREDTAIL     28
#define SB_1           29
#define NOTREGULARITY  30
#define WEIGHTM        31
// verbose options (si_opt_2)
#define QUIET          32
#define QRING          33
#define SHOW_MEM       34
#define YACC           35
#define REDEFINE       36
#define READING        37
#define LOAD_LIB       38
#define DEBUG_LIB      39
#define LOAD_PROC      40
#define DEF_RES        41
#define SHOW_USE       43
#define IMAP           44
#define PROMPT         45
#define NSB            46
#define CONTENTSB      47
#define CANCELUNIT     48
#define MODPSOLVSB     49
#define UPTORADICAL    50
#define FINDMONOM      51
#define COEFSTRAT      52
#define IDLIFT         53
#define LENGTH         54
#define ALLWARN        56
#define INTERSECT_ELIM 57
#define INTERSECT_SYZ  58
#define ASSIGN_NONE    59
#define PURE_GB        60
#define DEG_STOP       63

#include <string>
#include <config.hpp>
#include <boost/variant.hpp>
//#include <vector>

#include <iostream>
#include <fstream>

#include <gspc/we/type/literal/control.hpp>
#include <gspc/we/type/bitsetofint.hpp>
#include <gspc/we/type/bytearray.hpp>
#include <gspc/we/type/value.hpp>
#include <gspc/we/type/value/poke.hpp>
#include <gspc/we/type/value/peek.hpp>

#include <map>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <cassert>

#include <gspc/rpc/function_description.hpp>

#include <boost/multiprecision/cpp_int.hpp>

#define CONTROL_TOKEN gspc::we::type::literal::control{}

#define RESOLVE_INTERFACE_FUNCTION(function) \
    (gspc::util::scoped_dlhandle \
    (std::filesystem::path(config::implementation().string()), \
    RTLD_GLOBAL | RTLD_NOW | RTLD_DEEPBIND) \
    .sym<decltype(function)> \
    (BOOST_PP_STRINGIZE(function)))



// types needed from SINGULAR ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct spolyrec   *      poly;
typedef struct sip_sideal *     ideal;
typedef struct skStrategy * kStrategy;


// types used by GPI-Space ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using bitset     = gspc::pnet::type::bitsetofint::type;
using bytearray  = gspc::we::type::bytearray;
using GpiBigint  = boost::multiprecision::cpp_int;
using GpiVariant = gspc::pnet::type::value::value_type; // recursive variant type used for all tokens
using GpiStruct  = gspc::pnet::type::value::structured_type; // type used for structs
using GpiList    = std::list<GpiVariant>;
using GpiSet     = std::set<GpiVariant>;
using GpiMap     = std::map<GpiVariant,GpiVariant>;

using gspc::pnet::type::value::poke;
using gspc::pnet::type::value::peek;

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

inline GpiList&   get_list   (GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiList   >(), v);}
inline GpiSet&    get_set    (GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiSet    >(), v);}
inline GpiMap&    get_map    (GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiMap    >(), v);}
inline GpiStruct& get_struct (GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiStruct >(), v);}

inline GpiList   const& get_list   (GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiList   const>(), v);}
inline GpiSet    const& get_set    (GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiSet    const>(), v);}
inline GpiMap    const& get_map    (GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiMap    const>(), v);}
inline GpiStruct const& get_struct (GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiStruct const>(), v);}

class print_variant_visitor : public boost::static_visitor<void>
{
private:
  int depth;
public:
  print_variant_visitor(int const& depth=0)
  {
    this->depth = depth;
  }
  void operator() (const gspc::we::type::literal::control&) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << "CONTROL_TOKEN";
  }
  void operator() (const GpiList& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << "List of " << data.size() << " elements:" << std::endl;
    for(GpiList::const_iterator it=data.begin(); it!=data.end(); ++it)
      {boost::apply_visitor(print_variant_visitor(std::abs(this->depth) + 1), *it);}
  }
  void operator() (const GpiSet& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << "Set of " << data.size() << " elements:" << std::endl;
    for(GpiSet::const_iterator it=data.begin(); it!=data.end(); ++it)
      {boost::apply_visitor(print_variant_visitor(std::abs(this->depth) + 1), *it);}
  }
  void operator() (const GpiMap& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << "Map of " << data.size() << " elements:" << std::endl;
    for(GpiMap::const_iterator it=data.begin(); it!=data.end(); ++it) {
      boost::apply_visitor(print_variant_visitor(std::abs(this->depth) + 1), it->first);
      boost::apply_visitor(print_variant_visitor(std::abs(this->depth) + 1), it->second);
    }
  }
  void operator() (const GpiStruct& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << "Struct with " << data.size() << " fields:" << std::endl;
    for(GpiStruct::const_iterator it=data.begin(); it!=data.end(); ++it) {
      std::string whitespace2((std::abs(this->depth)+1) * 4, ' ');
      std::cout << whitespace2 << it->first << ": ";
      boost::apply_visitor(print_variant_visitor(-(std::abs(this->depth) + 1)), it->second);
    }
  }
  void operator() (const bool& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << data << std::endl;
  }
  void operator() (const int& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << data << "   (int)" << std::endl;
  }
  void operator() (const unsigned int& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << data << "   (unsigned int)" << std::endl;
  }
  void operator() (const long& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << data << "   (long)" << std::endl;
  }
  void operator() (const unsigned long& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << data << "   (unsigned long)" << std::endl;
  }
  void operator() (const float& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << data << "   (float)" << std::endl;
  }
  void operator() (const double& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << data << "   (double)" << std::endl;
  }
  void operator() (const char& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << data << "   (char)" << std::endl;
  }
  void operator() (const std::string& data) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << data << "   (string)" << std::endl;
  }
  template <typename U>
  void operator() (const U&) const {
    std::string whitespace(std::max(0,this->depth * 4), ' ');
    std::cout << whitespace << "type not implemented" << std::endl;
  }
};

inline void print_variant(GpiVariant const& v, int depth=0) {boost::apply_visitor(print_variant_visitor(depth), v);}

inline bitset uint2bitset(unsigned int v1, unsigned int v2)
{
  bitset bs;
  unsigned long bit = 0;
  while (v1)
  {
    if (v1 & 1)
      bs.ins(bit);
    v1 >>= 1;
    ++bit;
  }
  bit = 32;
  while (v2)
  {
    if (v2 & 1)
      bs.ins(bit);
    v2 >>= 1;
    ++bit;
  }
  return bs;
}


/*
#include <gspc/util/scoped_boost_asio_io_service_with_threads.hpp>
#include <gspc/rpc/remote_socket_endpoint.hpp>
#include <gspc/rpc/service_dispatcher.hpp>
#include <gspc/rpc/service_handler.hpp>
#include <gspc/rpc/service_socket_provider.hpp>
#include <cstdint>

namespace bb
{
  namespace cache
  {
    constexpr static auto socket_address {"/tmp/BB.CACHE"};

    namespace protocol
    {
      FHG_RPC_FUNCTION_DESCRIPTION
        ( get_generator
        , std::uintptr_t (std::string, int)
        );
    }

    struct provider
    {
      private:
      auto get_generator (std::string base, int num) -> std::uintptr_t
      {
        std::ignore = base;
        std::ignore = num;
        return reinterpret_cast<std::uintptr_t> (nullptr);
      }
      gspc::util::scoped_boost_asio_io_service_with_threads _io_service {1};
      //gspc::rpc::remote_socket_endpoint _socket_endpoint {_io_service, "/tmp/BB.CACHE"};
      gspc::rpc::service_dispatcher _service_dispatcher {};
      gspc::rpc::service_handler<protocol::get_generator> _service_handler {_service_dispatcher, [this] (std::string base, int num) { return get_generator (base, num); }};
      gspc::rpc::service_socket_provider _service_provider {_io_service, _service_dispatcher, socket_address};
    };
  }
}
*/

// Priority queue for managing s-poly reductions /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename V, typename Compare>
class PriorityQueue {
  // iterable container holding elements of type pair<K,V> sorted with respect to Compare with logarithmic insertion/removal and average constant time access by a key (of type K)
private:
    using SetType = std::set<std::pair<K,V>, Compare>;
    using MapType = std::unordered_map<K,typename SetType::iterator>;

    SetType entries;
    Compare comp; // = "is-left-of"
    MapType references;
    unsigned long number_of_inserts;
    long syz_comp;
public:
    PriorityQueue(long syz_comp)
    : entries(Compare(syz_comp)), comp(Compare(syz_comp)), number_of_inserts(0), syz_comp(syz_comp) {}

    using iterator = typename SetType::iterator;
    using const_iterator = typename SetType::const_iterator;

    iterator begin() { return entries.begin(); }
    iterator end()   { return entries.end(); }

    const_iterator begin() const { return entries.begin(); }
    const_iterator end()   const { return entries.end(); }

    const std::pair<K,V>& front() const {
      return *(entries.begin());
    }
    const K& front_key() const {
      return front().first;
    }
    const V& front_value() const {
      return front().second;
    }
    const std::pair<K,V>& back() const {
      return *(entries.rbegin());
    }
    const K& back_key() const {
      return back().first;
    }
    const V& back_value() const {
      return back().second;
    }

    std::size_t size() {
      assert(entries.size()==references.size());
      return entries.size();
    }

    // for debugging:
    /*
    std::size_t entries_size() {
      return entries.size();
    }
    std::size_t references_size() {
      return references.size();
    }
    SetType& get_entries() {
      return entries;
    }
    MapType& get_references() {
      return references;
    }
    int test_key(const K& key) {
      typename MapType::iterator it = references.find(key);
      if (it == references.end()) return -1;
      return std::distance(entries.begin(), it->second);
    }
    int test_key_value(const std::pair<K,V>& key_value) {
      typename SetType::iterator it = entries.find(key_value);
      if (it == entries.end()) return -1;
      return std::distance(entries.begin(), it);
    }
    */


    std::pair<iterator,bool> push(const std::pair<K,V>& key_value) {
      // Keep a strict one-to-one mapping between a key and a queue entry.
      // Without this guard, duplicate keys can accumulate in `entries` while
      // `references` only tracks the last one, corrupting queue operations.
      /*
      typename MapType::iterator existing = references.find(key_value.first);
      if (existing != references.end()) {
        return std::make_pair(existing->second, false);
      }
      */

      std::pair<iterator,bool> res = entries.insert(key_value);
      if (res.second) {
        number_of_inserts++;
        references[key_value.first] = res.first;
      }
      return res;
    }
    std::pair<iterator,bool> push(const K& key, const V& value) {
      return push(std::make_pair(key,value));
    }

    const std::pair<K,V>& get(const K& key) const {
      typename MapType::const_iterator it = references.find(key);
      if (it == references.end()) throw std::runtime_error("Element not in queue");
      return *(it->second);
    }
    const              V& get_value(const K& key) const {
      return get(key).second;
    }

    iterator erase(const K& key) {
      typename MapType::iterator it = references.find(key);
      if (it == references.end()) return end();

      iterator entry = it->second;
      iterator next = entries.erase(entry);
      references.erase(it);

      return next;
    }
    iterator erase(iterator it) {
      if (it == entries.end()) return end();
      K key = (*it).first;

      references.erase(key);
      iterator next = entries.erase(it);

      return next;
    }

    const std::pair<K,V>& top() const {
      iterator it = entries.begin();
      if (it == entries.end()) throw std::runtime_error("Queue empty");
      return *it;
    }
    const              K& top_key() const {
      return top().first;
    }
    const              V& top_value() const {
      return top().second;
    }

    std::pair<K,V> pop() {
      std::pair<K,V> entry = top();
      erase(begin());
      return entry;
    }
    K              pop_key() {
      return pop().first;
    }
    V              pop_value() {
      return pop().second;
    }

    iterator       find(const K& key) {
      typename MapType::iterator it = references.find(key);
      if(it==references.end()) return entries.end();
      return it->second;
    }
    const_iterator find(const K& key) const {
      typename MapType::const_iterator it = references.find(key);
      if(it==references.end()) return entries.end();
      return it->second;
    }
    iterator       find(const std::pair<K,V>& key_value) {
      return (entries.find(key_value));
    }
    const_iterator find(const std::pair<K,V>& key_value) const {
      return (entries.find(key_value));
    }

    bool contains_key(const K& key) const {
      return (references.find(key)!=references.end());
    }
    bool contains(const std::pair<K,V>& key_value) const {
      return (entries.find(key_value)!=entries.end());
    }

    unsigned long inserts()
    {
      return number_of_inserts;
    }

    long get_syz_comp()
    {
      return syz_comp;
    }
};

namespace std {
    template<>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& p) const noexcept {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };
}

struct Qdata {
          int i;
          int j;
  mutable int old_r;
          int deg_lcm;
          int length;
          GpiList l_spoly; // maybe make vector?
          GpiList lcm;     // same
          unsigned long tie_break;
  mutable GpiList new_lead; // data needed to add reduction result as new GB element later
  mutable bool PC;
};

inline bool dp_larger_equal(std::pair<std::pair<int,int>,Qdata> const& Qentry1, std::pair<std::pair<int,int>,Qdata> const& Qentry2, long syz_comp)
{
  GpiList T1 = Qentry1.second.lcm;
  GpiList T2 = Qentry2.second.lcm;
  int d1 = Qentry1.second.deg_lcm;
  int d2 = Qentry2.second.deg_lcm;
  //std::cout << " comparing T1 " << std::endl;
  //print_variant(T1);
  //std::cout << " ... and T2 " << std::endl;
  //print_variant(T2);
  GpiList::const_reverse_iterator it1 = T1.rbegin();
  GpiList::const_reverse_iterator it2 = T2.rbegin();
  int T1_comp = boost::get<int>(*it1); ++it1;
  int T2_comp = boost::get<int>(*it2); ++it2;

  if(syz_comp>0) {
    if(T2_comp>syz_comp && syz_comp>=T1_comp) {return true;}
    if(T1_comp>syz_comp && syz_comp>=T2_comp) {return false;}
  }

  if(T1.size()!=T2.size()) {throw std::runtime_error ("exponent vectors have different lengths ("+std::to_string(T1.size())+" and "+std::to_string(T2.size())+") in dp_larger_equal");}
  if(d1>d2) {return true;}
  if(d1<d2) {return false;}

  for(; it1 != T1.rend(); ++it1, ++it2)
  {
    int exp1 = boost::get<int>(*it1);
    int exp2 = boost::get<int>(*it2);
    if(exp1<exp2) {return true;}
    if(exp1>exp2) {return false;}
  }
  if(T1_comp < T2_comp) {return false;}
  if(T1_comp > T2_comp) {return true;}

  return (Qentry1.second.tie_break >= Qentry2.second.tie_break);
/*
  int i1 = Qentry1.first.first;
  int j1 = Qentry1.first.second;
  int i2 = Qentry2.first.first;
  int j2 = Qentry2.first.second;

  if (i1>i2 || (i1==i2 && j1>=j2)) return true;

  return false;
*/
}

inline bool posInL110_larger_equal(GpiList & Qentry1, GpiList & Qentry2) // deprecated! Still uses old list-format instead of the Qdata struct.
{
  //GpiList lcm1 = get_list(Qentry1.back());
  //GpiList lcm2 = get_list(Qentry2.back());
  int d1 =  boost::get<int> (*std::next(Qentry1.begin(),2));
  int d2 =  boost::get<int> (*std::next(Qentry2.begin(),2));
  int l1 =  boost::get<int> (*std::next(Qentry1.begin(),3));
  int l2 =  boost::get<int> (*std::next(Qentry2.begin(),3));
  GpiList lspoly1 = get_list(*std::next(Qentry1.begin(),4));
  GpiList lspoly2 = get_list(*std::next(Qentry2.begin(),4));
  //if(lcm1.size()!=lcm2.size()) {throw std::runtime_error ("exponent vectors have different lengths in posInL110_larger_equal");}
  if(lspoly1.size()!=lspoly2.size()) {throw std::runtime_error ("exponent vectors have different lengths in posInL110_larger_equal");}

  GpiList::const_reverse_iterator it1 = lspoly1.rbegin();
  GpiList::const_reverse_iterator it2 = lspoly2.rbegin();
  int lspoly1_comp = boost::get<int>(*it1); ++it1;
  int lspoly2_comp = boost::get<int>(*it2); ++it2;

  if(lspoly2_comp==-1) {return true;}
  if(lspoly1_comp==-1) {return false;}

  if(d1>d2) {return true;}
  if(d1<d2) {return false;}

  if(l1>l2) {return true;}
  for(; it1 != lspoly1.rend(); ++it1, ++it2)
  {
    int exp1 = boost::get<int>(*it1);
    int exp2 = boost::get<int>(*it2);
    if(exp1<exp2) {return true;}
    if(exp1>exp2) {return false;}
  }
  if(lspoly1_comp < lspoly2_comp) {return false;}

  return true;
}

/*
inline bool Q_smaller(std::pair<std::pair<int,int>,Qdata> & Qentry1, std::pair<std::pair<int,int>,Qdata> & Qentry2)
{
  return (!Q_larger_equal(Qentry1, Qentry2));
}

inline bool Q_larger(std::pair<std::pair<int,int>,Qdata> & Qentry1, std::pair<std::pair<int,int>,Qdata> & Qentry2)
{
  return (Q_smaller(Qentry2, Qentry1));
}
*/

struct QueueOrdering {
  long syz_comp;

  QueueOrdering(long syz_comp) : syz_comp(syz_comp) {}

  bool operator()(std::pair<std::pair<int,int>,Qdata> a, std::pair<std::pair<int,int>,Qdata> b) const {
      return !sel_strat_larger_equal(a, b, syz_comp); // "<", i.e. a comes first w.r.t. Singulars s-pair selection strategy
  }
};

using sPairQueue = PriorityQueue<std::pair<int,int>,Qdata,QueueOrdering>;
// keys:    s-pair indices (i,j) where i<j
// values:  struct of necessary information to sort s-pairs (degree, lead monomial, length, etc.)
// Compare: ordering of s-polnomials, should be the same as in Singular (has access to keys AND values)


// helper functions for handling lead monomials (represented as exponent vectors) ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
  for(; it1 != m1.end(); ++it1, ++it2)
  {
    if(*it1>*it2) {return false;}
  }
  return true;
}

inline bool coprime_monom(std::vector<int> const& m1, std::vector<int> const& m2)
{
  //if (m1.back()!=m2.back()) {return false;} // different components
  std::vector<int>::const_reverse_iterator it1 = m1.rbegin(); ++it1;
  std::vector<int>::const_reverse_iterator it2 = m2.rbegin(); ++it2;
  std::vector<int> res;
  for(; it1 != m1.rend(); ++it1, ++it2)
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

inline bool over_deg_bound(std::vector<int> const& monom, GpiList const& degBounds)
{
  long i=0;
  for(GpiList::const_iterator it=degBounds.begin(); it!=degBounds.end(); ++it)
  {
    long degBound = boost::get<long>(get_list(*it).front());
    long nvars    = boost::get<long>(get_list(*it).back());
    long end_i    = i+nvars;

    if (degBound==0)
    {
      i = end_i;
      continue;
    }

    int degree=0;
    for(; i<end_i; i++)
    {
      degree += monom[i];
    }

    if (degree > degBound) {return true;}
  }
  return false;
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


// std::vector<int> <--> GpiList /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline GpiList vec2list(std::vector<int> vec)
{
  GpiList L;
  for (std::vector<int>::iterator it=vec.begin(); it!=vec.end(); ++it)
  {
    L.emplace_back(*it);
  }
  return L;
}

inline GpiList vec2list(std::vector<std::vector<int>> vec)
{
  GpiList L;
  for (std::vector<std::vector<int>>::iterator it=vec.begin(); it!=vec.end(); ++it)
  {
    L.emplace_back(vec2list(*it));
  }
  return L;
}

inline GpiList vec2list(std::vector<std::vector<std::vector<int> > > vec)
{
  GpiList L;
  for (std::vector<std::vector<std::vector<int> > >::iterator it=vec.begin(); it!=vec.end(); ++it)
  {
    L.emplace_back(vec2list(*it));
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

inline GpiList lead_of_spoly(std::vector<std::vector<int>> const&  Mi, std::vector<std::vector<int>> const&  Mj, std::vector<int> const& lcm)
{
  int nvars = Mi[0].size()-1;
  std::vector<int> Li;
  std::vector<int> Lj;
  int di=0;
  int dj=0;
  int compi = Mi[1][nvars];
  int compj = Mj[1][nvars];

  for(int k=0; k<nvars; k++)
  {
    int ei = lcm[k]-Mi[0][k]+Mi[1][k];
    Li.emplace_back(ei);
    di += ei;
  }
  Li.emplace_back(compi);

  for(int k=0; k<nvars; k++)
  {
    int ej = lcm[k]-Mj[0][k]+Mj[1][k];
    Lj.emplace_back(ej);
    dj += ej;
  }
  Lj.emplace_back(compj);

  // compare (just always using dp for now!)
  if (compi==-1 && compj>=0 ) {return vec2list(Lj);} // component==-1 if there is no second term!
  if (compi>=0  && compj==-1) {return vec2list(Li);}
  if (compi==-1 && compj==-1) {return vec2list(Mi[1]);} // return 0
  bool bigger = false; // "Li > Lj"
  if(di>dj) {bigger=true;}
  else {
    if(di==dj) {
      for(int k=nvars-1; k>=0; k--) {
        if (Li[k]<Lj[k]) {
          bigger=true;
          break;
        }
      }
    }
  }
  if(bigger) {return vec2list(Li);}
  return vec2list(Lj);
}


// Queue operations //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void queue_insert(sPairQueue& Q, int i, int j, int old_r, std::vector<std::vector<int>> const&  Mi, std::vector<std::vector<int>> const&  Mj, std::vector<int> const& lcm_vec, bool PC=false)
{
  //FIX: std::ofstream ijFile(base_filename+"queue/started/"+std::to_string(i)+"_"+std::to_string(j));
  //FIX: ijFile << (int) 0; // initialize old_r
  //FIX: ijFile.close();

  int deg_lcm = deg(lcm_vec);
  GpiList lcm = vec2list(lcm_vec);

  //std::cout << "inserting element with lcm " << std::endl;
  //print_variant(lcm);

  GpiList l_spoly = lead_of_spoly(Mi,Mj,lcm_vec);

  unsigned long tie_break = Q.inserts();

  //GpiList data = {i, j, deg_lcm, (int) 0, l_spoly, tie_break, lcm};
  Qdata data = {i, j, old_r, deg_lcm, (int) 0, l_spoly, lcm, tie_break, GpiList({}), PC};
  //QUEUE std::cout << "  queue_insert before: test_key=" << Q.test_key(std::make_pair(i,j)) << ", test_key_value=" << Q.test_key_value(std::make_pair(std::make_pair(i,j),data)) << std::endl;
  Q.push(std::make_pair(i,j),data);
  //QUEUE std::cout << "  queue_insert after:  test_key=" << Q.test_key(std::make_pair(i,j)) << ", test_key_value=" << Q.test_key_value(std::make_pair(std::make_pair(i,j),data)) << std::endl;
}

inline sPairQueue::iterator queue_mark_paused_i_j(sPairQueue& Q, int i, int j, GpiVariant NF) // remove index (i,j) from Q
{
  GpiList new_lead = get_list(peek("lead_data",NF)->get());
	int old_r = boost::get<int>(peek("old_r",NF)->get());
  #ifdef DEBUG_BBA
  size_t Qs = Q.size();
  std::cout << "queue_mark_paused_i_j (" << i << "," << j << "), size="<<Qs<<"\n";
  #endif
  std::pair<int,int> indices = std::make_pair(std::min(i,j),std::max(i,j));
  //FIX: std::ofstream ijFile(base_filename+"queue/started/"+std::to_string(std::min(i,j))+"_"+std::to_string(std::max(i,j)));
  //FIX: ijFile << old_r;
  //FIX: ijFile.close();

  //QUEUE std::cout << "  queue_mark_paused_i_j before: test_key=" << Q.test_key(indices) << std::endl;
  sPairQueue::iterator itQ = Q.find(indices);
  if (itQ!=Q.end()) {
    (itQ->second).old_r = old_r;
    (itQ->second).new_lead = new_lead;
  }
  //QUEUE std::cout << "  queue_mark_paused_i_j after:  test_key=" << Q.test_key(indices) << std::endl;
  #ifdef DEBUG_BBA
  std::cout << "queue size in queue_mark_paused_i_j: "<<Qs<<" ---> "<<Q.size()<<"\n";
  #endif

  return itQ;
}

inline sPairQueue::iterator queue_delete_i_j(sPairQueue& Q, int i, int j, [[maybe_unused]] std::string to_filename, int* nsyzpairs, int i_is_syzygy, bool* deleted=NULL) // remove index (i,j) from Q
{
  #ifdef DEBUG_BBA
  size_t Qs = Q.size();
  std::cout << "queue_delete_i_j (" << i << "," << j << "), size="<<Qs<<"\n";
  #endif
  std::pair<int,int> indices = std::make_pair(std::min(i,j),std::max(i,j));

  //FIX: std::remove((base_filename+"queue/started/"+std::to_string(std::min(i,j))+"_"+std::to_string(std::max(i,j))).c_str());
  //FIX: std::ofstream ijFile(base_filename+"queue/"+to_filename+"/"+std::to_string(std::min(i,j))+"_"+std::to_string(std::max(i,j))); ijFile.close();

  sPairQueue::iterator itQ = Q.end();
  if (Q.contains_key(indices)) {
    if (deleted!=NULL) {*deleted=true;}
    (*nsyzpairs) -= i_is_syzygy; // if i is a syzygy, then all pairs (i,k) are syzygy pairs and we must decrease the counter
    //QUEUE std::cout << "  queue_delete_i_j before: test_key=" << Q.test_key(indices) << std::endl;
    itQ = Q.erase(indices);
    //QUEUE std::cout << "  queue_delete_i_j after:  test_key=" << Q.test_key(indices) << std::endl;
  }
  else {
    if (deleted!=NULL) {*deleted=false;}
  }
  //Q.erase(Qind[indices]);
  //Qind.erase(indices);
  #ifdef DEBUG_BBA
  std::cout << "queue size in queue_delete_i_j: "<<Qs<<" ---> "<<Q.size()<<"\n";
  #endif

  return itQ;
}

inline void queue_delete_i(sPairQueue& Q, int i, int r, [[maybe_unused]] std::string to_filename, int* nsyzpairs, int i_is_syzygy) // remove indices (i,j) and (j,i) from Q (for all j)
{
  #ifdef DEBUG_BBA
  size_t Qs = Q.size();
  std::cout << "queue_delete_i (" << i << ")\n";
  #endif
  for (int k=1; k<=r; k++)
  {
    std::pair<int,int> indices = std::make_pair(std::min(i,k),std::max(i,k));
    if(Q.contains_key(indices))
    {
      //std::cout << "\n("<<std::min(i,k)<<","<<std::max(i,k)<<")" << " == " << "("<< boost::get<int>((*Qind[indices]).front()) <<","<< boost::get<int>(*std::next((*Qind[indices]).begin())) <<")" << "\n";
      //GpiList data = Q.get_value(indices);// *(Qind[indices]);
      if (Q.contains_key(indices)) {
        (*nsyzpairs) -= i_is_syzygy; // if i is a syzygy, then all pairs (i,k) are syzygy pairs and we must decrease the counter
        //QUEUE std::cout << " queue_delete_i  before: test_key=" << Q.test_key(indices) << std::endl;
        Q.erase(indices);
        //QUEUE std::cout << " queue_delete_i  after:  test_key=" << Q.test_key(indices) << std::endl; 
      }
      //FIX: std::remove((base_filename+"queue/started/"+std::to_string(std::min(i,k))+"_"+std::to_string(std::max(i,k))).c_str());
      //FIX: std::ofstream ijFile (base_filename+"queue/"+to_filename+"/"+std::to_string(std::min(i,k))+"_"+std::to_string(std::max(i,k))); ijFile.close();
    }
  }

  #ifdef DEBUG_BBA
  std::cout << "queue size in queue_delete_i: "<<Qs<<" ---> "<<Q.size()<<"\n";
  #endif
}

inline void serialize_queue(sPairQueue Q , std::string filenameQ, int nvars)
{

  std::ofstream FileQ(filenameQ);

  FileQ << Q.get_syz_comp() << '\n';
  FileQ << nvars << '\n';
  FileQ << Q.size() << '\n';
  //std::cout << "\nSERIALIZING QUEUE..." << std::endl;
  //std::cout << "nvars: " << nvars << std::endl;
  //std::cout << "size: " << Q.size() << std::endl;

  for(sPairQueue::iterator Qit=Q.begin(); Qit!=Q.end(); ++Qit)
  {
    Qdata entry = (*Qit).second;

    FileQ << entry.i       << '\n';
    FileQ << entry.j       << '\n';
    FileQ << entry.old_r   << '\n'; //added!
    FileQ << entry.deg_lcm << '\n';
    FileQ << entry.length  << '\n';

    for(GpiList::iterator Lit=entry.l_spoly.begin(); Lit!=entry.l_spoly.end(); ++Lit) {
      FileQ << boost::get<int>(*Lit) << '\n';
    }

    for(GpiList::iterator Lit=entry.lcm.begin(); Lit!=entry.lcm.end(); ++Lit) {
      FileQ << boost::get<int>(*Lit) << '\n';
    }

    FileQ << entry.tie_break << '\n';
    FileQ << entry.PC << '\n';
  }
  FileQ.close();
}

inline sPairQueue deserialize_queue(std::string filenameQ)
{
  std::ifstream FileQ(filenameQ);
  std::string currLine;

  //std::cout << "\nDESERIALIZING QUEUE..." << std::endl;
  std::getline(FileQ, currLine);
  long syz_comp = std::stol(currLine);

  std::getline(FileQ, currLine);
  int nvars = std::stoi(currLine);
  //std::cout << "nvars: " << nvars << std::endl;

  std::getline(FileQ, currLine);
  int sizeQ = std::stoi(currLine);
  //std::cout << "size: " << sizeQ << std::endl;

  sPairQueue Q(syz_comp);

  //sPairQueue_by_indices Qind;
  for(int k=0; k<sizeQ; k++)
  {
    std::getline(FileQ, currLine);
    int i = std::stoi(currLine); //i
    //std::cout << i << '\n';
    std::getline(FileQ, currLine);
    int j = std::stoi(currLine); //j
    //std::cout << j << '\n';
    std::getline(FileQ, currLine);
    int old_r = std::stoi(currLine); //old_r
    //std::cout << old_r << '\n';
    std::getline(FileQ, currLine);
    int deg_lcm = std::stoi(currLine); //deg_lcm
    //std::cout << currLine << '\n';
    std::getline(FileQ, currLine);
    int length = std::stoi(currLine); //length
    //std::cout << currLine << '\n';

    GpiList l_spoly;
    for(int kk=0; kk<=nvars; kk++) // "smaller equal" since last entry will be the component!
    {
      std::getline(FileQ, currLine);
      l_spoly.emplace_back(std::stoi(currLine));
    }

    GpiList lcm;
    for(int kk=0; kk<=nvars; kk++) // "smaller equal" since last entrywill be the component!
    {
      std::getline(FileQ, currLine);
      lcm.emplace_back(std::stoi(currLine));
    }

    std::getline(FileQ, currLine);
    unsigned long tie_break = std::stoul(currLine); //tie_break

    std::getline(FileQ, currLine);
    bool PC = std::stoi(currLine)!=0; //PC true ==> just construct the syzygy

    Qdata entry = {i, j, old_r, deg_lcm, length, l_spoly, lcm, tie_break, GpiList({}), PC};
    Q.push(std::make_pair(i,j),entry);
  }

  FileQ.close();

  return Q;
}


// for debugging /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void displayQ(sPairQueue Q, std::ofstream& debugfile) // for debugging
{
  for (sPairQueue::const_iterator itQ = Q.begin(); itQ!=Q.end(); ++itQ)
  {
    int index_i = (itQ->first ).first;
    int index_j = (itQ->first ).second;
    int old_r   = (itQ->second).old_r;
    #ifdef DEBUG_BBA
    std::cout << "    ("<< index_i<<","<<index_j<<","<<old_r<<")" << std::endl;
    #endif
    debugfile << "("<< index_i<<","<<index_j<<","<<old_r<<") ";
  }
}

inline std::string joblist_to_string(GpiList L)
{
  std::string s = "";
  for (GpiList::const_iterator it=L.begin(); it!=L.end(); ++it) {
    if (it->which()==TYPE_ID(GpiSet)) {continue;}
    GpiList entry = get_list(*it);
    GpiList::const_iterator it2 = entry.begin();
    int i     = boost::get<int>(*it2); ++it2;
    int j     = boost::get<int>(*it2); ++it2;
    s = s+"("+std::to_string(i)+","+std::to_string(j);
    if (it2!=entry.end()) {
      int old_r = boost::get<int>(*it2);
      s = s+","+std::to_string(old_r);
    }
    s = s +") ";
  }
  return s;
}

inline std::string joblist_to_string(GpiSet L)
{
  std::string s = "";
  for (GpiSet::const_iterator it=L.begin(); it!=L.end(); ++it) {
    GpiList entry = get_list(*it);
    GpiList::const_iterator it2 = entry.begin();
    int i     = boost::get<int>(*it2); ++it2;
    int j     = boost::get<int>(*it2); ++it2;
    s = s+"("+std::to_string(i)+","+std::to_string(j);
    if (it2!=entry.end()) {
      int old_r = boost::get<int>(*it2);
      s = s+","+std::to_string(old_r);
    }
    s = s +") ";
  }
  return s;
}


// Product and Chain Criterion ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline int test_PC(std::vector<int> const& Mi, std::vector<int> const& Mj, int rank)
{
  int Mi_comp = Mi.back();
  int Mj_comp = Mj.back();
  if(Mi_comp==0 || (Mi_comp==1 && Mj_comp==1 && rank==1)) // ideals & polynomials (also rank-1 modules)
  {
    //std::cout << "COMPONENT 0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    return coprime_monom(Mi, Mj) ? 1 : 0; // product-criterion: if mi and mj are coprime then (i,j) can be discarded
  }
  else // modules & vectors
  {
    return (Mi_comp!=Mj_comp) ? 2 : 0; // for modules: test instead if the leads lie in different components (thus spoly=0 trivially)
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




NO_NAME_MANGLING
void singular_buchberger_compute(std::string const& singular_library_name,
																 std::string const& singular_function_name,
															 	 std::string const& base_filename,
																 std::vector<boost::variant<long,std::string,GpiList,GpiBigint>> const& args_read,
																 std::vector<boost::variant<long,std::string,GpiList,GpiBigint>> const& args_in,
																 std::vector<boost::variant<long*,std::string*,GpiList*,GpiBigint*>> &args_inout,
																 std::vector<boost::variant<long*,std::string*,GpiList*,GpiBigint*>> &out,
																 std::vector<GpiList*> &out_many,
															 	 bool delete_files,
                                 bool silent);

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
                   GpiMap* runtime);

NO_NAME_MANGLING
void singular_buchberger_compute_NF(std::string const& base_filename,
                                    std::list<poly> const& generators,
                                    int r,
                                    int index_i,
                                    int index_j,
                                    int old_r,
                                    int syzygy,
                                    long syz_comp,
                                    long red_syz,
                                    GpiMap* runtime,
                                    GpiList* NF);

NO_NAME_MANGLING
poly read_generator (std::string const& base_filename,
                     int k);

NO_NAME_MANGLING
void singular_buchberger_reduce_GB(std::string const& base_filename,
                                   std::list<poly> const& generators,
                                   int generator_name,
                                   int generator_index,
                                   int save_index,
                                   int is_syzygy,
                                   int ngens,
                                   long syz_comp,
                                   long red_syz,
                                   GpiMap* runtime);

NO_NAME_MANGLING
void tail_reduce(std::list<poly>  * generators,
                 std::string const& from_filename,
                 std::string const& to_filename);
