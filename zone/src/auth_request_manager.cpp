#include "auth_request_manager.hpp"

#include "server.hpp"

void ares::zone::auth_request_manager::auth_timeout(session_ptr s) {
  server_.log()->warn("Cid authentication by character server timed out, closing client session");
  s->close_gracefuly();
}
