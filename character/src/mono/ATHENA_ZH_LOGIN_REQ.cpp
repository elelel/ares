#include "state.hpp"
#include "../server.hpp"
#include "../zone_server/state.hpp"

void ares::character::mono::packet_handler<ares::packet::current<ares::packet::ATHENA_ZH_LOGIN_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_ZH_LOGIN_REQ: begin");
  // TODO: Check if char server isn't closed
  auto& conf = server_.conf();
  SPDLOG_TRACE(log(), "ATHENA_ZH_LOGIN_REQ acquiring server lock");
  std::lock_guard lock(server_.mutex());
  SPDLOG_TRACE(log(), "ATHENA_ZH_LOGIN_REQ server lock acquired");
  
  auto found = std::find_if(conf.zone_servers.begin(),
                            conf.zone_servers.end(),
                            [this] (const config::zone_server_config_record &c) {
                              return p_->login() == c.login;
                            });
  if (found != conf.zone_servers.end()) {
    if (p_->password() == found->password) {
      auto existing = find_if(server_.zone_servers().begin(), server_.zone_servers().end(), [this] (const session_ptr s) {
          return  p_->login() == s->as_zone_server().login;
        });
      if (existing == server_.zone_servers().end()) {
        session_.variant().emplace<zone_server::state>(std::move(zone_server::state(session_.as_mono()))); 
        auto& server = session_.server_;
        auto& conf = server.conf();
        auto& zone = session_.as_zone_server();
        zone.ip_v4 = asio::ip::address_v4(ntohl(p_->ip()));
        zone.port = ntohs(p_->port());
        SPDLOG_TRACE(log(), "Initializing maps to send to {} items", found->maps.size());
        std::copy(server_.zone_login_to_maps[std::string(p_->login())].begin(),
                  server_.zone_login_to_maps[std::string(p_->login())].end(),
                  std::back_inserter(zone.maps_to_send));
        server_.add(session_.shared_from_this());
        log()->info("Zone server accepted");
        session_.emplace_and_send<packet::current<packet::ATHENA_HZ_LOGIN_RESULT>>(0);
        session_.emplace_and_send<packet::current<packet::ATHENA_HZ_PRIVATE_MSG_NAME>>(*conf.priv_msg_server_name);

      } else {
        log()->error("Connection refused for zone server, connection already exists for login {}", p_->login().data());
        session_.emplace_and_send<packet::current<packet::ATHENA_HZ_LOGIN_RESULT>>(3);
        session_.close_gracefuly();
      }
    } else {
      log()->error("Connection refused for zone server, wrong password for login {}", p_->login().data());
      session_.emplace_and_send<packet::current<packet::ATHENA_HZ_LOGIN_RESULT>>(3);
      session_.close_gracefuly();
    }
  } else {
    log()->error("Connection refused for zone server, wrong login {}", p_->login().data());
    session_.emplace_and_send<packet::current<packet::ATHENA_HZ_LOGIN_RESULT>>(3);
    session_.close_gracefuly();
  }

  SPDLOG_TRACE(log(), "handle_packet ATHENA_ZH_LOGIN_REQ: end");
}
