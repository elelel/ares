#pragma once

#define ARES_DECLARE_PACKET_HANDLER_TEMPLATE(SERVER)                    \
  template <typename PacketSet, typename PacketName>                    \
  struct packet_handler : ares::network::handler::packet::base<packet_handler<PacketSet, PacketName>, \
                                                               PacketSet, \
                                                               PacketName, \
                                                               SERVER::state, \
                                                               session, \
                                                               state> { \
    using ares::network::handler::packet::base<packet_handler<PacketSet, PacketName>, \
                                               PacketSet,               \
                                               PacketName,              \
                                               SERVER::state,           \
                                               session,                 \
                                               state>::base;            \
  };                                                                    

#define ARES_SIMPLE_PACKET_HANDLER(SERVER, NAME)                        \
  template <>                                                           \
  struct packet_handler<packet_set, packet::NAME> :                    \
    ares::network::handler::packet::base<packet_handler<packet_set, packet::NAME>, \
                                         packet_set,                    \
                                         packet::NAME,                 \
                                         SERVER::state,                 \
                                         session,                       \
                                         state> {                       \
    using ares::network::handler::packet::base<packet_handler<packet_set, packet::NAME>, \
                                               packet_set,              \
                                               packet::NAME,           \
                                               SERVER::state,           \
                                               session,                 \
                                               state>::base;            \
    void operator()();                                                  \
  };                                                                    \

#define ARES_PACKET_CASE(NAME)                                          \
  case packet_sets::id_of<packet_set, packet::NAME>::value: {          \
    packet_handler<packet_set, packet::NAME> h(server_state_, session_, *this); return h.handle(); \
  }                                                                     \

