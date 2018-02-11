// TODO: Unused now, port them into new system 

void ares::character::account_server::timer::send_aids::on_timer() {
  std::lock_guard<std::mutex> lock(session_->server_state_.server.mutex());
  SPDLOG_TRACE(log(), "sending {} online aids", session_->server_state_.server.clients().size());
  session_->emplace_and_send<packet_type<packet::ATHENA_HA_ONLINE_AIDS>>(session_->server_state_.server.clients().size());
  SPDLOG_TRACE(log(), "online aids header sent" );
  for (const auto& p : session_->server_state_.server.clients()) {
    session_->copy_and_send(&p.first, sizeof(p.first));
  }
  session_->as_account_server().send_aids_timer.set();  
}

void ares::character::account_server::timer::send_user_count::on_timer() {
  std::lock_guard<std::mutex> lock(session_->server_state_.server.mutex());
  session_->emplace_and_send<packet_type<packet::ATHENA_HA_USER_COUNT>>(session_->server_state_.server.clients().size());
  session_->as_account_server().send_user_count_timer.set();  
}
