#include <ares/common>

#include "state.hpp"
#include "../server.hpp"
#include "../session.hpp"

#include "../character_server/state.hpp"

namespace ares {
  namespace account {
    namespace mono {
      namespace detail {
        struct ca_login_responder {
        protected:          
          ca_login_responder(server& serv,
                             session_ptr sess,
                             std::string&& pck_username,
                             std::string&& pck_password,
                             const uint32_t version,
                             const uint8_t clienttype) :
            server_(serv),
            session_(sess),
            pck_username(pck_username),
            pck_password(pck_password),
            version(version),
            clienttype(clienttype) {
          }

          ca_login_responder(const ca_login_responder&) = delete;
          ca_login_responder& operator=(const ca_login_responder&) = delete;
          ca_login_responder(ca_login_responder&&) = default;

          std::shared_ptr<spdlog::logger> log() const {
            return server_.log();
          }

          void refuse(const uint8_t error_code) {
            session_->emplace_and_send<packet::type<packet_set, packet::AC_REFUSE_LOGIN>>(error_code);
            server_.close_gracefuly(session_->shared_from_this());
          }
          
          void notify_ban(const uint8_t error_code) {
            session_->emplace_and_send<packet::type<packet_set, packet::SC_NOTIFY_BAN>>(error_code); 
            server_.close_gracefuly(session_->shared_from_this());
          }
          
          void accept(const std::string& login) {
            auto user_data = server_.db.user_data_for_login(login);
            if (user_data) {
              server_.
                on_rxthreads
                ([this, user_data = std::move(user_data)] () {
                  SPDLOG_TRACE(log(), "login responder running accept procedure");
                  if (server_.char_servers().size() > 0) {
                    auto found = server_.client_by_aid(user_data->aid);
                    if (!found) {
                      log()->info("Accepting login for AID = {}", user_data->aid);
                      const auto auth_code1 = ares::random_int32::get();
                      const auto auth_code2 = ares::random_int32::get();

                      SPDLOG_TRACE(log(), "Creating client::state");
                      auto new_state = client::state(session_->as_mono());
                      session_->variant().emplace<client::state>(std::move(new_state));
                      auto& client = session_->as_client();
                      client.aid = user_data->aid;
                      client.account_level = user_data->level;
                      client.clienttype = clienttype;
                      client.version = version;
                      client.sex = user_data->sex;
                      client.auth_code1 = auth_code1;
                      client.auth_code2 = auth_code2;
                      server_.add(session_);
        
                      session_->emplace_and_send<packet::type<packet_set, packet::AC_ACCEPT_LOGIN>>
                        (auth_code1,
                         user_data->aid,
                         auth_code2,
                         0,
                         "",
                         user_data->sex,
                         server_.char_servers().size());

                      for (const auto& c : server_.char_servers()) {
                        if (c) {
                          const auto& data = c->as_char_server();
                          session_->emplace_and_send<packet::type<packet_set, packet::AC_ACCEPT_LOGIN>::SERVER_ADDR>
                            (htonl(data.ip_v4.to_ulong()),
                             data.port,
                             data.name.c_str(),
                             data.user_count,
                             data.state_num,
                             data.property);
                        }
                      }
                    } else {
                      log()->info("AID {} session already exists, refusing new connection and kicking existing", user_data->aid);
                      for (const auto& c : server_.char_servers()) {
                        c->emplace_and_send<packet::type<packet_set, packet::ATHENA_AH_KICK_AID>>(user_data->aid);
                      }
                      // Close the other existing session for this aid
                      server_.close_gracefuly(found);
                      notify_ban(8); // 08 = Server still recognizes your last login
                    }
                  } else {
                    log()->warn("Client request login, but no char server is available");
                    notify_ban(1); // 1 = Server closed
                  }
                });
            } else {
              log()->error("Could not get user data for login {} from database", login);
              refuse(3); // 3 = Rejected from Server
            }
          }

          void check_login_password(const std::string& login, const std::string& password) {
            const auto& conf = server_.conf();
            if (conf.client_version) {
              if (conf.client_version != version) {
                log()->info("Login attempt failed for login {}, incorrect client version {}, expected {} (login/password auth)", login, version, *conf.client_version);
                refuse(5);
                return;
              }
            }
            if (server_.db.password_matches(login, password)) {
              accept(login);
            } else {
              log()->info("Login attempt failed for login {}, incorrect password (login/password auth)", login);
              refuse(1); // 1 = Incorrect Password
            }
          }

          server& server_;
          session_ptr session_;
          const std::string pck_username;
          const std::string pck_password;
          const uint32_t version;
          const uint8_t clienttype;
        };
        
        struct ca_login_responder_lp : ca_login_responder {
          ca_login_responder_lp(server& serv,
                                session_ptr sess,
                                std::string&& pck_username,
                                std::string&& pck_password,
                                const uint32_t version,
                                const uint8_t clienttype) :
            ca_login_responder(serv, sess, std::move(pck_username), std::move(pck_password), version, clienttype) {
          }
            
          void operator()() {
            check_login_password(pck_username, pck_password);
          }
        };

        struct ca_login_responder_token : ca_login_responder {
          ca_login_responder_token(server& serv,
                                   session_ptr sess,
                                   std::string&& pck_username,
                                   std::string&& pck_password,
                                   std::string&& pck_token,
                                   const uint32_t version,
                                   const uint8_t clienttype) :
            ca_login_responder(serv, sess, std::move(pck_username), std::move(pck_password), version, clienttype),
            pck_token(std::move(pck_token)) {
          }

          void operator()() {
            // TODO: make token-specific auth, then use check_login_password as fallback
            check_login_password(pck_username, pck_password);
          }
          
        private:
          const std::string pck_token;

        };

      }
    }
  }
}

void ares::account::mono::packet_handler<ares::packet_set, ares::packet::CA_SSO_LOGIN_REQ::login_password>::operator()() {
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ::login_password");
  auto pck_username = std::string(p_->ID(), p_->ID_size());
  auto pck_password = std::string(p_->Passwd(), p_->Passwd_size());
  auto zero_pos = pck_username.find(char(0));
  if (zero_pos != std::string::npos) pck_username.erase(zero_pos, std::string::npos);
  zero_pos = pck_password.find(char(0));
  if (zero_pos != std::string::npos) pck_password.erase(zero_pos, std::string::npos);
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ::login_password pck_username = " + pck_username + " pck_password " = pck_password);
  detail::ca_login_responder_lp respond(server_,
                                        session_.shared_from_this(),
                                        std::move(pck_username),
                                        std::move(pck_password),
                                        p_->Version(),
                                        p_->clienttype());
  respond();
}

void ares::account::mono::packet_handler<ares::packet_set, ares::packet::CA_SSO_LOGIN_REQ::token_auth>::operator()() {
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ::login_password");
  auto pck_username = std::string(p_->ID(), p_->ID_size());
  auto pck_password = std::string(p_->Passwd(), p_->Passwd_size());
  auto zero_pos = pck_username.find(char(0));
  if (zero_pos != std::string::npos) pck_username.erase(zero_pos, std::string::npos);
  zero_pos = pck_password.find(char(0));
  if (zero_pos != std::string::npos) pck_password.erase(zero_pos, std::string::npos);
  const auto pck_token_len = p_->PacketLength - sizeof(handled_packet_type);
  auto pck_token = std::string(p_->token(), pck_token_len);
  zero_pos = pck_token.find(char(0));
  if (zero_pos != std::string::npos) pck_token.erase(zero_pos, std::string::npos);
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ::token_auth pck_username = " + pck_username + ", pck_token = " + pck_token);
  detail::ca_login_responder_token respond(server_,
                                           session_.shared_from_this(),
                                           std::move(pck_username),
                                           std::move(pck_password),
                                           std::move(pck_token),
                                           p_->Version(),
                                           p_->clienttype());
  respond();
}

