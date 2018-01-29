#include <iostream>

#include <boost/asio.hpp>

#include "state.hpp"

int main() {
  // TODO: command line options: log destination, log level, config filename, foreground/background
  auto io_service = std::make_shared<boost::asio::io_service>();
  try {
    ares::character::state s;
    s.server.start();
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  } catch (const std::runtime_error e) {
    std::cerr << "main: terminated with runtime error " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "main: terminated with unknown exception" << std::endl;
    throw;
  }
  return 0;
}
