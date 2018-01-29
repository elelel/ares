#pragma once

#include <map>
#include <set>

#include <ares/network>

#include "session.hpp"
#include "database/database.hpp"

namespace ares {
  namespace character {
    struct server : ares::network::server<server> {
      server(state& state_);

      void start();
      void create_session(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

      void add(session_ptr s);
      void remove(session_ptr s);

      session_ptr client_by_aid(const uint32_t aid);
      void link_aid_to_zone_server(const uint32_t aid, session_ptr s);
      void unlink_aid_from_zone_server(const uint32_t aid, session_ptr s);

      void add_auth_aid_request(const int32_t request_id, session_ptr s);
      session_ptr session_by_auth_request_id(const int32_t request_id);
      void remove_auth_aid_request(const int32_t request_id);
      void prune_auth_aid_requests();

      const std::map<uint32_t, session_ptr>& clients() const;
      const session_ptr& account_server() const;
      const std::set<session_ptr>& zone_servers() const;

    private:
      state& state_;
      
      std::set<session_ptr> mono_;
      std::set<session_ptr> zone_servers_;
      std::map<uint32_t, session_ptr> clients_;
      std::map<uint32_t, session_ptr> aid_to_zone_server_;
      // ATHENA_HA_AID_AUTH_REQ requests with timestamps
      std::map<int32_t, std::pair<session_ptr, std::chrono::time_point<std::chrono::steady_clock>>> auth_aid_requests_;

      session_ptr account_server_;

    };
  }
}
