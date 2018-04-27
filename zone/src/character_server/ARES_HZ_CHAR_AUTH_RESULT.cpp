#include "state.hpp"
#include "../server.hpp"

#include "../pc/pc.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_CHAR_AUTH_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_CHAR_AUTH_RESULT: begin");
  std::shared_ptr<session> s;
  {
    std::lock_guard<std::mutex> lock(server_.auth_requests->mutex());
    s = server_.auth_requests->responded(p_->request_id());
  }
  if (s) {
    if (p_->result() == 0) {
      if (s->is_mono()) {
        server_.log()->info("Character id {} was authenticated by account server", p_->character_id().to_string());
        auto new_state = s->as_mono();
        s->session_state_.emplace<client::state>(std::move(new_state));

        auto pc_info = server_.db->query<database::characters::info>(p_->character_id());
        if (pc_info) {
          {
            std::lock_guard<std::mutex> maps_mutex(server_.maps.mutex());
            auto map_id = pc_info->location_last.map_id;
            auto map = server_.maps.by_id(map_id);
            if (map) {
              auto pc = std::make_shared<zone::pc>(*pc_info, map, session_.shared_from_this());
              s->as_client().pc = pc;
              server_.pcs.add(p_->character_id(), pc, map, s);
              s->emplace_and_send<packet::current<packet::ZC_ACCEPT_ENTER>>(std::chrono::system_clock::time_point(),
                                                                            *pc_info->location_last.coords.x(),
                                                                            *pc_info->location_last.coords.y(),
                                                                            0, // TODO: direction
                                                                            5,
                                                                            5,
                                                                            1 // TODO: font
                                                                            );
            } else {
              log()->error("Failed to log on char {}, last location map with id {} doesn't  exist in the server, closing session", p_->character_id().to_string(), pc_info->location_last.map_id);
              s->emplace_and_send<packet::current<packet::ZC_REFUSE_ENTER>>(0);
              s->close_gracefuly();
            }
          }

          std::lock_guard<std::mutex> server_lock(server_.mutex());
          server_.add(s);
        } else {
          log()->error("Failed to load information for character {}, closing session", p_->character_id().to_string());
          s->emplace_and_send<packet::current<packet::ZC_REFUSE_ENTER>>(0);
          s->close_gracefuly();
        }
      } else {
        log()->error("Received authentication result from character server for character_id {}, but it's session is not in mono state, ignoring.", p_->character_id().to_string());
      }
    } else {
      log()->warn("Character id {} authentication denied by account server with code {}, closing session", p_->character_id().to_string(), p_->result());
      s->emplace_and_send<packet::current<packet::ZC_REFUSE_ENTER>>(0);
      s->close_gracefuly();
    }
  } else {
    log()->error("Received authentication result from character server for character_id {}, but auth request manager was not expecting it, ignoring.", p_->character_id().to_string());
  }
  SPDLOG_TRACE(log(), "handle_packet ARES_HZ_CHAR_AUTH_RESULT: end");
}
