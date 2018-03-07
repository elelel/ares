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
      void add(session_ptr s);
      
      /*! Returns client session by account id 
        \param aid account id */
      session_ptr client_by_aid(const uint32_t aid);
      
      void link_aid_to_zone_server(const uint32_t aid, session_ptr s);
      void unlink_aid_from_zone_server(const uint32_t aid, session_ptr s);

      session_ptr zone_server_by_map_id(const uint32_t map_id) const;


      const std::map<uint32_t, session_ptr>& clients() const;
      const session_ptr& account_server() const;
      const std::set<session_ptr>& zone_servers() const;

      
      std::map<std::string, std::vector<uint32_t>> zone_login_to_maps;
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
      void remove(session_ptr s);

    private:
      std::map<uint32_t, session_ptr> clients_;
      std::set<session_ptr> mono_;
      std::set<session_ptr> zone_servers_;
      std::map<uint32_t, session_ptr> map_id_to_zone_server_;
      session_ptr account_server_;
      
      std::map<uint32_t, session_ptr> aid_to_zone_server_;

      const config& conf_;
    public:
      static thread_local std::shared_ptr<ares::database::db> db;

      std::shared_ptr<auth_request_manager> auth_requests;
      std::shared_ptr<maps_manager> maps;

    private:
      void verify_db_map_info(const uint32_t map_id, const std::string& map_name, std::shared_ptr<ares::grf::resource_set>& resources);
    };
  }
}
