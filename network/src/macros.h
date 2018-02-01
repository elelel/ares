#pragma once

#define ARES_DECLARE_PACKET_HANDLER_TEMPLATE(SERVER)                    \
  template <typename Packet>                                            \
  struct packet_handler : ares::network::handler::packet::base<packet_handler<Packet>, \
    Packet,                                                             \
    SERVER::state,                                                      \
    session,                                                            \
    state> {                                                            \
    using ares::network::handler::packet::base<packet_handler<Packet>,  \
      Packet,                                                           \
      SERVER::state,                                                    \
      session,                                                          \
      state>::base;                                                     \
  };                                                                    

#define ARES_SIMPLE_PACKET_HANDLER(SERVER, NAME)                        \
  template <>                                                           \
  struct packet_handler<packet<packets::NAME>> :                        \
  ares::network::handler::packet::base<packet_handler<packet<packets::NAME>>, \
    packet<packets::NAME>,                                              \
    SERVER::state,                                                      \
    session,                                                            \
    state> {                                                            \
    using ares::network::handler::packet::base<packet_handler<packet<packets::NAME>>, \
      packet<packets::NAME>,                                            \
      SERVER::state,                                                    \
      session,                                                          \
      state>::base;                                                     \
    void operator()();                                                  \
  };                                                                    \

#define ARES_PACKET_CASE(NAME)                                          \
  case packet_set::template id_of<packet<packets::NAME>>::value: {      \
    packet_handler<packet<packets::NAME>> h(server_state_, session_, *this); return h.handle(); \
  }                                                                     \

