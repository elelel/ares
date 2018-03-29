#include "session.hpp"

#include "server.hpp"

ares::zone::session::session(ares::zone::server& serv,
                             const std::optional<asio::ip::tcp::endpoint> connect_ep,
                             std::shared_ptr<asio::ip::tcp::socket> socket,
                             const std::chrono::seconds idle_timer_timeout) :
  ares::network::session<session, ares::zone::server>(serv, connect_ep, socket, idle_timer_timeout),
  session_state_(std::in_place_type<mono::state>, serv, *this) {
}

auto ares::zone::session::variant() -> state_variant& {
  return session_state_;
}

bool ares::zone::session::is_mono() const {
  return std::holds_alternative<mono::state>(session_state_);
}

auto ares::zone::session::as_mono() -> mono::state& {
  return std::get<mono::state>(session_state_);
}

bool ares::zone::session::is_client() const {
  return std::holds_alternative<client::state>(session_state_);
}

auto ares::zone::session::as_client() -> client::state& {
  return std::get<client::state>(session_state_);
}

bool ares::zone::session::is_char_server() const {
  return std::holds_alternative<character_server::state>(session_state_);
}

auto ares::zone::session::as_char_server() -> character_server::state& {
  return std::get<character_server::state>(session_state_);
}

#define ARES_VARIANT_EVENT_DISPATCHER(NAME)             \
  void ares::zone::session::NAME() {                    \
    struct visitor {                                    \
      visitor(session& s) :                             \
        s(s) {};                                        \
                                                        \
      void operator()(const mono::state&) {             \
        s.as_mono().NAME();                             \
      }                                                 \
                                                        \
      void operator()(const client::state&) {           \
        s.as_client().NAME();                           \
      }                                                 \
                                                        \
      void operator()(const character_server::state&) { \
        s.as_char_server().NAME();                      \
      }                                                 \
                                                        \
    private:                                            \
    session& s;                                         \
    };                                                  \
    std::visit(visitor(*this), variant());              \
  }                                                     \
  

ARES_VARIANT_EVENT_DISPATCHER(on_connect);
ARES_VARIANT_EVENT_DISPATCHER(on_connection_reset);
ARES_VARIANT_EVENT_DISPATCHER(on_operation_aborted);
ARES_VARIANT_EVENT_DISPATCHER(on_eof);
ARES_VARIANT_EVENT_DISPATCHER(on_socket_error);
ARES_VARIANT_EVENT_DISPATCHER(defuse_asio);

#undef ARES_VARIANT_EVENT_DISPATCHER

auto ares::zone::session::packet_sizes(uint16_t& packet_id) -> std::tuple<size_t, size_t, size_t> {
  struct visitor {
    visitor(session& s, uint16_t& packet_id) :
      s(s), packet_id(packet_id) {};

    std::tuple<size_t, size_t, size_t> operator()(const mono::state&) {
      return s.as_mono().packet_sizes(packet_id);
    }

    std::tuple<size_t, size_t, size_t> operator()(const character_server::state&) {
      return s.as_char_server().packet_sizes(packet_id);
    }
    
   std::tuple<size_t, size_t, size_t> operator()(const client::state&) {
      return s.as_client().packet_sizes(packet_id);
    }

  private:
    session& s;
    uint16_t& packet_id;
  };

  return std::visit(visitor(*this, packet_id), variant());
}

void ares::zone::session::dispatch_packet(std::shared_ptr<std::byte[]> buf) {
  struct visitor {
    visitor(session& s, std::shared_ptr<std::byte[]> buf) :
      s(s), buf(buf) {};

    void operator()(const mono::state&) {
      s.as_mono().dispatch_packet(buf);
    }

    void operator()(const character_server::state&) {
      s.as_char_server().dispatch_packet(buf);
    }
    
    void operator()(const client::state&) {
      s.as_client().dispatch_packet(buf);
    }

  private:
    session& s;
    std::shared_ptr<std::byte[]> buf;
  };

  return std::visit(visitor(*this, buf), variant());
}
