#include "state.hpp"
#include "../server.hpp"
#include "../character_server/state.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_PRIVATE_MSG_NAME>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_PRIVATE_MSG_NAME: begin");
  if (strlen(p_->name()) > 0) {
    //auto& cs = session_.as_char_server();
    //    cs.private_msg_name = p_->name();
    //    SPDLOG_TRACE(log(), "Character server for private messages is set to '{}'", cs.private_msg_name);
  } else {
    log()->error("Error parsing ARES_HZ_PRIVATE_MSG_NAME, name length is 0");
    session_.close_gracefuly();
  }
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_PRIVATE_MSG_NAME: end");
}
