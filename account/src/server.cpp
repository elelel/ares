#include "server.hpp"

#include "session.hpp"

thread_local std::shared_ptr<ares::database::db> ares::account::server::db = nullptr;

ares::account::server::server(std::shared_ptr<spdlog::logger> log,
                              std::shared_ptr<asio::io_context> io_context,
                              const ares::account::config& conf) :
  ares::network::server<server, session>(log, io_context, *conf.network_threads),
  conf_(conf) {
  log_->set_level(spdlog::level::trace);
}

void ares::account::server::init_thread_local() {
  db = std::make_shared<ares::database::db>(log_, conf_.postgres->dbname, conf_.postgres->host, conf_.postgres->port, conf_.postgres->user, conf_.postgres->password);
}

void ares::account::server::start() {
  if (conf_.listen_ipv4.size() > 0) {
    for (const auto& listen : conf_.listen_ipv4) {
      ares::network::server<server, session>::start(listen);
    }
  } else {
    log()->error("Can't start: listen ipv4 configuration is empty");
  }
}

void ares::account::server::create_session(std::shared_ptr<asio::ip::tcp::socket> socket) {
  SPDLOG_TRACE(log_, "account::server::create_session");
  auto s = std::make_shared<session>(*this, std::optional<asio::ip::tcp::endpoint>{}, socket, std::chrono::seconds{120});
  mono_.insert(s);
  s->receive();
  s->reset_idle_timer();      
}

auto ares::account::server::conf() const -> const config& {
  return conf_;
}

void ares::account::server::add(std::shared_ptr<session> s) {
  struct visitor {
    visitor(server& serv, const std::shared_ptr<session>& s) :
      serv_(serv), s_(s) {};

    void operator()(const mono::state&) {
      serv_.mono_.insert(s_);
    }

    void operator()(const client::state&) {
      serv_.clients_.insert({s_->as_client().account_id, s_});
      serv_.mono_.erase(s_);
    }

    void operator()(const character_server::state&) {
      serv_.char_servers_.insert(s_);
      serv_.mono_.erase(s_);
    }
    
  private:
    server& serv_;
    std::shared_ptr<session> s_;
  };
  std::visit(visitor(*this, s), s->variant());
}

void ares::account::server::remove(std::shared_ptr<session> s) {
  struct visitor {
    visitor(server& serv, const std::shared_ptr<session>& s) :
      serv_(serv), s_(s) {};

    void operator()(const mono::state&) {
      serv_.mono_.erase(s_);
    }

    void operator()(const client::state&) {
      serv_.clients_.erase(s_->as_client().account_id);
    }

    void operator()(const character_server::state&) {
      serv_.char_servers_.erase(s_);
    }
  private:
    server& serv_;
    std::shared_ptr<session> s_;
  };
  std::visit(visitor(*this, s), s->variant());
}

auto ares::account::server::find_client_session(const model::account_id& account_id) const -> std::shared_ptr<session> {
  auto found = clients_.find(account_id);
  if (found != clients_.end()) {
    if (auto s = found->second.lock()) return s;
  }
  return nullptr;
}

void ares::account::server::link_to_char_server(const model::account_id& account_id, std::shared_ptr<session> s) {
  account_id_to_char_server_[account_id] = s;
}

void ares::account::server::unlink_from_char_server(const model::account_id& account_id, std::shared_ptr<session> s) {
  auto found = account_id_to_char_server_.find(account_id);
  if (found != account_id_to_char_server_.end()) {
    if (found->second.lock() == s) {
      account_id_to_char_server_.erase(account_id);
    } else {
      log_->warn("unlink_account_id_from_char_server account_id is linked to another char server session, not unlinking");
    }
  }
}

size_t ares::account::server::num_char_servers_open() const {
  // For now just return the total number of character servers connected
  return char_servers_.size();
}

auto ares::account::server::char_servers() const -> const std::set<std::weak_ptr<session>>& {
  return char_servers_;
}

auto ares::account::server::char_server_by_login(const std::string& login) const -> std::shared_ptr<session> {
  auto found = std::find_if(char_servers_.begin(), char_servers_.end(), [&login] (const std::weak_ptr<session>& s) {
      return s.lock()->as_char_server().login == login;
    });
  if (found != char_servers_.end()) {
    if (auto s = found->lock()) return s;
  }
  return nullptr;
}
