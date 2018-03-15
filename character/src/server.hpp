#pragma once

#include <map>
#include <set>

#include <ares/database>
#include <ares/network>

#include "auth_request_manager.hpp"
#include "maps_manager.hpp"
#include "session.hpp"

namespace ares {
  namespace character {
    struct server : ares::network::server<server, session> {
      server(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<asio::io_context> io_context,
             const config& conf);

      void init_thread_local();
      
      void start();

      const config& conf() const;

      /*! Adds a session to current sessions list
        \param s session to add */
      void add(std::shared_ptr<session> s);
      
      /*! Returns client session by account id 
        \param account_id account id */
      std::shared_ptr<session> find_client_session(const model::account_id& account_id) const;
      
      void link_to_zone_session(const model::account_id& account_id, std::shared_ptr<session> s);
      void unlink_from_zone_session(const model::account_id& account_id, std::shared_ptr<session> s);

      std::shared_ptr<session> account_server() const;

      std::shared_ptr<session> zone_server_by_login(const std::string& login) const;
      
      // Data
      uint16_t state_num{0};
      uint16_t property{0};
      
    protected:
      friend ares::network::server<server, session>;
      friend ares::network::acceptor<server>;

      /*! Creates a network session within the server, called from acceptor
        \param socket TCP socket of the new network session
      */
      void create_session(std::shared_ptr<asio::ip::tcp::socket> socket);

      /*! Removes a session from current sessions list. Should be called only from RX interface
       \param s session to remove */
      void remove(std::shared_ptr<session> s);

    private:
      std::map<model::account_id, std::weak_ptr<session>> clients_;
      std::set<std::weak_ptr<session>> mono_;
      std::set<std::weak_ptr<session>> zone_servers_;
      std::weak_ptr<session> account_server_;

      std::map<model::account_id, std::weak_ptr<session>> account_id_to_zone_server_;

      const config& conf_;
    public:
      static thread_local std::unique_ptr<ares::database::db> db;

      std::shared_ptr<auth_request_manager> auth_requests;
      std::unique_ptr<maps_manager> maps;

    };
  }
}
