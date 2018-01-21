#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::CH_MAKE_CHAR>::operator()() {
  SPDLOG_TRACE(log(), "CH_MAKE_CHAR begin");
  const auto& c = session_.as_client();
  auto ad = server_.db().acc_data_for_aid(c.aid);
  if (ad) {
    // TODO: Check if new char creation is allowed in server configuration
    if (p_->CharNum() < ad->creatable_slots) {
      // TODO: Create char in sql
      // TODO: Load data from sql
      // TODO: Send data in HC_ACCEPT_MAKECHAR
    } else {
      // TODO: Send "Can't use this slot"
    }
  } else {
    log()->error("Could not find account data record for aid {} in SQL database, closing client session", c.aid);
    server_.remove(session_.shared_from_this());
  }

  SPDLOG_TRACE(log(), "CH_MAKE_CHAR end");
}
