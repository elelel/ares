#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"
#include "../session.hpp"
#include "../char_server/state.hpp"


void ares::account::mono::packet_handler<typename ares::packet::ATHENA_HA_LOGIN_REQ>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_LOGIN_REQ: begin");
  boost::asio::ip::address_v4 ip_v4(ntohl(p_->ip()));
  uint16_t port(ntohs(p_->port()));
  log()->info("Connection request from char server {} state {}, invites to listening point at {}:{}", p_->server_name(), p_->state(), ip_v4.to_string(), port);

  // TODO: Check if account server isn't closed

  SPDLOG_TRACE(log(), "ATHENA_HA_LOGIN_REQ acquiring server lock");
  std::lock_guard lock(server_.mutex());
  SPDLOG_TRACE(log(), "ATHENA_HA_LOGIN_REQ server lock acquired");
  auto found = std::find_if(server_.conf().char_servers.begin(),
                            server_.conf().char_servers.end(),
                            [this] (const config::char_server_config_record &c) {
                              return c.login == p_->login();
                            });
  if (found != server_.conf().char_servers.end()) {
    if (found->password == p_->password()) {
      auto existing = find_if(server_.char_servers().begin(), server_.char_servers().end(), [this] (const session_ptr s) {
          return s->as_char_server().login == p_->login();
        });
      if (existing == server_.char_servers().end()) {
        log()->info("Char server {} accepted", p_->server_name());
        session_.emplace_and_send<packet::ATHENA_AH_LOGIN_RESULT>(0);
      
        auto c = char_server::state(state_);
        c.login = p_->login();
        c.name = p_->server_name();
        c.ip_v4 = ip_v4;
        c.port = port;
        c.state_num = p_->state();
        c.property = p_->property();
        session_.state_.emplace<char_server::state>(std::move(c));
        server_.add(session_.shared_from_this());
      } else {
        log()->error("Connection refused for char server {}, session for login {} already exists (server name {})",
                     p_->server_name(), p_->login(), (*existing)->as_char_server().name);
        session_.emplace_and_send<packet::ATHENA_AH_LOGIN_RESULT>(3);
        throw network::terminate_session();
      }
    } else {
      log()->error("Connection refused for char server {}, wrong password", p_->server_name());
      session_.emplace_and_send<packet::ATHENA_AH_LOGIN_RESULT>(3);
      throw network::terminate_session();
    }
  } else {
    log()->error("Connection refused for char server {}, wrong login", p_->server_name());
    session_.emplace_and_send<packet::ATHENA_AH_LOGIN_RESULT>(3);
    throw network::terminate_session();
  }

  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_LOGIN_REQ: end");
}
