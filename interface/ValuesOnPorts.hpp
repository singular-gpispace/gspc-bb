#pragma once

#include <gspc/we/type/value.hpp>

#include <map>
#include <string>

namespace buchberger_module
{
    class ValuesOnPorts
    {
        public:
            using Key = std::string;
            using Value = gspc::we::type::value::value_type;
            using Map = std::multimap<Key, Value>;

            ValuesOnPorts (Map map);

            Map const& map() const;

    protected:
        Map _values_on_ports;
  };
}
