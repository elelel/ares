#include "state.hpp"
#include "../state.hpp"
#include "../zone_server/state.hpp"

void ares::character::mono::packet_handler<ares::packet::ATHENA_ZH_LOGIN_REQ>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_ZH_LOGIN_REQ: begin");
  // TODO: Check if char server isn't closed
  auto& server = server_state_.server;
  auto& conf = server_state_.conf;
  SPDLOG_TRACE(log(), "ATHENA_ZH_LOGIN_REQ acquiring server lock");
  std::lock_guard lock(server.mutex());
  SPDLOG_TRACE(log(), "ATHENA_ZH_LOGIN_REQ server lock acquired");
  
  auto found = std::find_if(conf.zone_servers.begin(),
                            conf.zone_servers.end(),
                            [this] (const config::zone_server_config_record &c) {
                              return c.login == p_->login();
                            });
  if (found != conf.zone_servers.end()) {
    if (found->password == p_->password()) {
      auto existing = find_if(server.zone_servers().begin(), server.zone_servers().end(), [this] (const session_ptr s) {
          return s->as_zone_server().login == p_->login();
        });
      if (existing == server.zone_servers().end()) {
        auto new_state = zone_server::state(session_state_);
        session_.session_state_.emplace<zone_server::state>(std::move(new_state));
        auto& server = session_.server_state_.server;
        auto& conf = session_.server_state_.conf;
        auto& zone = session_.as_zone_server();
        zone.ip = p_->ip();
        zone.port = p_->port();
        server.add(session_.shared_from_this());
        log()->info("Zone server accepted");
        session_.emplace_and_send<packet::ATHENA_HZ_LOGIN_RESULT>(0);
        session_.emplace_and_send<packet::ATHENA_HZ_PRIVATE_MSG_NAME>(*conf.priv_msg_server_name);
      } else {
        log()->error("Connection refused for zone server, connection already exists for login {}", p_->login());
        session_.emplace_and_send<packet::ATHENA_HZ_LOGIN_RESULT>(3);
        throw ares::network::terminate_session();
      }
    } else {
      log()->error("Connection refused for zone server, wrong password for login {}", p_->login());
      session_.emplace_and_send<packet::ATHENA_HZ_LOGIN_RESULT>(3);
      throw ares::network::terminate_session();
    }
  } else {
    log()->error("Connection refused for zone server, wrong login {}", p_->login());
    session_.emplace_and_send<packet::ATHENA_HZ_LOGIN_RESULT>(3);
    throw ares::network::terminate_session();
  }

  SPDLOG_TRACE(log(), "handle_packet ATHENA_ZH_LOGIN_REQ: end");
}
