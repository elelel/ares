#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::current<ares::packet::CH_CHAR_PAGE_REQ>>::operator()() {
  SPDLOG_TRACE(log(), "CH_CHAR_PAGE_REQ begin");
  auto& c = session_.as_client();
  auto& chars = c.char_select_character_info;
  SPDLOG_TRACE(log(), "Got char page req, unsent chars {}", chars.size());
  ++c.char_page_reqs;
  if (c.char_page_reqs < 20) {
    if (chars.size() > 0) {
      size_t num_to_send = 3;
      if (chars.size() < 3) num_to_send = chars.size();
      session_.emplace_and_send<packet::current<packet::HC_CHAR_PAGES>>(num_to_send);
      std::lock_guard<std::mutex> lock(server_.mutex());
      for (size_t k = 0; k < num_to_send; ++k) {
        const auto& ci = chars[chars.size() - 1];
        long delete_timeout{0};
        auto delete_date = server_.db->query<database::characters::delete_date>(ci.character_id);
        if (delete_date) {
          auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(*delete_date - std::chrono::system_clock::now());
          delete_timeout = diff.count();
          if (delete_timeout < 0) {
            log()->error("Character {} of account id {} should already have been deleted for {} seconds",
                         ci.character_id.to_string(), ci.account_id.to_string(), (delete_timeout / 1000) * -1);
          }
        }

        const auto& last_map_name = server_.maps->name_by_id(ci.location_last.map_id);
        if (last_map_name.size() > 0) {
          SPDLOG_TRACE(log(), "Sending character {} in response to char page req", ci.character_id.to_string());

          session_.emplace_and_send<packet::CHARACTER_INFO>(ci,
                                                            last_map_name,
                                                            delete_timeout,
                                                            (ci.slot < c.playable_slots) ? 1 : 0,
                                                            (ci.rename > 0) && (ci.slot < c.playable_slots) ? 1 : 0
                                                            );

        } else {
          log()->error("Character {} has unknown map id {} in last location, not sending to client", ci.character_id.to_string(), ci.location_last.map_id);
          --k;
        }
        chars.pop_back();
      }
      if ((num_to_send == 3) && (chars.size() == 0)) {
        SPDLOG_TRACE(log(), "Sending characters page terminator 1");
        // Send page terminator if we filled the whole page
        session_.emplace_and_send<packet::current<packet::HC_CHAR_PAGES>>(0);
      }
      if (chars.size() == 0) {
        c.char_select_all_sent = true;
      }
    } else {
      if (!c.char_select_all_sent) {
        SPDLOG_TRACE(log(), "Sending characters page terminator 2");
        // Send page terminator if there were no chars at all
        session_.emplace_and_send<packet::current<packet::HC_CHAR_PAGES>>(0);
        c.char_select_all_sent = true;
      }
    }
  } else {
    log()->error("Session for account id {} requested char page {} times, probably HC_CHAR_PAGES_NUM, or HC_CHAR_PAGES/CHARACTER_INFO format has changed, disconnecting",
                 c.account_id.to_string(), c.char_page_reqs);
    session_.close_gracefuly();
  }
  SPDLOG_TRACE(log(), "CH_CHAR_PAGE_REQ end");
}
