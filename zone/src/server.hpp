#pragma once

#include <map>
#include <set>

#include <ares/network>

#include "config.hpp"
#include "session.hpp"

namespace ares {
  namespace zone {

    struct state;
    
    struct server : ares::network::server<server> {
      server(state& state_);

      void start();
      void create_session(std::shared_ptr<asio::ip::tcp::socket> socket);

      void add(session_ptr s);
      void remove(session_ptr s);

      session_ptr client_by_aid(const uint32_t aid);

      const session_ptr& char_server() const;

    private:
      state& state_;
      
      std::set<session_ptr> mono_;
      std::map<uint32_t, session_ptr> clients_;
      session_ptr char_server_;
    };
  }
}
