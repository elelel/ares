#pragma once

#include "../timer.hpp"

template <typename Handler, typename Session>
inline void ares::network::handler::asio::timer<Handler, Session>::operator()(const boost::system::error_code& ec) {
  switch (ec.value()) {
  case 0:
    static_cast<Handler&>(*this).on_success();
    break;
  case boost::asio::error::operation_aborted:
    SPDLOG_TRACE(this->log(), "timer operation aborted");
    break;
  default:
    this->log()->error("timer error {}, {}", ec.value(), ec.message());
    throw std::runtime_error("timer error");
  }
}
