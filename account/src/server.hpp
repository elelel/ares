#pragma once

#include <map>
#include <set>

#include <ares/network>

#include "session.hpp"

namespace ares {
  namespace account {
    struct state;
    
    struct server : ares::network::server<server> {
      server(state& state_);
      
      /*! Starts the account server */
      void start();

      /*! Creates a new session (interface for acceptor)
       \param socket session's TCP socket */
      void create_session(std::shared_ptr<asio::ip::tcp::socket> socket);

      /*! Adds a session to current sessions list 
       \param s session to add */
      void add(session_ptr s);
      /*! Removes a session from current sessions list
       \param s session to remove */
      void remove(session_ptr s);

      /*! Returns client session by account id 
       \param aid account id */
      session_ptr client_by_aid(const uint32_t aid);
      /*! Associates account id with character server session
       \param aid Account id
       \param s character server session */
      void link_aid_to_char_server(const uint32_t aid, session_ptr s);
      /*! Dessociates account id from character server session
       \param aid Account id
       \param s character server session */
      void unlink_aid_from_char_server(const uint32_t aid, session_ptr s);

      /*! Returns connected character server sessions */ 
      const std::set<session_ptr>& char_servers() const;

    private:
      state& state_;

      std::set<session_ptr> mono_;
      std::set<session_ptr> char_servers_;
      std::map<uint32_t, session_ptr> clients_;
      std::map<uint32_t, session_ptr> aid_to_char_server_;
    };
  }
}
