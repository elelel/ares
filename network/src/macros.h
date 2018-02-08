#pragma once

#define ARES_DECLARE_PACKET_HANDLER_TEMPLATE()                          \
  template <typename PacketSet, typename PacketName>                    \
  struct packet_handler : ares::network::packet_handler<packet_handler<PacketSet, PacketName>, \
                                                        PacketSet,      \
                                                        PacketName,     \
                                                        server,         \
                                                        session,        \
                                                        state> {        \
    using ares::network::packet_handler<packet_handler<PacketSet, PacketName>, \
                                        PacketSet,                      \
                                        PacketName,                     \
                                        server,                         \
                                        session,                        \
                                        state>::packet_handler;         \
  };                                                                    

#define ARES_SIMPLE_PACKET_HANDLER(NAME)                                \
  template <>                                                           \
  struct packet_handler<packet_set, packet::NAME> :                     \
    ares::network::packet_handler<packet_handler<packet_set, packet::NAME>, \
                                  packet_set,                           \
                                  packet::NAME,                         \
                                  server,                               \
                                  session,                              \
                                  state> {                              \
    using ares::network::packet_handler<packet_handler<packet_set, packet::NAME>, \
                                        packet_set,                     \
                                        packet::NAME,                   \
                                        server,                         \
                                        session,                        \
                                        state>::packet_handler;         \
    void operator()();                                                  \
  };                                                                    \

#define ARES_PACKET_CASE(NAME)                                          \
  case packet_sets::id_of<packet_set, packet::NAME>::value: {           \
    packet_handler<packet_set, packet::NAME> h(server_, session_, *this); return h.handle(); \
  }                                                                     \

