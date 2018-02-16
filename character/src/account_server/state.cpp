#include "state.hpp"

#include <ares/packets>
#include <ares/network>

#include "../server.hpp"

namespace ares {
  namespace character {
    namespace account_server {
      struct ping_account_server_handler {
        ping_account_server_handler(session_ptr sess, const bool pinged) :
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
              session_->emplace_and_send<packet::current<packet::ATHENA_HA_PING_REQ>>();
              session_->as_account_server().ping_account_server_timer_->expires_at(std::chrono::steady_clock::now() + std::chrono::seconds{5});
              session_->as_account_server().ping_account_server_timer_->async_wait(ping_account_server_handler{session_, true});
            }
          }
        }

      private:
        session_ptr session_;
        bool pinged_;
      };
    }
  }
}

ares::character::account_server::state::state(character::server& serv, session& sess) :
  server_(serv),
  session_(sess),
  ping_account_server_timer_(std::make_shared<asio::steady_timer>(*server_.io_context()))
  
  //  send_aids_timer(sess.shared_from_this(), std::chrono::seconds(120)),
  //  send_user_count_timer(sess.shared_from_this(), std::chrono::seconds(120)),
{
  
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
  session_.connected_ = true;
  log()->info("Starting handshake with account server");
  const auto my_ipv4 = conf.listen_ipv4[0].address().to_v4().to_ulong();
  const auto my_port = conf.listen_ipv4[0].port();
  session_.emplace_and_send<packet::current<packet::ATHENA_HA_LOGIN_REQ>>(conf.account_server->login,
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
  session_.close_abruptly();
}

void ares::character::account_server::state::on_socket_error() {
  session_.close_abruptly();
}

void ares::character::account_server::state::defuse_asio() {
  if (ping_account_server_timer_)
    ping_account_server_timer_->cancel();
  
  //send_aids_timer.cancel();
  //send_user_count_timer.cancel();
}

auto ares::character::account_server::state::allocate(const uint16_t packet_id) -> packet::alloc_info {
  switch (packet_id) {
    ARES_ALLOCATE_PACKET_CASE(ATHENA_AH_LOGIN_RESULT);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_AH_PING_ACK);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_AH_AID_AUTH_RESULT);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_AH_ACCOUNT_DATA_RESULT);
    ARES_ALLOCATE_PACKET_CASE(ATHENA_AH_KICK_AID);
  default:
    { // Packet id is not known to this server under selected packet set
      log()->error("Unexpected packet_id {:#x} for account server session while allocating", packet_id);
      packet::alloc_info ai;
      ai.expected_packet_sz = 0;
      ai.buf = nullptr;
      ai.buf_sz = 0;
      ai.deallocator = [] (void*) {};
      ai.PacketLength_offset = 0;
      return std::move(ai);
    }
  }
}

void ares::character::account_server::state::dispatch_packet(void* buf, std::function<void(void*)> deallocator) {
  uint16_t* packet_id = reinterpret_cast<uint16_t*>(buf);
  switch (*packet_id) {
    ARES_DISPATCH_PACKET_CASE(ATHENA_AH_LOGIN_RESULT);
    ARES_DISPATCH_PACKET_CASE(ATHENA_AH_PING_ACK);
    ARES_DISPATCH_PACKET_CASE(ATHENA_AH_AID_AUTH_RESULT);
    ARES_DISPATCH_PACKET_CASE(ATHENA_AH_ACCOUNT_DATA_RESULT);
    ARES_DISPATCH_PACKET_CASE(ATHENA_AH_KICK_AID);
  default:
    {
      log()->error("Unexpected packet_id {:#x} for account server session while dispatching, disconnecting", *packet_id);
      session_.close_gracefuly();
      session_.connected_ = false;
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
