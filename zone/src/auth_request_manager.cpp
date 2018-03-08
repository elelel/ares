#include "auth_request_manager.hpp"

#include "server.hpp"

void ares::zone::auth_request_manager::auth_timeout(std::shared_ptr<session> s) {
  server_.log()->warn("Cid authentication by character server timed out, closing client session");
  s->close_gracefuly();
}
