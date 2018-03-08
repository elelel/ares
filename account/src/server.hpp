#pragma once

#include <ares/network>
#include <ares/database>

#include "config.hpp"
#include "session.hpp"

namespace ares {
  namespace account {
    struct server : ares::network::server<server, session> {
      server(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<asio::io_context> io_context,
             const config& conf);

      void init_thread_local();
      
      /*! Starts the account server */
      void start();

      const config& conf() const;

      /*! Returns connected character server sessions */ 
      //      const std::set<session_ptr>& char_servers() const;

      /*! Adds a session to current sessions list
       \param s session to add */
      void add(std::shared_ptr<session> s);
      
      /*! Returns client session by account id 
       \param aid account id */
      std::shared_ptr<session> client_by_aid(const uint32_t aid) const;
      /*! Associates account id with character server session
       \param aid Account id
       \param s character server session */
      void link_aid_to_char_server(const uint32_t aid, std::shared_ptr<session> s);
      /*! Dessociates account id from character server session
       \param aid Account id
       \param s character server session */
      void unlink_aid_from_char_server(const uint32_t aid, std::shared_ptr<session> s);

      size_t num_char_servers_open() const;

      const std::set<std::weak_ptr<session>>& char_servers() const;

      std::shared_ptr<session> char_server_by_login(const std::string& login) const;

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
      std::map<uint32_t, std::weak_ptr<session>> aid_to_char_server_;
      std::map<uint32_t, std::weak_ptr<session>> clients_;
      std::set<std::weak_ptr<session>> char_servers_;
      std::set<std::weak_ptr<session>> mono_;

      const config& conf_;
      
    public:
      static thread_local std::shared_ptr<ares::database::db> db;
    };
  }
}
