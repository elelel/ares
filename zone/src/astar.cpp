#include "astar.hpp"

thread_local std::multimap<uint32_t, ares::zone::a_star::nodes_vector> ares::zone::a_star::search_state::fringe_ = std::multimap<uint32_t, nodes_vector>();
thread_local ares::zone::a_star::nodes_set ares::zone::a_star::search_state::closed_ = ares::zone::a_star::nodes_set();
thread_local ares::zone::a_star::nodes_vector ares::zone::a_star::search_state::children_ = ares::zone::a_star::nodes_vector();

auto ares::zone::a_star::nodes_vector::pool_instance() -> pool_type& {
  thread_local foonathan::memory::memory_pool<> p(sizeof(space_node), 4096);
  return p;
}

ares::zone::a_star::nodes_vector::nodes_vector() :
  data(pool_instance()) {
}

ares::zone::a_star::nodes_vector::nodes_vector(const std::initializer_list<space_node>& init) :
  data(pool_instance()) {
  for (const auto& i : init) {
    data.push_back(i);
  }
}


auto ares::zone::a_star::nodes_set::pool_instance() -> pool_type& {
  thread_local foonathan::memory::memory_pool<> p(foonathan::memory::set_node_size<space_node>::value, 4096);
  return p;
}

ares::zone::a_star::nodes_set::nodes_set() :
  data(pool_instance()) {
}

ares::zone::a_star::search_state::search_state(const model::map_info& map, const space_node& start, const space_node& goal) :
  map_(map),
  current_(start),
  goal_(goal) {
  closed_.data.clear();
  fringe_.insert(std::move(std::make_pair(0, nodes_vector{start})));
  }

auto ares::zone::a_star::search_state::begin() -> ares::zone::a_star::search_iterator {
  return search_iterator(this);
}

auto ares::zone::a_star::search_state::end() -> ares::zone::a_star::search_iterator {
  return search_iterator(this, true);
}

/*
auto ares::zone::a_star::search_state::result() const -> const std::vector<space_state>& {
  return result_;
  }*/

ares::zone::a_star::search_iterator::search_iterator(search_state* search, bool end) :
  search_(search),
  end_(end) {
}
