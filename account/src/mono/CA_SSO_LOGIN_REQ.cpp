#include <ares/common/random>

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
                             std::shared_ptr<session> sess,
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
          
          void accept(const std::string& login) {
            auto user_data = server_.db->query<database::accounts::user_data_for_login>(login);
            if (user_data) {
              SPDLOG_TRACE(log(), "login responder running accept procedure");
              std::unique_lock<std::mutex> l(server_.mutex());
              if (server_.num_char_servers_open() > 0) {
                auto found = server_.find_client_session(user_data->account_id);
                if (!found) {
                  log()->info("Accepting login for account id {} ('{}')", user_data->account_id.to_string(), login);
                  const auto auth_code1 = ares::random_int32::get();
                  const auto auth_code2 = ares::random_int32::get();

                  SPDLOG_TRACE(log(), "Creating client::state");
                  auto new_state = session_->as_mono();
                  session_->variant().emplace<client::state>(std::move(new_state));
                  client::state& c = session_->as_client();
                  c.account_id = user_data->account_id;
                  c.account_level = user_data->level;
                  c.clienttype = clienttype;
                  c.version = version;
                  SPDLOG_TRACE(log(), "Sending sex {}", user_data->sex);
                  c.sex = user_data->sex;
                  c.auth_code1 = auth_code1;
                  c.auth_code2 = auth_code2;

                  server_.add(session_);
                                      
                  session_->emplace_and_send<packet::current<packet::AC_ACCEPT_LOGIN>>
                    (auth_code1,
                     model::AID(user_data->account_id),
                     auth_code2,
                     0,
                     "",
                     user_data->sex,
                     server_.char_servers().size());

                  for (const auto& cs : server_.char_servers()) {
                    if (auto c = cs.lock()) {
                      const auto& data = c->as_char_server();
                      session_->emplace_and_send<packet::current<packet::AC_ACCEPT_LOGIN>::SERVER_ADDR>
                        (htonl(data.ip_v4.to_ulong()),
                         data.port,
                         data.name.c_str(),
                         data.user_count,
                         data.state_num,
                         data.property);
                    }
                  }
                } else {
                  log()->info("Session for account id {} ('{}') already exists, refusing the connection for new session {} and closing the existing one {}",
                              user_data->account_id.to_string(), login, (void*)session_.get(), (void*)found.get());
                  for (const auto& cs : server_.char_servers()) {
                    if (auto c = cs.lock()) c->emplace_and_send<packet::current<packet::ARES_AH_KICK_ACCOUNT>>(user_data->account_id);
                  }
                  session_->emplace_and_send<packet::current<packet::SC_NOTIFY_BAN>>(8);  // 08 = Server still recognizes your last login
                  session_->close_gracefuly();
                  // close_abruptly locks server mutex
                  l.unlock();
                  found->close_abruptly();
                }
              } else {
                log()->warn("Client request login, but no char server is available");
                session_->emplace_and_send<packet::current<packet::SC_NOTIFY_BAN>>(1);
                session_->close_gracefuly(); // 1 = Server closed
              }
            } else {
              log()->error("Could not get user data for login {} from database", login);
              session_->emplace_and_send<packet::current<packet::AC_REFUSE_LOGIN>>(3); // 3 = Rejected from Server
              session_->close_gracefuly();
            }
          }

          void check_login_password(const std::string& login, const std::string& password) {
            const auto& conf = server_.conf();
            if (conf.client_version) {
              if (conf.client_version != version) {
                log()->info("Login attempt failed for login {}, incorrect client version {}, expected {} (login/password auth)", login, version, *conf.client_version);
                session_->emplace_and_send<packet::current<packet::AC_REFUSE_LOGIN>>(5);
                session_->close_gracefuly();
                return;
              }
            }
            if (server_.db->query<database::accounts::password_matches>(login, password)) {
              accept(login);
            } else {
              log()->info("Login attempt failed for login {}, incorrect password (login/password auth)", login);
              session_->emplace_and_send<packet::current<packet::AC_REFUSE_LOGIN>>(1); // 1 = Incorrect Password
              session_->close_gracefuly();
            }
          }

          server& server_;
          std::shared_ptr<session> session_;
          const std::string pck_username;
          const std::string pck_password;
          const uint32_t version;
          const uint8_t clienttype;
        };
        
        struct ca_login_responder_lp : ca_login_responder {
          ca_login_responder_lp(server& serv,
                                std::shared_ptr<session> sess,
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
                                   std::shared_ptr<session> sess,
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

void ares::account::mono::packet_handler<ares::packet::current<ares::packet::CA_SSO_LOGIN_REQ::login_password>>::operator()() {
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ::login_password");
  auto pck_username = std::string(p_->ID(), std::remove_reference<decltype(p_->ID())>::type::size::value);
  auto pck_password = std::string(p_->Passwd(), std::remove_reference<decltype(p_->ID())>::type::size::value);
  auto zero_pos = pck_username.find(char(0));
  if (zero_pos != std::string::npos) pck_username.erase(zero_pos, std::string::npos);
  zero_pos = pck_password.find(char(0));
  if (zero_pos != std::string::npos) pck_password.erase(zero_pos, std::string::npos);
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ::login_password pck_username = " + pck_username + " pck_password " + pck_password);
  detail::ca_login_responder_lp respond(server_,
                                        session_.shared_from_this(),
                                        std::move(pck_username),
                                        std::move(pck_password),
                                        p_->Version(),
                                        p_->clienttype());
  respond();
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ::login_password end");
}

void ares::account::mono::packet_handler<ares::packet::current<ares::packet::CA_SSO_LOGIN_REQ::token_auth>>::operator()() {
  SPDLOG_TRACE(log(), "CA_SSO_LOGIN_REQ::token_auth");
  auto pck_username = std::string(p_->ID(), std::remove_reference<decltype(p_->ID())>::type::size::value);
  auto pck_password = std::string(p_->Passwd(), std::remove_reference<decltype(p_->Passwd())>::type::size::value);
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

