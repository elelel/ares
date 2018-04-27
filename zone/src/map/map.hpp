#pragma once

#include <map>
#include <memory>
#include <mutex>

#include <spdlog/spdlog.h>

#include <asio/ts/io_context.hpp>
#include <asio/ts/executor.hpp>
#include <asio/ts/socket.hpp>
#include <asio/ts/net.hpp>
#include <concurrentqueue.h>

#include <ares/model>

#include "events.hpp"

namespace ares {
  namespace zone {

    struct pc;

    struct map : std::enable_shared_from_this<zone::map> {
      map(std::shared_ptr<spdlog::logger> log,
          std::shared_ptr<asio::io_context> io_context,
          const uint32_t id, const model::map_info& info);

      bool is_walkable(const uint16_t x, const uint16_t y) const;

      std::mutex& mutex();

      /*! Fold events that are scheduled to run on map tick
       */
      void on_tick();

      /*! Associates pc with current map
       */
      bool add_pc(std::shared_ptr<zone::pc> pc);

      /*! Remove pc assiociation by character id
       */
      void remove_pc(const model::character_id cid);

      /*! Remove pc assiociation by pointer
       */
      void remove_pc(std::shared_ptr<zone::pc> pc);

      void defuse_asio();
      std::shared_ptr<spdlog::logger> log() const;

      // Lock-free queue for tick timer events
      moodycamel::ConcurrentQueue<map_event> event_queue;

      static constexpr std::chrono::milliseconds tick_granularity{30};
      std::shared_ptr<asio::steady_timer> tick_timer;
    private:
      std::map<model::character_id, std::weak_ptr<zone::pc>> pcs_;
      //      std::map<uint32_t, std::vector<std::weak_ptr<...>>> mobs_;
      uint32_t id_;
      model::map_info info_;

      std::mutex mutex_;
      std::shared_ptr<asio::io_context> io_context_;
      std::shared_ptr<spdlog::logger> log_;

      void start_tick_timer();
      void stop_tick_timer();
    };

    /* Each map has a tick timer for the events that are expected to be not CPU-intensive and frequent.
       Non frequent events or long executing events should be executed on respective ASIO handlers.
     */
    struct map_tick_handler {
      map_tick_handler(std::weak_ptr<zone::map> map_);
      map_tick_handler(const map_tick_handler& other);

      void operator()(const std::error_code& ec);

    private:
      std::weak_ptr<zone::map> map_;
    };
    

  }
}
