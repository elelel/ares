#include "state.hpp"
#include "../server.hpp"
#include "../character_server/state.hpp"

void ares::zone::character_server::packet_handler<ares::packet::ATHENA_HZ_PRIVATE_MSG_NAME>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_PRIVATE_MSG_NAME: begin");
  if (strlen(p_->name()) > 0) {
    auto& cs = session_.as_character_server();
    cs.private_msg_name = p_->name();
    SPDLOG_TRACE(log(), "Character server for private messages is set to '{}'", cs.private_msg_name);
  } else {
    log()->error("Error parsing ATENA_HZ_PRIVATE_MSG_NAME, name length is 0");
    throw network::terminate_session();
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_PRIVATE_MSG_NAME: end");
}
