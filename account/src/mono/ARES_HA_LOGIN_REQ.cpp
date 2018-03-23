#include "state.hpp"
#include "../server.hpp"
#include "../session.hpp"

void ares::account::mono::packet_handler<ares::packet::current<ares::packet::ARES_HA_LOGIN_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HA_LOGIN_REQ: begin");
  asio::ip::address_v4 ip_v4(ntohl(p_->ip()));
  uint16_t port(ntohs(p_->port()));
  log()->info("Connection request from char server {} state {}, invites to listening point at {}:{}", std::string(p_->server_name()), p_->state(), ip_v4.to_string(), port);

  // TODO: Check if account server isn't closed

  auto& serv = server_;
  auto& conf = server_.conf();
  const auto login = std::string((char*)p_->login().data());
  SPDLOG_TRACE(log(), "ARES_HA_LOGIN_REQ acquiring server lock");
  std::lock_guard<std::mutex> lock(serv.mutex());
  SPDLOG_TRACE(log(), "ARES_HA_LOGIN_REQ server lock acquired");
  auto found = std::find_if(conf.char_servers.begin(),
                            conf.char_servers.end(),
                            [&login] (const config::char_server_config_record &c) {
                              return login == c.login;
                            });
  if (found != conf.char_servers.end()) {
    if (p_->password() == found->password) {
      auto existing = server_.char_server_by_login(login);
      if (existing == nullptr) {
        log()->info("Char server {} accepted", std::string(p_->server_name()));
        session_.emplace_and_send<packet::current<packet::ARES_AH_LOGIN_RESULT>>(0);
      
        auto new_state = character_server::state(state_);
        session_.variant().emplace<character_server::state>(std::move(new_state));
        auto& char_serv = session_.as_char_server();
        char_serv.login = login;
        char_serv.name = p_->server_name();
        char_serv.ip_v4 = ip_v4;
        char_serv.port = port;
        char_serv.state_num = p_->state();
        char_serv.property = p_->property();
        auto s = session_.shared_from_this();
        server_.add(s);
      } else {
        log()->warn("Got login request from character server with login '{}', name '{}', but connection for this login already exist (existing server name '{}'), closing both sessions",
                    std::string(p_->server_name()), login, existing->as_char_server().name);
        session_.emplace_and_send<packet::current<packet::ARES_AH_LOGIN_RESULT>>(3);
        existing->close_gracefuly();
        session_.close_gracefuly();
      }
    } else {
      log()->error("Connection refused for char server '{}', wrong password", std::string(p_->server_name()));
      session_.emplace_and_send<packet::current<packet::ARES_AH_LOGIN_RESULT>>(3);
      session_.close_gracefuly();
    }
  } else {
    log()->error("Connection refused for char server '{}', wrong login", std::string(p_->server_name()));
    session_.emplace_and_send<packet::current<packet::ARES_AH_LOGIN_RESULT>>(3);
    session_.close_gracefuly();
  }

  SPDLOG_TRACE(log(), "handle_packet ARES_HA_LOGIN_REQ: end");
}
