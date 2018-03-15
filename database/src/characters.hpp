#pragma once

#include <pqxx/pqxx>

#include <ares/model>

#include "pqxx_traits.hpp"

namespace ares {
  namespace database {
    namespace characters {
      struct create : pqxx::transactor<> {
        using result_type = std::optional<model::character_id>;
        result_type& rslt;
        
        create(result_type& rslt,
               const model::account_id& account_id,
               const std::string& name,
               const uint8_t& slot,
               const uint16_t& head_palette,
               const uint16_t& head,
               const model::pc_job& job,
               const uint8_t& sex,
               const uint64_t& zeny,
               const uint32_t& map_id,
               const uint16_t& map_x,
               const uint16_t& map_y);

        void operator()(argument_type& trans);

      private:
        const model::account_id& account_id_;
        const std::string& name_;
        const uint8_t& slot_;
        const uint16_t& head_palette_;
        const uint16_t& head_;
        const model::pc_job& job_;
        const uint8_t& sex_;
        const uint64_t& zeny_;
        const uint32_t& map_id_;
        const uint16_t& map_x_;
        const uint16_t& map_y_;
      };

      struct delete_date : pqxx::transactor<> {
        using result_type = std::optional<std::chrono::system_clock::time_point>;
        result_type& rslt;
        
        delete_date(result_type& rslt,
                    const model::character_id& character_id);

        void operator()(argument_type& trans);
        
      private:
        model::character_id character_id_;
      };

      struct info : pqxx::transactor<> {
        using result_type = std::optional<model::pc_info>;
        result_type& rslt;
        
        info(result_type& rslt, const model::character_id& character_id);

        void operator()(argument_type& trans);

      private:
        const model::character_id& character_id_;
      };

      struct infos_for_account_id : pqxx::transactor<> {
        using result_type = std::vector<model::pc_info>;
        result_type& rslt;
        
        infos_for_account_id(result_type& rslt, const model::account_id& account_id, const size_t& max_chars);
        void operator()(argument_type& trans);
      private:
        const model::account_id& account_id_;
        const size_t& max_chars_;
      };

      struct info_for_slot : pqxx::transactor<> {
        using result_type = std::optional<model::pc_info>;
        result_type& rslt;

        info_for_slot(result_type& rslt, const model::account_id& account_id, const uint16_t& slot);

        void operator()(argument_type& trans);
      private:
        const model::account_id& account_id_;
        const uint16_t& slot_;
      };
    }
  }
}
