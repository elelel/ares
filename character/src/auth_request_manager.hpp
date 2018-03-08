#pragma once

#include <ares/network>

#include "session.hpp"

namespace ares {
  namespace character {
    struct server;
    
    struct auth_request_manager : network::auth_request_manager<character::auth_request_manager, server, session>,
                                  std::enable_shared_from_this<character::auth_request_manager> {
      using network::auth_request_manager<character::auth_request_manager, server, session>::auth_request_manager;

      void auth_timeout(std::shared_ptr<session> s);
    };
  }
}

