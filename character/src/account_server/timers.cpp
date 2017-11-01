#include "timers.hpp"

#include "state.hpp"
#include "../session.hpp"
#include "../server.hpp"

void ares::character::account_server::timer::reconnect::on_timer() {
  if (!session_->connected()) {
    auto& server = session_->server_;
    std::lock_guard<std::mutex> lock(server.mutex());
    session_->socket() = std::make_shared<boost::asio::ip::tcp::socket>(*session_->io_service());
    boost::system::error_code ec;
    auto& ep = *server.conf().account_server->connect;
    log()->info("Connection to account server is not established. Connecting to {}:{}...",
                ep.address().to_string(), ep.port());
    session_->socket()->open(ep.protocol(), ec);

    if (ec.value() == 0) {
      auto session_copy = session_;
      session_->socket()->async_connect(ep, [session_copy] (const boost::system::error_code ec) {
          auto& server = session_copy->server_;
          std::lock_guard<std::mutex> lock(server.mutex());
          if (ec.value() == 0) {
            session_copy->set_connected();
            server.log()->info("Starting handshake with account server");
            const auto my_ipv4 = server.conf().listen_ipv4[0].address().to_v4().to_ulong();
            const auto my_port = server.conf().listen_ipv4[0].port();
            session_copy->emplace_and_send<packet::ATHENA_HA_LOGIN_REQ>(server.conf().account_server->login,
                                                                        server.conf().account_server->password,
                                                                        htonl(my_ipv4),
                                                                        htons(my_port),
                                                                        *server.conf().server_name,
                                                                        server.state_num,
                                                                        server.property);
            session_copy->receive();
            session_copy->as_account_server().ping_request_timer.set();    
          } else {
            session_copy->close_socket();
            server.log()->warn("Connecting socket to account server failed. {}, {}", ec.value(), ec.message());
          }
          session_copy->as_account_server().reconnect_timer.set();
        });
    } else {
      log()->error("Opening socket to account server failed. {}, {}", ec.value(), ec.message());
    }
  } else {
    SPDLOG_TRACE(log(), "Account server is still connected, reconnect is not needed");
    session_->as_account_server().reconnect_timer.set();
  }
}

void ares::character::account_server::timer::send_aids::on_timer() {
  std::lock_guard<std::mutex> lock(session_->server_.mutex());
  SPDLOG_TRACE(log(), "sending {} online aids", session_->server_.clients().size());
  session_->emplace_and_send<packet::ATHENA_HA_ONLINE_AIDS>(session_->server_.clients().size());
  SPDLOG_TRACE(log(), "online aids header sent" );
  for (const auto& p : session_->server_.clients()) {
    session_->copy_and_send(&p.first, sizeof(p.first));
  }
  session_->as_account_server().send_aids_timer.set();  
}

void ares::character::account_server::timer::send_user_count::on_timer() {
  std::lock_guard<std::mutex> lock(session_->server_.mutex());
  session_->emplace_and_send<packet::ATHENA_HA_USER_COUNT>(session_->server_.clients().size());
  session_->as_account_server().send_user_count_timer.set();  
}

void ares::character::account_server::timer::ping_request::on_timer() {
  auto& server = session_->server_;
  std::lock_guard<std::mutex> lock(server.mutex());
  if (server.account_server() &&
      (server.account_server() == session_) &&
      session_->connected()) {
    SPDLOG_TRACE(log(), "ping_request_handler sending ping to account server");
    session_->emplace_and_send<packet::ATHENA_HA_PING_REQ>();
    session_->as_account_server().ping_timeout_timer.set();
  } else {
    SPDLOG_TRACE(log(),
                 "ping_request_handler not sending ping to account server, not connected."
                 "server.account_server() == {}, session_ == {}, session_->connected() = {}",
                 (uintptr_t)server.account_server().get(),
                 (uintptr_t)session_.get(),
                 session_->connected());
  }
}

void ares::character::account_server::timer::ping_timeout::on_timer() {
  auto& server = session_->server_;
  std::lock_guard<std::mutex> lock(server.mutex());
  if (server.account_server() &&
      (server.account_server() == session_) &&
      session_->connected()) {
    log()->error("Timeout while waiting for ping response from account server, closing session");
    session_->close_socket();
  }
}
