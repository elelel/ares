#pragma once

#include <ares/network>

#include "config.hpp"
#include "session.hpp"
#include "database/database.hpp"
#include "world/events.hpp"
#include "world/world.hpp"

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

      const session_ptr& char_server() const;
      const config& conf() const;

      std::set<std::string> map_names;
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

      /*! Returns client session by account id 
       \param aid account id */
      session_ptr client_by_aid_(const uint32_t aid);

    private:
      std::map<uint32_t, session_ptr> clients_;
      session_ptr char_server_;
      std::set<session_ptr> mono_;

      const config& conf_;
      zone::world world_;
      
    public:
      database db;

      rxcpp::rxsub::subject<event::add_map> add_map_stream;
    };
  }
}
