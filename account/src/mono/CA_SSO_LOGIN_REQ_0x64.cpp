#include <ares/common>

#include "state.hpp"
#include "../state.hpp"
#include "../session.hpp"

#include "../char_server/state.hpp"

ares::account::mono::packet_handler<typename ares::packet::CA_SSO_LOGIN_REQ_0x64>::packet_handler(account::state& server_state, session& sess, state& st) :
  network::handler::packet::base<packet_handler<packet::CA_SSO_LOGIN_REQ_0x64>,
                                 packet::CA_SSO_LOGIN_REQ_0x64, account::state, session, state>(server_state, sess, st),
  pck_username(p_->ID(), sizeof(p_->ID_)),
  pck_password(p_->Passwd(), sizeof(p_->Passwd_)) {
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ_0x64 constructor");
  auto zero_pos = pck_username.find(char(0));
  if (zero_pos != std::string::npos) pck_username.erase(zero_pos, std::string::npos);
  zero_pos = pck_password.find(char(0));
  if (zero_pos != std::string::npos) pck_password.erase(zero_pos, std::string::npos);
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ_0x64 pck_username = " + pck_username);
}

void ares::account::mono::packet_handler<ares::packet::CA_SSO_LOGIN_REQ_0x64>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CA_SSO_LOGIN_REQ_0x64: begin");
  // Here will be switching for other login systems
  handle_default();
  SPDLOG_TRACE(log(), "handle_packet CA_SSO_LOGIN_REQ_0x64: end");
}

void ares::account::mono::packet_handler<ares::packet::CA_SSO_LOGIN_REQ_0x64>::refuse(const uint8_t error_code) {
  session_.emplace_and_send<packet::AC_REFUSE_LOGIN>(error_code);
  throw ares::network::terminate_session();
}

void ares::account::mono::packet_handler<ares::packet::CA_SSO_LOGIN_REQ_0x64>::notify_ban(const uint8_t error_code) {
  session_.emplace_and_send<packet::SC_NOTIFY_BAN>(error_code); 
  throw ares::network::terminate_session();
}

void ares::account::mono::packet_handler<ares::packet::CA_SSO_LOGIN_REQ_0x64>::accept(const std::string& login) {
  auto user_data = server_state_.db.user_data_for_login(login);
  if (user_data) {
    auto& serv = server_state_.server;
    SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ_0x64 accept: acquiring server lock");
    std::lock_guard lock(server_state_.server.mutex());
    SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ_0x64 accept: server lock acquired");
    if (serv.char_servers().size() > 0) {
      auto found = serv.client_by_aid(user_data->aid);
      if (!found) {
        log()->info("Accepting login for AID = {}", user_data->aid);
        const auto auth_code1 = ares::random_int32::get();
        const auto auth_code2 = ares::random_int32::get();

        SPDLOG_TRACE(log(), "Creating client::state");
        auto new_state = client::state(session_state_);
        session_.session_state_.emplace<client::state>(std::move(new_state));
        auto& client = session_.as_client();
        client.aid = user_data->aid;
        client.account_level = user_data->level;
        client.clienttype = p_->clienttype();
        client.version = p_->Version();
        client.sex = user_data->sex;
        client.auth_code1 = auth_code1;
        client.auth_code2 = auth_code2;
        serv.add(session_.shared_from_this());
        
        session_.emplace_and_send<packet::AC_ACCEPT_LOGIN>(auth_code1,
                                                           user_data->aid,
                                                           auth_code2,
                                                           0,
                                                           "",
                                                           user_data->sex,
                                                           serv.char_servers().size());

        for (const auto& c : serv.char_servers()) {
          if (c) {
            const auto& data = c->as_char_server();
            session_.emplace_and_send<packet::AC_ACCEPT_LOGIN::SERVER_ADDR>(htonl(data.ip_v4.to_ulong()),
                                                                            data.port,
                                                                            data.name.c_str(),
                                                                            data.user_count,
                                                                            data.state_num,
                                                                            data.property);
          }
        }
      } else {
        log()->info("AID {} session already exists, refusing new connection and kicking existing", user_data->aid);
        for (const auto& c : serv.char_servers()) {
          c->emplace_and_send<packet::ATHENA_AH_KICK_AID>(user_data->aid);
        }
        found->remove_from_server();
        notify_ban(8); // 08 = Server still recognizes your last login
      }
    } else {
      log()->warn("Client request login, but no char server is available");
      notify_ban(1); // 1 = Server closed
    }
  } else {
    log()->error("Could not get user data for login {} from database", login);
    refuse(3); // 3 = Rejected from Server
  }
}

void ares::account::mono::packet_handler<ares::packet::CA_SSO_LOGIN_REQ_0x64>::check(const std::string& login, const std::string& password) {
  const auto& conf = server_state_.conf;
  if (conf.client_version) {
    if (conf.client_version != p_->Version()) {
      log()->info("Login attempt failed for login {}, incorrect client version {}, expected {}", login, p_->Version(), *conf.client_version);
      refuse(5);
      return;
    }
  }
  if (server_state_.db.password_matches(login, password)) {
    accept(login);
  } else {
    log()->info("Login attempt failed for login {}, incorrect password", login);
    refuse(1); // 1 = Incorrect Password
  }
}
  
void ares::account::mono::packet_handler<ares::packet::CA_SSO_LOGIN_REQ_0x64>::handle_default() {
  check(pck_username, pck_password);
}
