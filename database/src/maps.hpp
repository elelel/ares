#pragma once

#include <pqxx/pqxx>

#include <ares/model>

#include "pqxx_traits.hpp"

namespace ares {
  namespace database {
    namespace maps {
      struct update : pqxx::transactor<> {
        using result_type = std::optional<uint32_t>;
        result_type& rslt;

        update(result_type& rslt, const std::string& name, const model::map_info& info);

        void operator()(argument_type& trans);
      private:
        const std::string& name_;
        const model::map_info& info_;
      };

      struct ids_and_names : pqxx::transactor<> {
        using result_type = std::vector<std::tuple<uint32_t, std::string>>;
        result_type& rslt;

        ids_and_names(result_type& rslt);
        
        void operator()(argument_type& trans);
      };

      struct id_by_name : pqxx::transactor<> {
        using result_type = std::optional<uint32_t>;
        result_type& rslt;

        id_by_name(result_type& rslt, const std::string& name);

        void operator()(argument_type& trans);
      private:
        const std::string& name_;
      };

      struct info : pqxx::transactor<> {
        using result_type = std::optional<model::map_info>;
        result_type& rslt;

        info(result_type& rslt, const uint32_t& id);

        void operator()(argument_type& trans);
      private:
        const uint32_t& id_;
      };
    }
  }
}
