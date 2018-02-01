#include "state.hpp"

#include "../state.hpp"

void ares::zone::client::packet_handler<ares::packet<ares::packets::CZ_REQUEST_MOVE>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_MOVE: begin");

  // TODO: Validate p_->to_x() p_->to_y() here
  
  auto& c = session_.as_client();
  rxcpp::rxs::just(event::pc_move_command{c.cid, p_->to_x(), p_->to_y()}).
    subscribe(server_state_.pc_move_commands.get_subscriber());
  
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_MOVE: end");
}
