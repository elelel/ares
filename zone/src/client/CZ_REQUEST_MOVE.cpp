#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::zone::client::packet_handler<ares::packet::CZ_REQUEST_MOVE>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_MOVE: begin");
  auto& c = session_.as_client();
  rxcpp::rxs::just(event::pc_move_command{c.gid, p_.to_x(), p_.to_y()}) | server_.world().pc_move_commands();
  SPDLOG_TRACE(log(), "handle_packet ATHENA_ZH_LOGIN_REQ: end");
}
