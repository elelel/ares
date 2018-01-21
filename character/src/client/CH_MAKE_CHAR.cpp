#include "packet_handlers.hpp"

#include "state.hpp"
#include "../server.hpp"

void ares::character::client::packet_handler<ares::packet::CH_MAKE_CHAR>::operator()() {
  SPDLOG_TRACE(log(), "CH_MAKE_CHAR begin");
  auto ad = server_.db().acc_data_for_aid(p_->aid());
  if (ad) {

  } else {
    log()->error("Could not find account data record for aid {} in SQL database, closing client session", p_->aid());
    server_.remove(s);
  }

  SPDLOG_TRACE(log(), "CH_MAKE_CHAR end");
}
