#pragma once

#include <ares/network>
#include <ares/database>

#include "auth_request_manager.hpp"
#include "config.hpp"
#include "session.hpp"
#include "maps_manager.hpp"
#include "pcs_manager.hpp"

namespace ares {
  namespace zone {
    struct server : ares::network::server<server, session> {
      server(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<asio::io_context> io_context,
             const config& conf);

      void init_thread_local();
      
      /*! Starts the zone server */
      void start();

      const config& conf() const;
      
      /*! Adds a session to current sessions list tracking the state type.
       \param s session to add */
      void add(std::shared_ptr<session> s);

      /*! Returns client session by account id 
       \param aid account id */
      std::shared_ptr<session> find_client_session(const model::account_id& account_id) const;
      
      std::shared_ptr<session> char_server() const;

      std::shared_ptr<auth_request_manager> auth_requests;

      zone::maps_manager maps;
      zone::pcs_manager pcs;

    protected:
      friend ares::network::server<server, session>;
      friend ares::network::acceptor<server>;

      /*! Creates a network session within the server, called from acceptor
        \param socket TCP socket of the new network session
      */
      void create_session(std::shared_ptr<asio::ip::tcp::socket> socket);

      /*! Removes a session from current sessions list
       \param s session to remove */
      void remove(std::shared_ptr<session> s);

    private:
      std::map<model::account_id, std::weak_ptr<session>> clients_;
      std::weak_ptr<session> char_server_;
      std::set<std::weak_ptr<session>> mono_;
      
      const config& conf_;
      
    public:
      static thread_local std::shared_ptr<ares::database::db> db;

    };
  }
}
