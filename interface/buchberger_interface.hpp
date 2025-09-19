#pragma once

#define DEBUG_BBA

#define USE_KNF true
#define HEAD_SIZE_FACTOR 5

#define TRACE 0

#define SLEEP_MS(t) std::this_thread::sleep_for( std::chrono::milliseconds(t) );

#define NO_NAME_MANGLING extern "C"

#define sel_strat_larger_equal dp_larger_equal
#define Q_larger_equal         dp_larger_equal
//#define sel_strat_larger_equal posInL110_larger_equal
//#define Q_larger_equal         posInL110_larger_equal

#include <string>
#include <config.hpp>
#include <boost/variant.hpp>
//#include <vector>

#include <iostream>
#include <fstream>

#include <we/type/literal/control.hpp>
#include <we/type/bitsetofint.hpp>
#include <we/type/bytearray.hpp>
#include <we/type/value.hpp>
#include <map>

#include <vector>
#include <unordered_map>
#include <algorithm>

#include <cassert>

#define CONTROL_TOKEN we::type::literal::control{}

/*
#define RESOLVE_INTERFACE_FUNCTION(function) \
      (fhg::util::scoped_dlhandle \
      (config::implementation(), \
      RTLD_GLOBAL | RTLD_NOW | RTLD_DEEPBIND) \
      .sym<decltype(function)> \
      (BOOST_PP_STRINGIZE(function)))
*/
#define RESOLVE_INTERFACE_FUNCTION(function) \
    (fhg::util::scoped_dlhandle \
    (boost::filesystem::path(config::implementation().string()), \
    RTLD_GLOBAL | RTLD_NOW | RTLD_DEEPBIND) \
    .sym<decltype(function)> \
    (BOOST_PP_STRINGIZE(function)))





// Priority queue with delete: //!!!

// iterable container holding elements of type pair<K,V> sorted with respect to Compare with logarithmic insertion/removal and average constant time access by a key (of type K)
template<typename K, typename V, typename Compare>
class PriorityQueue {
private:
    using SetType = std::set<std::pair<K,V>, Compare>;
    using MapType = std::unordered_map<K,typename SetType::iterator>;

    SetType entries;
    Compare comp; // = "is-left-of"
    std::size_t head_size;
    typename SetType::iterator head_end;
    MapType references;
public:
    PriorityQueue(std::size_t head_size)
    : entries(Compare()), comp(Compare()), head_size(head_size), head_end(entries.end()) {}

    using iterator = typename SetType::iterator;
    using const_iterator = typename SetType::const_iterator;

    iterator begin() { return entries.begin(); }
    iterator end()   { return entries.end(); }
    iterator hend()  { return head_end; }

    const_iterator begin() const { return entries.begin(); }
    const_iterator end()   const { return entries.end(); }
    const_iterator hend()  const { return head_end; }

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

    bool is_in_head(const std::pair<K,V>& key_value)
      {return comp(key_value, *head_end);}
    bool is_in_head(const K& key)
      {return comp(*(references[key]), *head_end);}

    std::pair<iterator,bool> push(const std::pair<K,V>& key_value) {
      std::pair<iterator,bool> res = entries.insert(key_value);
      if (res.second) {
        references[key_value.first] = res.first;
        if(entries.size()>head_size && (head_end==entries.end() || comp(key_value, *head_end))) {--head_end;}
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
      if(entries.size()>head_size && (comp(*entry, *head_end) || entry == head_end)) {++head_end;}

      iterator next = entries.erase(entry);
      references.erase(it);


      return next;
    }
    iterator erase(iterator it) {
      if (it == entries.end()) return end();
      K key = (*it).first;

      if(entries.size()>head_size && (comp(*it, *head_end) || it == head_end)) {++head_end;}

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

    size_t get_head_size()
      {return head_size;}
    /*
    void display() {
      std::cout << "\nhead_size="<<head_size << std::endl;
      std::cout << "entries.size()="<<entries.size()<<", references.size()="<<references.size() << std::endl;
      std::cout << "elements:" << std::endl;
      for (iterator it=begin(); it!=end(); ++it)
      {
        std::string s="";
        if (it==head_end) s=" <-------- head_end";
        std::cout << "  " << it->first << ", " << it->second << s << std::endl;
        if (references.find(it->first)==references.end())
          std::cout << "  NOT IN REFERENCES!" << std::endl;
      }
      std::string s="";
      if (entries.end()==head_end) s=" <-------- head_end";
        std::cout << "  end" << s << std::endl;
      std::cout << "" << std::endl;
    }
    */
};

namespace std {
    template<>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& p) const noexcept {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };
}


/* OLD VERSION
template<typename K, typename V>
class PriorityQueue {
private:
    std::vector<std::pair<K,V>> heap;
    std::unordered_map<K, size_t> positions;

    // Helper to swap elements and update positions
    void swap_positions(size_t i, size_t j) {
        std::swap(heap[i], heap[j]);
        positions[heap[i].first] = i;
        positions[heap[j].first] = j;
    }

    // Heapify-up after insertion //!! use std::make_heap?    also: flat_map, flat_set instead of map, set? --> no dynamic allocation
    void heapify_up(size_t index) {
        while (index != 0 && Q_smaller(heap[parent(index)] , heap[index])) {
            swap_positions(index, parent(index));
            index = parent(index);
        }
    }

    // Heapify-down after deletion
    void heapify_down(size_t index) {
        size_t left = left_child(index);
        size_t right = right_child(index);
        size_t largest = index;

        if (left  < heap.size() && Q_larger(heap[left]  , heap[largest])) largest = left;
        if (right < heap.size() && Q_larger(heap[right] , heap[largest])) largest = right;

        if (largest != index) {
            swap_positions(index, largest);
            heapify_down(largest);
        }
    }

    size_t parent(size_t index) { return (index - 1) / 2; }
    size_t left_child(size_t index) { return 2 * index + 1; }
    size_t right_child(size_t index) { return 2 * index + 2; }

public:
    PriorityQueue() = default;

    bool empty() const { return heap.empty(); }

    void push(const std::pair<K,V>& value) {
        heap.push_back(value);
        size_t index = heap.size() - 1;
        positions[value.first] = index;
        heapify_up(index);
    }

    void push(const K& key, const V& value) {
        push(std::make_pair(key,value));
    }

    void pop() {
        if (heap.empty()) return;

        positions.erase(heap[0].first);
        heap[0] = heap.back();
        heap.pop_back();

        if (!heap.empty()) {
            positions[heap[0].first] = 0;
            heapify_down(0);
        }
    }

    const std::pair<K,V>& top() const {
        if (heap.empty()) throw std::runtime_error("PriorityQueue is empty");
        return heap[0];
    }

    const std::pair<K,V>& get(const K& key) const {
      auto it = positions.find(key);
      if (it == positions.end()) throw std::runtime_error("Element not in queue");
      return heap[it->second];
    }

    const std::pair<K,V>& get_value(const K& key) const {
      return get(key).second;
    }

    bool remove(const K& key) {
        auto it = positions.find(key);
        if (it == positions.end()) return false;

        size_t index = it->second;
        positions.erase(it);

        heap[index] = heap.back();
        heap.pop_back();

        if (index < heap.size()) {
            positions[heap[index].first] = index;
            heapify_down(index);
            heapify_up(index);
        }
        return true;
    }
};
*/



//extern int Kstd1_deg;


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

//inline bool dp_larger_equal(GpiList const& T1, GpiList const& T2, int d1, int d2)
inline bool dp_larger_equal(GpiList const& Qentry1, GpiList const& Qentry2)
{
  GpiList T1 = get_list(Qentry1.back());
  GpiList T2 = get_list(Qentry2.back());
  int d1 = boost::get<int>(*std::next(Qentry1.begin(),2));
  int d2 = boost::get<int>(*std::next(Qentry2.begin(),2));
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
  if(T1_comp < T2_comp) {return false;}

  return true;
}




inline bool posInL110_larger_equal(GpiList & Qentry1, GpiList & Qentry2)
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

inline bool Q_smaller(GpiList & Qentry1, GpiList & Qentry2)
{
  return (!Q_larger_equal(Qentry1, Qentry2));
}

inline bool Q_larger(GpiList & Qentry1, GpiList & Qentry2)
{
  return (Q_smaller(Qentry2, Qentry1));
}

struct QueueOrdering {
    bool operator()(std::pair<std::pair<int,int>,GpiList> a, std::pair<std::pair<int,int>,GpiList> b) const {
        return !sel_strat_larger_equal(a.second, b.second); // "<", i.e. a comes first w.r.t. Singulars s-pair selection strategy
    }
};


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


// Product and Chain Criterion

inline bool test_PC(std::vector<int> const& Mi, std::vector<int> const& Mj)
{
  int Mi_comp = Mi.back();
  int Mj_comp = Mj.back();
  if(Mi_comp==0) // ideals & polynomials
  {
    std::cout << "COMPONENT 0 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
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

// helper functions for the queue (using a "static" queue)

/*
struct Descending {
    bool operator()(GpiList a, GpiList b) const {
        return sel_strat_larger_equal(a, b); // descending order
    }
};
*/

using sPairQueue = PriorityQueue<std::pair<int,int>,GpiList,QueueOrdering>;
// keys:    s-pair indices (i,j) where i<j
// values:  list of necessary information to sort s-pairs (degree, lead monomial, length, etc.)
// Compare: ordering of s-polnomials, should be the same as in Singular (has access to keys AND values)

//using sPairQueue = PriorityQueue<std::pair<int,int>,GpiList>; //std::set<GpiList, Descending>;
//using sPairQueue_by_indices = std::map<std::pair<int,int>,sPairQueue::iterator>;

inline void queue_insert(sPairQueue& Q, int i, int j, std::vector<std::vector<int>> const&  Mi, std::vector<std::vector<int>> const&  Mj, std::vector<int> const& lcm_vec, std::string base_filename, [[maybe_unused]] int state)
{
  std::ofstream ijFile(base_filename+"queue/started/"+std::to_string(i)+"_"+std::to_string(j));
  ijFile.close();

  int deg_lcm = deg(lcm_vec);
  GpiList lcm = vec2list(lcm_vec);

  GpiList l_spoly = lead_of_spoly(Mi,Mj,lcm_vec);

  GpiList data = {i, j, deg_lcm, (int) 0, l_spoly, lcm};
  Q.push(std::make_pair(i,j),data);
}

inline sPairQueue::iterator queue_delete_i_j(sPairQueue& Q, int i, int j, std::string base_filename, std::string to_filename, int* nrunning) // remove index (i,j) from Q
{
  #ifdef DEBUG_BBA
  size_t Qs = Q.size();
  std::cout << "queue_delete_i_j (" << i << "," << j << "), size="<<Qs<<"\n";
  #endif
  std::pair<int,int> indices = std::make_pair(std::min(i,j),std::max(i,j));
  std::remove((base_filename+"queue/started/"+std::to_string(std::min(i,j))+"_"+std::to_string(std::max(i,j))).c_str());
  std::ofstream ijFile(base_filename+"queue/"+to_filename+"/"+std::to_string(std::min(i,j))+"_"+std::to_string(std::max(i,j)));
  ijFile.close();

  //std::pair<int,int> indices = std::make_pair(std::min(i,j),std::max(i,j));
  //std::cout << "\n("<<std::min(i,j)<<","<<std::max(i,j)<<")" << " == " << "("<< boost::get<int>((*Qind[indices]).front()) <<","<< boost::get<int>(*std::next((*Qind[indices]).begin())) <<")" << "\n";
  //std::cout << "\n(which, size)" << " == " << "("<< (*Qind[indices]).front().which() <<","<< (*Qind[indices]).size() <<")" << "\n";
  //GpiList data = Q.get(std::make_pair(i,j));// *(Qind[indices]);
  sPairQueue::iterator itQ = Q.end();
  if (Q.contains_key(indices)) {
    (*nrunning)--;
    itQ = Q.erase(indices);
  }
  //Q.erase(Qind[indices]);
  //Qind.erase(indices);
	#ifdef DEBUG_BBA
  std::cout << "queue size in queue_delete_i_j: "<<Qs<<" ---> "<<Q.size()<<"\n";
  #endif

  return itQ;
}

inline void queue_delete_i(sPairQueue& Q, int i, int r, std::string base_filename, std::string to_filename, int* nrunning) // remove indices (i,j) and (j,i) from Q (for all j)
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
      //if (boost::get<int>(data.back())==1) {(*nrunning)--;}
      if (Q.contains_key(indices)) {
        (*nrunning)--;
        Q.erase(indices);
      }
      std::remove((base_filename+"queue/started/"+std::to_string(std::min(i,k))+"_"+std::to_string(std::max(i,k))).c_str());
      std::ofstream ijFile (base_filename+"queue/"+to_filename+"/"+std::to_string(std::min(i,k))+"_"+std::to_string(std::max(i,k)));
      ijFile.close();
    }
  }

  #ifdef DEBUG_BBA
  std::cout << "queue size in queue_delete_i: "<<Qs<<" ---> "<<Q.size()<<"\n";
  #endif
}
/*
inline void fix_Q(sPairQueue& Q , sPairQueue_by_indices& Qind, int* nrunning)
{
  (*nrunning) = 0;
  for(sPairQueue::iterator it=Q.begin(); it!=Q.end(); ++it)
  {
    int i = boost::get<int>((*it).front());
    int j = boost::get<int>(*std::next((*it).begin()));
    Qind[std::make_pair(i,j)] = it;
    if(boost::get<int>((*it).back())==1)
    {
      (*nrunning)++;
    }
  }
}
*/
inline void serialize_queue(sPairQueue Q , std::string filenameQ, int nvars)
{
	std::ofstream FileQ(filenameQ);

  FileQ << nvars << '\n';
  FileQ << Q.size() << '\n';
  FileQ << (int) Q.get_head_size() << '\n';
  #ifdef DEBUG_BBA
  std::cout << "head_size (serialize): " << Q.get_head_size() << std::endl;
  std::cout << "head_size (serialize, int): " << (int) Q.get_head_size() << std::endl;
  #endif
  //std::cout << "\nSERIALIZING QUEUE..." << std::endl;
  //std::cout << "nvars: " << nvars << std::endl;
  //std::cout << "size: " << Q.size() << std::endl;

  for(sPairQueue::iterator Qit=Q.begin(); Qit!=Q.end(); ++Qit)
  {
    GpiList::const_iterator entry = (*Qit).second.begin();


    //std::cout << ' ' << std::endl;
    //std::cout << boost::get<int>(*entry) << std::endl;
    FileQ << boost::get<int>(*entry) << '\n'; ++entry; // i
    //std::cout << boost::get<int>(*entry) << std::endl;
    FileQ << boost::get<int>(*entry) << '\n'; ++entry; // j
    //std::cout << boost::get<int>(*entry) << std::endl;
    FileQ << boost::get<int>(*entry) << '\n'; ++entry; // deg_lcm
    //std::cout << boost::get<int>(*entry) << std::endl;
    FileQ << boost::get<int>(*entry) << '\n'; ++entry; // length

    //print_variant(*entry);
    GpiList l_spoly = get_list(*entry); ++entry;
    for(GpiList::iterator Lit=l_spoly.begin(); Lit!=l_spoly.end(); ++Lit) {
      //std::cout << " l_spoly " << (*Lit).which() << std::endl;
      FileQ << boost::get<int>(*Lit) << '\n';
    }

    //print_variant(*entry);
    GpiList lcm = get_list(*entry); ++entry;
    for(GpiList::iterator Lit=lcm.begin(); Lit!=lcm.end(); ++Lit) {
      //std::cout << " lcm " << (*Lit).which() << std::endl;
      FileQ << boost::get<int>(*Lit) << '\n';
    }

    //FileQ << boost::get<int>(*entry) << '\n'; // status
  }
	FileQ.close();
}

inline sPairQueue deserialize_queue(std::string filenameQ)
{
  std::ifstream FileQ(filenameQ);
  std::string currLine;

  std::cout << "\nDESERIALIZING QUEUE..." << std::endl;
  std::getline(FileQ, currLine);
  int nvars = std::stoi(currLine);
  std::cout << "nvars: " << nvars << std::endl;

  std::getline(FileQ, currLine);
  int sizeQ = std::stoi(currLine);
  std::cout << "size: " << sizeQ << std::endl;

  std::getline(FileQ, currLine);
  int head_size = std::stoi(currLine);
  std::cout << "head_size (deserialize): " << head_size << std::endl;

  sPairQueue Q(head_size);

  //sPairQueue_by_indices Qind;
  for(int k=0; k<sizeQ; k++)
  {
    GpiList data; //{i, j, deg_lcm, length, l_spoly, lcm};
    std::getline(FileQ, currLine);
    int i = std::stoi(currLine); data.emplace_back(i); //i
    //std::cout << i << '\n';
    std::getline(FileQ, currLine);
    int j = std::stoi(currLine); data.emplace_back(j); //j
    //std::cout << j << '\n';
    std::getline(FileQ, currLine);
    data.emplace_back(std::stoi(currLine)); //deg_lcm
    //std::cout << currLine << '\n';
    std::getline(FileQ, currLine);
    data.emplace_back(std::stoi(currLine)); //length
    //std::cout << currLine << '\n';

    GpiList l_spoly;
    for(int kk=0; kk<nvars; kk++)
    {
      std::getline(FileQ, currLine);
      l_spoly.emplace_back(std::stoi(currLine));
    }
    data.emplace_back(l_spoly);

    GpiList lcm;
    for(int kk=0; kk<nvars; kk++)
    {
      std::getline(FileQ, currLine);
      lcm.emplace_back(std::stoi(currLine));
    }
    data.emplace_back(lcm);

    //std::getline(FileQ, currLine);
    //data.emplace_back(std::stoi(currLine)); //status

    Q.push(std::make_pair(i,j),data);
    //Qind[std::make_pair(i,j)] = (--Q.end());
    //Qind[std::make_pair(i,j)] = res.first;
    //std::cout << "("<<i<<","<<j<<")" << " == " << "("<< boost::get<int>((*Qind[std::make_pair(i,j)]).front()) <<","<< boost::get<int>(*std::next((*Qind[std::make_pair(i,j)]).begin())) <<")" << "\n\n";
  }
  //std::cout << " TEST: ("<<11<<","<<13<<")" << " == " << "("<< boost::get<int>((*Qind[std::make_pair(11,13)]).front()) <<","<< boost::get<int>(*std::next((*Qind[std::make_pair(11,13)]).begin())) <<")" << "\n\n";
  //print_variant(*Qind[std::make_pair(11,13)]);
  FileQ.close();

  return Q;
}


// helper functions for the queue (deprecated)

/*
inline void queue_insert(GpiList& Q, int i, int j, std::vector<std::vector<int>> const&  Mi, std::vector<std::vector<int>> const&  Mj, std::vector<int> const& lcm_vec, std::string base_filename)
{
  std::ofstream ijFile(base_filename+"queue/started/"+std::to_string(i)+"_"+std::to_string(j));
  ijFile.close();

  int deg_lcm = deg(lcm_vec);
  GpiList lcm = vec2list(lcm_vec);

  GpiList l_spoly = lead_of_spoly(Mi,Mj,lcm_vec);

  GpiList data = {i, j, deg_lcm, (int) 0, l_spoly, lcm};
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
    //if(!sel_strat_larger_equal(get_list(entry.back()), lcm, boost::get<int>(*std::next(entry.begin(),2)), deg_lcm))
    if(!sel_strat_larger_equal(entry, data))
    {
      Q.push(it, data);
      return;
    }
  }
  Q.push_back(data);
}

inline void queue_delete_i_j(GpiList& Q, int i, int j, std::string base_filename, std::string to_filename) // remove index (i,j) from Q
{
  for (GpiList::iterator Qk=Q.begin(); Qk!=Q.end(); )
  {
    int ii = boost::get<int>(get_list(*Qk).front());
    int jj = boost::get<int>(*std::next(get_list(*Qk).begin()));
    if(ii==i && jj==j)
    {
      Qk = Q.erase(Qk);
      std::remove((base_filename+"queue/started/"+std::to_string(std::min(ii,jj))+"_"+std::to_string(std::max(ii,jj))).c_str());
      std::ofstream ijFile(base_filename+"queue/"+to_filename+"/"+std::to_string(std::min(ii,jj))+"_"+std::to_string(std::max(ii,jj)));
      std::cout << "writing file " << base_filename+to_filename+"/"+std::to_string(std::min(ii,jj))+"_"+std::to_string(std::max(ii,jj)) << std::endl;
      ijFile.close();
      break;
    }
    else
    {
      ++Qk;
    }
  }
}

inline void queue_delete_i(GpiList& Q, int i, std::string base_filename, std::string to_filename) // remove indices (i,j) and (j,i) from Q (for all j)
{
  for (GpiList::iterator Qk=Q.begin(); Qk!=Q.end(); )
  {
    int ii = boost::get<int>(get_list(*Qk).front());
    int jj = boost::get<int>(*std::next(get_list(*Qk).begin()));
    if(ii==i || jj==i)
    {
      Qk = Q.erase(Qk);
      std::remove((base_filename+"queue/started/"+std::to_string(std::min(ii,jj))+"_"+std::to_string(std::max(ii,jj))).c_str());
      std::ofstream ijFile (base_filename+"queue/"+to_filename+"/"+std::to_string(std::min(ii,jj))+"_"+std::to_string(std::max(ii,jj)));
      std::cout << "writing file " << base_filename+to_filename+"/"+std::to_string(std::min(ii,jj))+"_"+std::to_string(std::max(ii,jj)) << std::endl;
      ijFile.close();
    }
    else
    {
      ++Qk;
    }
  }
}
*/


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
void singular_init(std::string const& base_filename,
                   std::string const& input,
                   bool prev_queue_had_started,
                   std::vector<std::vector<std::vector<int> > >* Mvec,
                   GpiList* degBounds,
                   long* redSB,
                   long* nworkers,
                   int* prev_r,
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
