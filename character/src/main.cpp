#include <iostream>

#include "server.hpp"

int main() {
  // TODO: command line options: log destination, log level, config filename, foreground/background
  try {
    auto log = spdlog::stdout_color_mt("character");
    log->set_level(spdlog::level::trace);
    auto io_context = std::make_shared<asio::io_context>();

    auto conf = std::make_shared<ares::character::config>(log, io_context, std::optional<std::string>{});
    auto s = std::make_shared<ares::character::server>(log, io_context, *conf);
    s->start();
    s->run();
    
  } catch (const std::runtime_error e) {
    std::cerr << "main: terminated with runtime error {} " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "main: terminated with unknown exception" << std::endl;
    throw;
  }
  return 0;
}

