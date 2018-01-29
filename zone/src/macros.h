#pragma once

#define ARES_DECLARE_PACKET_HANDLER_TEMPLATE()                          \
  template <typename Packet>                                            \
  struct packet_handler : ares::network::handler::packet::base<packet_handler<Packet>, \
                                                               Packet,  \
                                                               zone::state, \
                                                               session, \
                                                               state> { \
    using ares::network::handler::packet::base<packet_handler<Packet>,  \
                                               Packet,                  \
                                               zone::state,             \
                                               session,                 \
                                               state>::base;            \
};                                                                    


#define ARES_SIMPLE_PACKET_HANDLER(NAME)                                \
  template <>                                                           \
  struct packet_handler<typename packet::NAME> :                        \
    ares::network::handler::packet::base<packet_handler<packet::NAME>,  \
                                         packet::NAME,                  \
                                         zone::state,                   \
                                         session,                       \
                                         state> {                       \
                                                 using ares::network::handler::packet::base<packet_handler<packet::NAME>, \
                                                                                            packet::NAME, \
                                                                                            zone::state, \
                                                                                            session, \
                                                                                            state>::base; \
                                                 void operator()();     \
                                                 };                     \

#define ARES_PACKET_CASE(NAME)                                          \
  case packet_id::NAME: { packet_handler<packet::NAME> h(server_state_, session_, *this); return h.handle(); } \

