#pragma once

#include <ares/network>
#include <ares/database>

#include "auth_request_manager.hpp"
#include "config.hpp"
#include "session.hpp"

namespace ares {
  namespace zone {
    struct server : ares::network::server<server, session> {
      server(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<asio::io_context> io_context,
             const config& conf);

      /*! Starts the zone server */
      void start();

      /*! Adds a session to current sessions list tracking the state type.
       \param s session to add */
      void add(session_ptr s);


      /*! Returns client session by account id 
       \param aid account id */
      session_ptr client_by_aid(const uint32_t aid);
      
      const session_ptr& char_server() const;
      const config& conf() const;

      std::set<uint32_t> map_ids;

      std::shared_ptr<auth_request_manager> auth_requests;

      std::unordered_map<std::string, uint32_t> map_name_to_id;
      std::map<uint32_t, std::string> map_id_to_name;

    protected:
      friend ares::network::server<server, session>;
      friend ares::network::acceptor<server>;

      /*! Creates a network session within the server, called from acceptor
        \param socket TCP socket of the new network session
      */
      void create_session(std::shared_ptr<asio::ip::tcp::socket> socket);

      /*! Removes a session from current sessions list
       \param s session to remove */
      void remove(session_ptr s);

    private:
      std::map<uint32_t, session_ptr> clients_;
      session_ptr char_server_;
      std::set<session_ptr> mono_;
      
      const config& conf_;
      
    public:
      ares::database::db db;

    };
  }
}
