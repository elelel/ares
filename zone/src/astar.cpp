#include "astar.hpp"

thread_local std::multimap<float, ares::zone::a_star::nodes_vector> ares::zone::a_star::search_state::fringe_ = std::multimap<float, nodes_vector>();
thread_local ares::zone::a_star::nodes_set ares::zone::a_star::search_state::closed_ = ares::zone::a_star::nodes_set();
thread_local ares::zone::a_star::nodes_vector ares::zone::a_star::search_state::children_ = ares::zone::a_star::nodes_vector();

ares::zone::a_star::space_node::space_node(const space_node& other) :
  x(other.x),
  y(other.y),
  diag(other.diag),
  dir(other.dir),
  depth(other.depth) {
}

auto ares::zone::a_star::space_node::operator=(const space_node& other) -> space_node& {
  x = other.x;
  y = other.y;
  diag = other.diag;
  dir = other.dir;
  depth = other.depth;
  return *this;
}

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
  goal_(goal) {
  closed_.data.clear();
  fringe_.clear();
  fringe_.insert(std::move(std::make_pair(0.0, nodes_vector{start})));
  }

auto ares::zone::a_star::search_state::begin() -> ares::zone::a_star::search_iterator {
  return search_iterator(this);
}

auto ares::zone::a_star::search_state::end() -> ares::zone::a_star::search_iterator {
  return search_iterator(this, true);
}

auto ares::zone::a_star::search_state::run() -> std::vector<model::packed_coordinates> {
  for (auto it = begin(); it != end(); ++it);
  return path();
}

auto ares::zone::a_star::search_state::path() const -> std::vector<model::packed_coordinates> {
  std::vector<model::packed_coordinates> rslt;
  for (size_t i = 0; i < result_.data.size(); ++i) {
    uint8_t dir;
    if (i < result_.data.size() - 1) dir = result_.data[i + 1].dir;
    else dir = result_.data[i].dir;
    rslt.push_back({result_.data[i].x, result_.data[i].y, dir});
  }
  return rslt;
}

auto ares::zone::a_star::search_state::result() const -> const nodes_vector& {
  return result_;
}

ares::zone::a_star::search_iterator::search_iterator(search_state* search, bool end) :
  search_(search),
  end_(end) {
}
