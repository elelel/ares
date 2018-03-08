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
        \param aid account id */
      std::shared_ptr<session> client_by_aid(const uint32_t aid);
      
      void link_aid_to_zone_server(const uint32_t aid, std::shared_ptr<session> s);
      void unlink_aid_from_zone_server(const uint32_t aid, std::shared_ptr<session> s);

      // std::shared_ptr<session> zone_server_by_map_id(const uint32_t map_id) const;

      std::shared_ptr<session> account_server() const;
      /*
      const std::map<uint32_t, std::weak_ptr<session>>& clients() const;

      const std::set<std::weak_ptr<session>>& zone_servers() const;
      */

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
      std::map<uint32_t, std::weak_ptr<session>> clients_;
      std::set<std::weak_ptr<session>> mono_;
      std::set<std::weak_ptr<session>> zone_servers_;
      std::weak_ptr<session> account_server_;

      std::map<uint32_t, std::weak_ptr<session>> aid_to_zone_server_;

      const config& conf_;
    public:
      static thread_local std::unique_ptr<ares::database::db> db;

      std::shared_ptr<auth_request_manager> auth_requests;
      std::unique_ptr<maps_manager> maps;

    private:
      void verify_db_map_info(const uint32_t map_id, const std::string& map_name, std::shared_ptr<ares::grf::resource_set>& resources);
    };
  }
}
