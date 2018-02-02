#include "recv_handler.hpp"

#include "session.hpp"
#include "state.hpp"
  
size_t ares::zone::recv_handler::dispatch(const uint16_t PacketType) {
  struct visitor {
    visitor(session& s, const uint16_t PacketType) :
      s(s), PacketType(PacketType) { };

    size_t operator()(const mono::state&) {
      return s.as_mono().dispatch(PacketType);
    }

    size_t operator()(const character_server::state&) {
      return s.as_character_server().dispatch(PacketType);
    }

    size_t operator()(const client::state&) {
      return s.as_client().dispatch(PacketType);
    }
  private:
    session& s;
    uint16_t PacketType;
  };

  uint16_t packet_id = session_->obf_crypt_key
    ? PacketType ^ ((*session_->obf_crypt_key >> 16) & 0x7fff)
    : PacketType;
  SPDLOG_TRACE(log(), "Obfuscation status obf_crypt_key active: {}, original PacketType {:x}, decrypted packet_id {:x}", bool(session_->obf_crypt_key), PacketType, packet_id);
  return std::visit(visitor(*session_, packet_id), session_->session_state_);
}

void ares::zone::recv_handler::on_processed_packet() {
  if (session_->obf_crypt_key) {
    session_->obf_crypt_key.emplace(*session_->obf_crypt_key *
                                   std::get<1>(*session_->server_state_.conf.obfuscation_key) +
                                   std::get<2>(*session_->server_state_.conf.obfuscation_key));
  }
}

void ares::zone::recv_handler::terminate_session() {
  session_->close_socket();
  session_->remove_from_server();
}

void ares::zone::recv_handler::on_operation_aborted() {
  session_->on_operation_aborted();
}

void ares::zone::recv_handler::on_connection_reset() {
  session_->on_connection_reset();
}

void ares::zone::recv_handler::on_eof() {
  session_->on_eof();
}

void ares::zone::recv_handler::on_socket_error() {
  session_->on_socket_error();
}

void ares::zone::recv_handler::on_inactivity_timeout() {
  //SPDLOG_TRACE(session_->log_, "zone::recv_handler::on_inactivity_timeout()");
  session_->close_socket();
}
