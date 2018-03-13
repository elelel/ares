#include "state.hpp"

#include "../server.hpp"

void ares::zone::client::packet_handler<ares::packet::current<ares::packet::CZ_REQUEST_MOVE>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_MOVE: begin");

  SPDLOG_TRACE(log(), "CZ_REQUEST_MOVE: to x = {}, to y = {}, dir = {}", p_->coords().x(), p_->coords().y(), p_->coords().dir());

  SPDLOG_TRACE(log(), "handle_packet CZ_REQUEST_MOVE: end");
}
