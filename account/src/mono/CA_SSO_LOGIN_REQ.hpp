// For inclusion from packet_handlers.hpp only

template <>
struct packet_handler<packet_set, packet::CA_SSO_LOGIN_REQ::login_password>
  : network::handler::packet::base<packet_handler<packet_set, packet::CA_SSO_LOGIN_REQ::login_password>,
                                   packet_set,
                                   packet::CA_SSO_LOGIN_REQ::login_password,
                                   account::state, session, state> {
  
  packet_handler(account::state&, session&, state&);

  void operator()();

private:
  std::string pck_username;
  std::string pck_password;

  void refuse(const uint8_t error_code);
  void notify_ban(const uint8_t error_code);
  void accept(const std::string& login);
  void check(const std::string& login, const std::string& password);
  void handle_default();
};

template <>
struct packet_handler<packet_set, packet::CA_SSO_LOGIN_REQ::token_auth>
  : network::handler::packet::base<packet_handler<packet_set, packet::CA_SSO_LOGIN_REQ::token_auth>,
                                   packet_set,
                                   packet::CA_SSO_LOGIN_REQ::token_auth,
                                   account::state, session, state> {
  
  packet_handler(account::state&, session&, state&);

  void operator()();

private:
  std::string pck_username;
  std::string pck_password;
  std::string pck_token;

  void refuse(const uint8_t error_code);
  void notify_ban(const uint8_t error_code);
  void accept(const std::string& login);
  void check(const std::string& login, const std::string& password);
  void handle_default();
};
