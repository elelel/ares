#include "state.hpp"
#include "../server.hpp"
#include "../session.hpp"

void ares::account::mono::packet_handler<ares::packet::current<ares::packet::CA_EXE_HASHCHECK>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet CA_EXE_HASHCHECK: begin");
  state_.client_exe_hash.emplace(std::array<uint8_t, 16>());
  memcpy(state_.client_exe_hash->data(), p_->HashValue(), state_.client_exe_hash->size());
  SPDLOG_TRACE(log(), "handle_packet CA_EXE_HASHCHECK: end");
}

