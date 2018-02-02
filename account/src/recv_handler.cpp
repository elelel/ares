#include "recv_handler.hpp"

#include "session.hpp"

#include "mono/state.hpp"
#include "client/state.hpp"
#include "char_server/state.hpp"

size_t ares::account::recv_handler::dispatch(const uint16_t PacketType) {
  struct visitor {
    visitor(session& s, const uint16_t PacketType) :
      s(s), PacketType(PacketType) {};

    size_t operator()(const mono::state&) {
      return s.as_mono().dispatch(PacketType);
    }

    size_t operator()(const char_server::state&) {
      return s.as_char_server().dispatch(PacketType);
    }

    size_t operator()(const client::state&) {
      return s.as_client().dispatch(PacketType);
    }
  private:
    session& s;
    uint16_t PacketType;
  };
  return std::visit(visitor(*session_, PacketType), session_->session_state_);
}

void ares::account::recv_handler::on_processed_packet() {
}

void ares::account::recv_handler::terminate_session() {
  session_->close_socket();
  session_->remove_from_server();
}

void ares::account::recv_handler::on_operation_aborted() {
  session_->on_operation_aborted();
}

void ares::account::recv_handler::on_connection_reset() {
  session_->on_connection_reset();
}

void ares::account::recv_handler::on_eof() {
  session_->on_eof();
}

void ares::account::recv_handler::on_socket_error() {
  session_->on_socket_error();
}

void ares::account::recv_handler::on_inactivity_timeout() {
  //SPDLOG_TRACE(session_->log_, "account::recv_handler::on_inactivity_timeout()");
  session_->close_socket();
}
