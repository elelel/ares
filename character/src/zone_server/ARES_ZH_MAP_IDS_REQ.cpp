#include "state.hpp"
#include "../server.hpp"

void ares::character::zone_server::packet_handler<ares::packet::current<ares::packet::ARES_ZH_MAP_IDS_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "ARES_ZH_MAP_IDS_REQ: begin");
  auto& maps_to_send = session_.as_zone_server().maps_to_send;
  if (maps_to_send.size() > 0) {
    size_t batch_sz =
      p_->buf_sz() * sizeof(uint32_t) > (session_.send_buf().unfragmented_free_size())
      ? session_.send_buf().unfragmented_free_size()
      : p_->buf_sz();
    batch_sz = batch_sz > maps_to_send.size() ? maps_to_send.size() : batch_sz;

    if (batch_sz > 20) {
      const size_t data_sz = batch_sz * 4;
      SPDLOG_TRACE(log(), "Creating map ids batch of {} bytes", data_sz);
      session_.emplace_and_send<packet::current<packet::ARES_HZ_MAP_IDS>>(data_sz);
      session_.copy_and_send(maps_to_send.data(), data_sz);
      std::copy(maps_to_send.begin(), maps_to_send.begin() + batch_sz,
                std::inserter(session_.as_zone_server().map_ids, session_.as_zone_server().map_ids.begin()));
      maps_to_send.erase(maps_to_send.begin(), maps_to_send.begin() + batch_sz);
    } else {
      log()->error("Can't send maps to zone server: buffer size limits are too low, closing session");
      session_.close_gracefuly();
    }
  } else {
    SPDLOG_TRACE(log(), "No more maps to send to zone server");
    session_.emplace_and_send<packet::current<packet::ARES_HZ_MAP_IDS>>(0);
  }
  SPDLOG_TRACE(log(), "ARES_ZH_MAP_IDS_REQ: end");
}
