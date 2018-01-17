#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

#include "../world/world.hpp"

void ares::zone::client::packet_handler<ares::packet::CZ_REQUEST_MOVE>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_MOVE: begin");

  // TODO: Validate p_->to_x() p_->to_y() here
  
  auto& c = session_.as_client();
  rxcpp::rxs::just(world::event::pc_move_command{c.gid, p_->to_x(), p_->to_y()}).
    subscribe(server_.world().pc_move_commands.get_subscriber());
  
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_MOVE: end");
}