#pragma once

namespace ares {
  namespace network {
    namespace handler {
      namespace asio {
        template <typename Handler, typename Session>
        struct base {
          base(const std::shared_ptr<Session>& session);

        protected:
          std::shared_ptr<Session> session_;

          std::shared_ptr<spdlog::logger>& log();
        };
      }
    }
  }
}
