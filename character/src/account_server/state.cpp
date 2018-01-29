#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../state.hpp"

ares::character::account_server::state::state(character::state& server_state, session& sess) :
  server_state_(server_state),
  session_(sess),
  reconnect_timer(sess.shared_from_this(), std::chrono::seconds(5)),
  send_aids_timer(sess.shared_from_this(), std::chrono::seconds(120)),
  send_user_count_timer(sess.shared_from_this(), std::chrono::seconds(120)),
  ping_request_timer(sess.shared_from_this(), std::chrono::seconds(10)),
  ping_timeout_timer(sess.shared_from_this(), std::chrono::seconds(5)) {
  }

void ares::character::account_server::state::defuse_asio() {
  reconnect_timer.cancel();
  send_aids_timer.cancel();
  send_user_count_timer.cancel();
  ping_request_timer.cancel();
  ping_timeout_timer.cancel();
}

void ares::character::account_server::state::on_open() {
  SPDLOG_TRACE(server_state_.log(), "account_server::state on_open");
}

void ares::character::account_server::state::before_close() {
  SPDLOG_TRACE(server_state_.log(), "account_server::state before_close");
}

void ares::character::account_server::state::on_connection_reset() {
  server_state_.log()->warn("Connection with account server reset");
  SPDLOG_TRACE(server_state_.log(), "account_server::state on_connection_reset");
}

void ares::character::account_server::state::on_eof() {
  server_state_.log()->warn("Connection with account server closed");
  SPDLOG_TRACE(server_state_.log(), "account_server::state on_eof");
}

void ares::character::account_server::state::on_socket_error() {
  SPDLOG_TRACE(server_state_.log(), "account_server::state on_socket_error");
}

void ares::character::account_server::state::on_operation_aborted() {
  SPDLOG_TRACE(server_state_.log(), "account_server::state on_operation_aborted");
}

size_t ares::character::account_server::state::dispatch(const uint16_t PacketType) {
  SPDLOG_TRACE(server_state_.log(), "account_server::state::dispatch() switching on PacketType = {0:#x}", PacketType);
  switch (PacketType) {
    ARES_PACKET_CASE(ATHENA_AH_LOGIN_RESULT);
    ARES_PACKET_CASE(ATHENA_AH_PING_ACK);
    ARES_PACKET_CASE(ATHENA_AH_AID_AUTH_RESULT);
    ARES_PACKET_CASE(ATHENA_AH_ACCOUNT_DATA_RESULT);
    ARES_PACKET_CASE(ATHENA_AH_KICK_AID);
  }
  server_state_.log()->error("Unexpected PacketType {0:#x} for account_server::state session", PacketType);
  throw ares::network::terminate_session();
}
