#include "state.hpp"
#include "../server.hpp"
#include "../session.hpp"

void ares::account::mono::packet_handler<ares::packet_set, ares::packet::ATHENA_HA_LOGIN_REQ>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_LOGIN_REQ: begin");
  asio::ip::address_v4 ip_v4(ntohl(p_->ip()));
  uint16_t port(ntohs(p_->port()));
  log()->info("Connection request from char server {} state {}, invites to listening point at {}:{}", p_->server_name(), p_->state(), ip_v4.to_string(), port);

  // TODO: Check if account server isn't closed

  auto& serv = server_;
  auto& conf = server_.conf();
  SPDLOG_TRACE(log(), "ATHENA_HA_LOGIN_REQ acquiring server lock");
  std::lock_guard lock(serv.mutex());
  SPDLOG_TRACE(log(), "ATHENA_HA_LOGIN_REQ server lock acquired");
  auto found = std::find_if(conf.char_servers.begin(),
                            conf.char_servers.end(),
                            [this] (const config::char_server_config_record &c) {
                              return c.login == p_->login();
                            });
  if (found != conf.char_servers.end()) {
    if (found->password == p_->password()) {
      auto existing = find_if(serv.char_servers().begin(), serv.char_servers().end(), [this] (const session_ptr s) {
          return s->as_char_server().login == p_->login();
        });
      if (existing == serv.char_servers().end()) {
        log()->info("Char server {} accepted", p_->server_name());
        session_.emplace_and_send<packet::type<packet_set, packet::ATHENA_AH_LOGIN_RESULT>>(0);
      
        auto new_state = character_server::state(state_);
        session_.variant().emplace<character_server::state>(std::move(new_state));
        auto& char_serv = session_.as_char_server();
        char_serv.login = p_->login();
        char_serv.name = p_->server_name();
        char_serv.ip_v4 = ip_v4;
        char_serv.port = port;
        char_serv.state_num = p_->state();
        char_serv.property = p_->property();
        auto s = session_.shared_from_this();        
        server_.on_rxthreads([this, s] () { server_.add(s); });
      } else {
        log()->error("Connection refused for char server '{}', session for login '{}' already exists (server name {})",
                     p_->server_name(), p_->login(), (*existing)->as_char_server().name);
        session_.emplace_and_send<packet::type<packet_set, packet::ATHENA_AH_LOGIN_RESULT>>(3);
        server_.close_gracefuly(session_.shared_from_this());
      }
    } else {
      log()->error("Connection refused for char server '{}', wrong password", p_->server_name());
      session_.emplace_and_send<packet::type<packet_set, packet::ATHENA_AH_LOGIN_RESULT>>(3);
      server_.close_gracefuly(session_.shared_from_this());
    }
  } else {
    log()->error("Connection refused for char server '{}', wrong login", p_->server_name());
    session_.emplace_and_send<packet::type<packet_set, packet::ATHENA_AH_LOGIN_RESULT>>(3);
    server_.close_gracefuly(session_.shared_from_this());
  }

  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_LOGIN_REQ: end");
}
