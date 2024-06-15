#pragma once

#include <interface/ValuesOnPorts.hpp>

#include <cstddef>

namespace buchberger_module
{
  class WorkflowResult : public ValuesOnPorts
  {
  public:
    using ValuesOnPorts::ValuesOnPorts;

    void assert_key_count
      ( Key key
      , std::size_t expected_count
      ) const;

    // asserts there is exactly one occurence of key
    template<typename T> T const& get (Key key) const;




  private:

    template<typename T, typename TypeDescription>
       T const& get_impl (Key key, TypeDescription type_description) const;
  };

    template<> int const& WorkflowResult::get (Key key) const;
}
