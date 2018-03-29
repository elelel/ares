#include "state.hpp"

#include <ares/packets>

#include "../server.hpp"
#include "../session.hpp"

namespace ares {
  namespace zone {
    namespace character_server {
      struct ping_character_server_handler {
        ping_character_server_handler(std::shared_ptr<session> sess, const bool pinged) :
          session_(sess),
          pinged_(pinged) {
        }
        
        ping_character_server_handler(const ping_character_server_handler& other) :
          session_(other.session_),
          pinged_(other.pinged_) {
        }

        void operator()(const std::error_code& ec) {
          if (ec.value() == 0) {
            if (pinged_) {
              session_->log()->error("Timeout while waiting for ping response from character server, closing session");
              session_->close_gracefuly();
            } else {
              SPDLOG_TRACE(session_->log(), "sending ping to character server");
              session_->emplace_and_send<packet::current<packet::ARES_ZH_PING_REQ>>();
              session_->as_char_server().ping_character_server_timer_->expires_at(std::chrono::steady_clock::now() + std::chrono::seconds{5});
              session_->as_char_server().ping_character_server_timer_->async_wait(ping_character_server_handler{session_, true});
            }
          }
        }

      private:
        std::shared_ptr<session> session_;
        bool pinged_;
      };
    }
  }
}


ares::zone::character_server::state::state(ares::zone::server& serv,
                                           ares::zone::session& sess) :
  server_(serv),
  session_(sess),
  ping_character_server_timer_(std::make_shared<asio::steady_timer>(*server_.io_context())) {
}

void ares::zone::character_server::state::reset_ping_character_server_timer() {
  const size_t timeout = 10;
  SPDLOG_TRACE(log(), "character_server::state resetting ping timer, timeout {} second", timeout);
  ping_character_server_timer_->cancel();
  ping_character_server_timer_->expires_at(std::chrono::steady_clock::now() + std::chrono::seconds{timeout});
  ping_character_server_timer_->async_wait(ping_character_server_handler{session_.shared_from_this(), false});
  
}

void ares::zone::character_server::state::on_connect() {
  log()->info("Starting handshake with character server");
  std::lock_guard<std::mutex> lock(server_.mutex());
  const auto my_ipv4 = conf().listen_ipv4[0].address().to_v4().to_ulong();
  const auto my_port = conf().listen_ipv4[0].port();
  session_.emplace_and_send<packet::current<packet::ARES_ZH_LOGIN_REQ>>(conf().character_server->login,
                                                                        conf().character_server->password,
                                                                        htonl(my_ipv4),
                                                                        htons(my_port));
}

void ares::zone::character_server::state::on_connection_reset() {
  log()->warn("Disconnected from character server (connection reset)");
  session_.close_abruptly();
}

void ares::zone::character_server::state::on_operation_aborted() {
  log()->warn("Operation aborted while communicating with character server");
  session_.close_abruptly();
}

void ares::zone::character_server::state::on_eof() {
  log()->warn("Character server closed connection (eof)");
  session_.close_abruptly();
}

void ares::zone::character_server::state::on_socket_error() {
  log()->warn("Socket error while communicating with character server");
  session_.close_abruptly();
}

void ares::zone::character_server::state::defuse_asio() {
  if (ping_character_server_timer_)
    ping_character_server_timer_->cancel();
}

auto ares::zone::character_server::state::packet_sizes(const uint16_t packet_id) -> std::tuple<size_t, size_t, size_t> {
  switch (packet_id) {
    ARES_PACKET_SIZES_CASE(ARES_HZ_LOGIN_RESULT);
    ARES_PACKET_SIZES_CASE(ARES_HZ_PRIVATE_MSG_NAME);
    ARES_PACKET_SIZES_CASE(ARES_HZ_PING_ACK);
    ARES_PACKET_SIZES_CASE(ARES_HZ_MAP_IDS);
    ARES_PACKET_SIZES_CASE(ARES_HZ_CHAR_AUTH_RESULT);
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for character server session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::zone::character_server::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(ARES_HZ_LOGIN_RESULT);
    ARES_DISPATCH_PACKET_CASE(ARES_HZ_PRIVATE_MSG_NAME);
    ARES_DISPATCH_PACKET_CASE(ARES_HZ_PING_ACK);
    ARES_DISPATCH_PACKET_CASE(ARES_HZ_MAP_IDS);
    ARES_DISPATCH_PACKET_CASE(ARES_HZ_CHAR_AUTH_RESULT);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for character server session, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
}

auto ares::zone::character_server::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::zone::character_server::state::conf() const -> const config& {
  return server_.conf();
}
