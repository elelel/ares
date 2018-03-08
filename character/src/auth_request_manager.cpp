#include "auth_request_manager.hpp"

#include "server.hpp"

void ares::character::auth_request_manager::auth_timeout(std::shared_ptr<session> s) {
  server_.log()->warn("Aid authentication by account server timed out, closing client session");
  s->emplace_and_send<packet::current<packet::AC_REFUSE_LOGIN>>(8); // Rejected by server
  s->close_gracefuly();
}
