#pragma once

#include <map>
#include <set>

#include <ares/network>

#include "predeclare.hpp"

#include "session.hpp"
#include "database/database.hpp"

namespace ares {
  namespace account {
    struct server : ares::network::server<server> {
      server(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<boost::asio::io_service> io_service,
             const config& conf,
             const size_t num_threads);
      
      /*! Starts the account server */
      void start();

      /*! Creates a new session (interface for acceptor)
       \param socket session's TCP socket */
      void create_session(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

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

      /*! Returns reference to account server config */
      const config& conf() const;
      /*! Returns reference to account server database object */
      database& db();
      
      /*! Returns connected character server sessions */ 
      const std::set<session_ptr>& char_servers() const;

    private:
      const config& config_;
      database db_;

      std::set<session_ptr> mono_;
      std::set<session_ptr> char_servers_;
      std::map<uint32_t, session_ptr> clients_;
      std::map<uint32_t, session_ptr> aid_to_char_server_;
    };
  }
}
