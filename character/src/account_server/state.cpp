#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"

namespace ares {
  namespace character {
    namespace account_server {
      struct ping_account_server_handler {
        ping_account_server_handler(std::shared_ptr<session> sess, const bool pinged) :
          session_(sess),
          pinged_(pinged) {
        }
        
        ping_account_server_handler(const ping_account_server_handler& other) :
          session_(other.session_),
          pinged_(other.pinged_) {
        }

        void operator()(const std::error_code& ec) {
          if (ec.value() == 0) {
            if (pinged_) {
              session_->log()->error("Timeout while waiting for ping response from account server, closing session");
              session_->close_gracefuly();
            } else {
              SPDLOG_TRACE(session_->log(), "sending ping to account server");
              session_->emplace_and_send<packet::current<packet::ARES_HA_PING_REQ>>();
              session_->as_account_server().ping_account_server_timer_->expires_at(std::chrono::steady_clock::now() + std::chrono::seconds{5});
              session_->as_account_server().ping_account_server_timer_->async_wait(ping_account_server_handler{session_, true});
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

ares::character::account_server::state::state(character::server& serv, session& sess) :
  server_(serv),
  session_(sess),
  ping_account_server_timer_(std::make_shared<asio::steady_timer>(*server_.io_context())) {
}

ares::character::account_server::state::~state() {
  SPDLOG_TRACE(log(), "Destructing client state for session {}", session_.id());
}

void ares::character::account_server::state::reset_ping_account_server_timer() {
  const size_t timeout = 10;
  SPDLOG_TRACE(log(), "account_server::state resetting ping timer, timeout {} second", timeout);
  ping_account_server_timer_->cancel();
  ping_account_server_timer_->expires_at(std::chrono::steady_clock::now() + std::chrono::seconds{timeout});
  ping_account_server_timer_->async_wait(ping_account_server_handler{session_.shared_from_this(), false});
  
}

void ares::character::account_server::state::on_connect() {
  log()->info("Starting handshake with account server");
  auto& conf = server_.conf();
  std::lock_guard<std::mutex> lock(server_.mutex());
  log()->info("Starting handshake with account server");
  const auto my_ipv4 = conf.listen_ipv4[0].address().to_v4().to_ulong();
  const auto my_port = conf.listen_ipv4[0].port();
  session_.emplace_and_send<packet::current<packet::ARES_HA_LOGIN_REQ>>(conf.account_server->login,
                                                                        conf.account_server->password,
                                                                        htonl(my_ipv4),
                                                                        htons(my_port),
                                                                        *conf.server_name,
                                                                        server_.state_num,
                                                                        server_.property);
}

void ares::character::account_server::state::on_connection_reset() {
  session_.close_abruptly();
}

void ares::character::account_server::state::on_operation_aborted() {
  session_.close_abruptly();
}

void ares::character::account_server::state::on_eof() {
  SPDLOG_TRACE(log(), "Account server session {} EOF", session_.id());
  session_.close_abruptly();
}

void ares::character::account_server::state::on_socket_error() {
  session_.close_abruptly();
}

void ares::character::account_server::state::defuse_asio() {
  if (ping_account_server_timer_)
    ping_account_server_timer_->cancel();
}

auto ares::character::account_server::state::packet_sizes(const uint16_t packet_id) -> std::tuple<size_t, size_t, size_t> {
  switch (packet_id) {
    ARES_PACKET_SIZES_CASE(ARES_AH_LOGIN_RESULT);
    ARES_PACKET_SIZES_CASE(ARES_AH_PING_ACK);
    ARES_PACKET_SIZES_CASE(ARES_AH_ACCOUNT_AUTH_RESULT);
    ARES_PACKET_SIZES_CASE(ARES_AH_KICK_ACCOUNT);
  default:
    { 
      log()->error("Unexpected packet_id {:#x} for account server session while getting packet sizes", packet_id);
      return std::tuple<size_t, size_t, size_t>(0, 0, 0);
    }
  }
}

void ares::character::account_server::state::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf.get());
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(ARES_AH_LOGIN_RESULT);
    ARES_DISPATCH_PACKET_CASE(ARES_AH_PING_ACK);
    ARES_DISPATCH_PACKET_CASE(ARES_AH_ACCOUNT_AUTH_RESULT);
    ARES_DISPATCH_PACKET_CASE(ARES_AH_KICK_ACCOUNT);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for account server session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      return;
    }
  }
}

auto ares::character::account_server::state::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

auto ares::character::account_server::state::conf() const -> const config& {
  return server_.conf();
}
