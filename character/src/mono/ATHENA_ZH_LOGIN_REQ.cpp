#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"
#include "../zone_server/state.hpp"

void ares::character::mono::packet_handler<ares::packet::ATHENA_ZH_LOGIN_REQ>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_ZH_LOGIN_REQ: begin");
  // TODO: Check if char server isn't closed
  SPDLOG_TRACE(log(), "ATHENA_ZH_LOGIN_REQ acquiring server lock");
  std::lock_guard lock(server_.mutex());
  SPDLOG_TRACE(log(), "ATHENA_ZH_LOGIN_REQ server lock acquired");
  
  auto found = std::find_if(server_.conf().zone_servers.begin(),
                            server_.conf().zone_servers.end(),
                            [this] (const config::zone_server_config_record &c) {
                              return c.login == p_->login();
                            });
  if (found != server_.conf().zone_servers.end()) {
    if (found->password == p_->password()) {
      auto existing = find_if(server_.zone_servers().begin(), server_.zone_servers().end(), [this] (const session_ptr s) {
          return s->as_zone_server().login == p_->login();
        });
      if (existing == server_.zone_servers().end()) {
        auto new_state = zone_server::state(state_);
        session_.state_.emplace<zone_server::state>(std::move(new_state));
        auto& zone = session_.as_zone_server();
        zone.ip = p_->ip();
        zone.port = p_->port();
        server_.add(session_.shared_from_this());
        log()->info("Zone server accepted");
        session_.emplace_and_send<packet::ATHENA_HZ_LOGIN_RESULT>(0);
        session_.emplace_and_send<packet::ATHENA_HZ_PRIVATE_MSG_NAME>(*server_.conf().priv_msg_server_name);
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
