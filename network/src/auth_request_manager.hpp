#pragma once

#include <chrono>

namespace ares {
  namespace network {
    template <typename Derived, typename Server, typename Session>
    struct auth_request_manager {

      auth_request_manager(Server& serv,
                           const std::chrono::seconds auth_timeout);

      void defuse_asio();
      uint32_t new_request(std::shared_ptr<Session> s);
      std::shared_ptr<Session> responded(const uint32_t request_id);
      void cancel(std::shared_ptr<Session> s);
      
    protected:
      Server& server_;

    private:
      struct request_info {
        std::shared_ptr<Session> session;
        std::shared_ptr<asio::steady_timer> timer;
      };
      
      std::map<uint32_t, request_info> pending_;
      uint32_t i_;
      std::chrono::seconds auth_timeout_;
    };
  }
}
