#include "event_visitor.hpp"

ares::zone::map_event_visitor::map_event_visitor(zone::map& map,
                                                 const std::chrono::steady_clock::time_point& tick_time,
                                                 pmr::map<std::chrono::steady_clock::time_point, map_event>& sorted_events) :
  map_(map),
  tick_time_(tick_time),
  sorted_events_(sorted_events) {
}

