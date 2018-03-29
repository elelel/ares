#pragma once

#define ARES_DECLARE_PACKET_HANDLER_TEMPLATE()                          \
  template <typename Packet>                                            \
  struct packet_handler : ares::network::packet_handler<packet_handler<Packet>, \
                                                        Packet,         \
                                                        server,         \
                                                        session,        \
                                                        state> {        \
    using ares::network::packet_handler<packet_handler<Packet>,         \
                                        Packet,                         \
                                        server,                         \
                                        session,                        \
                                        state>::packet_handler;         \
  };                                                                    \


#define ARES_PACKET_HANDLER(NAME)                                       \
  template <>                                                           \
  struct packet_handler<packet::current<packet::NAME>> :                \
    ares::network::packet_handler<packet_handler<packet::current<packet::NAME>>, \
                                  packet::current<packet::NAME>,        \
                                  server,                               \
                                  session,                              \
                                  state> {                              \
    using ares::network::packet_handler<packet_handler<packet::current<packet::NAME>>, \
                                        packet::current<packet::NAME>,  \
                                        server,                         \
                                        session,                        \
                                        state>::packet_handler;         \
    void operator()();                                                  \
  };                                                                    \
  

#define ARES_DISPATCH_PACKET_CASE(NAME)                                 \
  case packet_sets::id_of<packet_sets::current, packet::NAME>::value: { \
    using packet_type = packet::current<packet::NAME>;                  \
    packet_handler<packet_type>(server_, session_, *this, buf)();       \
    break;                                                              \
  }                                                                     \


#define ARES_PACKET_SIZES_CASE(NAME)                                    \
  case packet_sets::id_of<packet_sets::current, packet::NAME>::value: { \
    using packet_type = packet::current<packet::NAME>;                  \
    return packet::size::allocate_size<packet_type>();                  \
  }                                                                     \
  
