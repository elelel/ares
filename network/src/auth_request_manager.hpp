#pragma once

#include <chrono>
#include <mutex>

namespace ares {
  namespace network {
    /*!
      Generic interserver authentication request manager
     */
    template <typename Derived, typename Server, typename Session>
    struct auth_request_manager {

      /*! Create authentication request manager
        \param auth_timeout authentication timeout after which auth request is considered failed
       */
      auth_request_manager(Server& serv,
                           const std::chrono::seconds auth_timeout);

      /*!
        Defuse Asio which may hold resources and prevent cleanup
       */
      void defuse_asio();

      /*!
        Create a new request for a client session and return it's id
       */
      uint32_t new_request(std::shared_ptr<Session> s);

      /*!
        Notify manager that a there was a response to the request. This deletes it from pending list
        and returns the client session this request_id belongs to.
       */
      std::shared_ptr<Session> responded(const uint32_t request_id);

      /*!
        Notify manager that all requests for the client session should be deleted (e.g., when
        the client disconnects).
       */
      void cancel(std::shared_ptr<Session> s);

      /*!
        Return reference to the manager's mutex for synchronization
       */
      std::mutex& mutex();
      
    protected:
      Server& server_;

    private:
      struct request_info {
        std::weak_ptr<Session> session;
        std::shared_ptr<asio::steady_timer> timer;
      };
      
      std::map<uint32_t, request_info> pending_;
      uint32_t i_;
      std::chrono::seconds auth_timeout_;

      std::mutex mutex_;
    };
  }
}
