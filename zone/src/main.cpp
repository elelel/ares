#include <iostream>

#include "server.hpp"

int main() {
  // TODO: command line options: log destination, log level, config filename, foreground/background
  try {
    auto log = spdlog::stdout_color_mt("zone");
    auto io_context = std::make_shared<asio::io_context>();
    log->set_level(spdlog::level::trace);
    ares::zone::config conf(log, io_context, std::optional<std::string>{});
    ares::zone::server s(log, io_context, conf);
    s.start();
    s.run();
    
  } catch (const std::runtime_error e) {
    std::cerr << "main: terminated with runtime error {} " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "main: terminated with unknown exception" << std::endl;
    throw;
  }
  return 0;
}
