#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

#include "server.hpp"

int main() {
  // TODO: command line options: log destination, log level, config filename, foreground/background
  auto log = spdlog::stdout_color_mt("zone");
  log->set_level(spdlog::level::trace);
  log->info("Starting");

  auto io_service = std::make_shared<boost::asio::io_service>();
  try {
    ares::zone::config conf(log, io_service, std::optional<std::string>());
    ares::zone::server serv(log, io_service, conf, 2);
    serv.start();
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  } catch (const std::runtime_error e) {
    log->error("main: terminated with runtime error {}", e.what());
  } catch (...) {
    log->error("main: terminated with unknown exception");
    throw;
  }
  log->info("Terminating");
  return 0;
}
