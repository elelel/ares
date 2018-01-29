#include "state.hpp"
#include "../session.hpp"

void ares::account::mono::packet_handler<ares::packet::CA_EXE_HASHCHECK>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CA_EXE_HASHCHECK: begin");
  session_state_.client_exe_hash.emplace(std::array<uint8_t, 16>());
  memcpy(session_state_.client_exe_hash->data(), p_->HashValue(), session_state_.client_exe_hash->size());
  SPDLOG_TRACE(log(), "handle_packet CA_EXE_HASHCHECK: end");
}

