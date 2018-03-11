#include "state.hpp"
#include "../server.hpp"

#include "../pc.hpp"

void ares::zone::character_server::packet_handler<ares::packet::current<ares::packet::ARES_HZ_CID_AUTH_RESULT>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_CID_AUTH_RESULT: begin");
  std::shared_ptr<session> s;
  {
    std::lock_guard<std::mutex> lock(server_.auth_requests->mutex());
    s = server_.auth_requests->responded(p_->request_id());
  }
  if (s) {
    if (p_->result() == 0) {
      if (s->is_mono()) {
        server_.log()->info("Cid {} was authenticated by account server", p_->cid());
        auto new_state = s->as_mono();
        s->session_state_.emplace<client::state>(std::move(new_state));

        auto pc_info = server_.db->query<database::characters::info>(p_->cid());
        if (pc_info) {
          {
            std::lock_guard<std::mutex> maps_mutex(server_.maps.mutex());
            auto map_id = pc_info->location_last.map_id;
            auto map = server_.maps.by_id(map_id);
            if (map) {
              auto pc = std::make_shared<zone::pc>(*pc_info, map);
              s->as_client().pc = pc;
              server_.pcs.add(p_->cid(), pc, s);
              s->emplace_and_send<packet::current<packet::ZC_ACCEPT_ENTER>>(std::chrono::system_clock::time_point(),
                                                                            pc_info->location_last.coords.x,
                                                                            pc_info->location_last.coords.y,
                                                                            0, // TODO: direction
                                                                            5,
                                                                            5,
                                                                            1 // TODO: font
                                                                            );
            } else {
              log()->error("Failed to log on char {}, last location map with id {} doesn't  exist in the server, closing session", p_->cid(), pc_info->location_last.map_id);
              s->emplace_and_send<packet::current<packet::ZC_REFUSE_ENTER>>(0);
              s->close_gracefuly();
            }
          }

          std::lock_guard<std::mutex> server_lock(server_.mutex());
          server_.add(s);
        } else {
          log()->error("Failed to load information for character {}, closing session", p_->cid());
          s->emplace_and_send<packet::current<packet::ZC_REFUSE_ENTER>>(0);
          s->close_gracefuly();
        }
      } else {
        log()->error("Received authentication result from character server for cid {}, but it's session is not in mono state, ignoring.", p_->cid());
      }
    } else {
      log()->warn("Cid {} authentication denied by account server, closing session", p_->cid());
      s->emplace_and_send<packet::current<packet::ZC_REFUSE_ENTER>>(0);
      s->close_gracefuly();
    }
  } else {
    log()->error("Received authentication result from character server for cid {}, but auth request manager was not expecting it, ignoring.", p_->cid());
  }
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HZ_CID_AUTH_RESULT: end");
}
