#include "recv_handler.hpp"

#include "session.hpp"

size_t ares::character::recv_handler::dispatch(const uint16_t PacketType) {
  struct visitor {
    visitor(session& s, const uint16_t PacketType) :
      s(s), PacketType(PacketType) {};

    size_t operator()(const mono::state&) {
      return std::get<mono::state>(s.session_state_).dispatch(PacketType);
    }

    size_t operator()(const zone_server::state&) {
      return std::get<zone_server::state>(s.session_state_).dispatch(PacketType);
    }

    size_t operator()(const account_server::state&) {
      return std::get<account_server::state>(s.session_state_).dispatch(PacketType);
    }

    size_t operator()(const client::state&) {
      return std::get<client::state>(s.session_state_).dispatch(PacketType);
    }
  private:
    session& s;
    uint16_t PacketType;
  };
  return std::visit(visitor(*session_, PacketType), session_->session_state_);
}

void ares::character::recv_handler::terminate_session() {
  session_->close_socket();
  session_->remove_from_server();
}

void ares::character::recv_handler::on_operation_aborted() {
  session_->on_operation_aborted();
}

void ares::character::recv_handler::on_connection_reset() {
  session_->on_connection_reset();
}

void ares::character::recv_handler::on_eof() {
  session_->on_eof();
}

void ares::character::recv_handler::on_socket_error() {
  session_->on_socket_error();
}

void ares::character::recv_handler::on_inactivity_timeout() {
  //SPDLOG_TRACE(session_->log_, "character::recv_handler::on_inactivity_timeout()");
  session_->close_socket();
}
