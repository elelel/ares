#include "astar.hpp"

#include "map.hpp"

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

ares::zone::a_star::search_state::search_state(const zone::map& map, const space_node& start, const space_node& goal) :
  map_(map),
  goal_(goal) {
  closed_set().clear();
  fringe().clear();
  auto v = pmr::vector<space_node>(&memory::thread_local_pool());
  v.push_back(start);
  fringe().insert(std::pair<const float, pmr::vector<space_node>>(0.0f, std::move(v)));
  }

auto ares::zone::a_star::search_state::fringe() -> pmr::multimap<float, pmr::vector<space_node>>& {
  static thread_local pmr::multimap<float, pmr::vector<space_node>> f(&memory::thread_local_pool());
  return f;
}

auto ares::zone::a_star::search_state::closed_set() -> pmr::set<space_node>& {
  static thread_local pmr::set<space_node> c(&memory::thread_local_pool());
  return c;
}

auto ares::zone::a_star::search_state::children() -> pmr::vector<space_node>& {
  static thread_local pmr::vector<space_node> v(&memory::thread_local_pool());
  return v;
}

auto ares::zone::a_star::search_state::result() -> pmr::vector<space_node>& {
  static thread_local pmr::vector<space_node> v(&memory::thread_local_pool());
  return v;
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

void ares::zone::a_star::search_state::refresh_children(const space_node& current) {
  children().clear();
  const auto x = current.x;
  const auto y = current.y;
  bool north{false};
  bool south{false};
  bool east{false};
  bool west{false};
  if (map_.is_walkable(x, y + 1)) north = true;
  if (map_.is_walkable(x, y - 1)) south = true;
  if (map_.is_walkable(x + 1, y)) east = true;
  if (map_.is_walkable(x - 1, y)) west = true;

  // Since we are storing the direction in node state, we can use it to prune search tree early from steps backwards towards the root
  if (north && (current.dir != model::packed_coordinates::DIR_SOUTH))
    children().push_back(space_node(x, y + 1, false, model::packed_coordinates::DIR_NORTH, current.depth + 1));
  if (south && (current.dir != model::packed_coordinates::DIR_NORTH))
    children().push_back(space_node(x, y - 1, false, model::packed_coordinates::DIR_SOUTH, current.depth + 1));
  if (east && (current.dir != model::packed_coordinates::DIR_WEST))
    children().push_back(space_node(x + 1, y, false, model::packed_coordinates::DIR_EAST, current.depth + 1));
  if (west && (current.dir != model::packed_coordinates::DIR_EAST))
    children().push_back(space_node(x - 1, y, false, model::packed_coordinates::DIR_WEST, current.depth + 1));

  if ((south && east) &&
      (current.dir != model::packed_coordinates::DIR_NORTHWEST) &&
      map_.is_walkable(x + 1, y - 1))
    children().push_back(space_node(x + 1, y - 1, true, model::packed_coordinates::DIR_SOUTHEAST, current.depth + 1));
        
  if ((north && east) &&
      (current.dir != model::packed_coordinates::DIR_SOUTHWEST) &&
      map_.is_walkable(x + 1, y + 1))
    children().push_back(space_node(x + 1, y + 1, true, model::packed_coordinates::DIR_NORTHEAST, current.depth + 1));
      
  if ((north && west) &&
      (current.dir != model::packed_coordinates::DIR_SOUTHEAST) &&
      map_.is_walkable(x - 1, y + 1))
    children().push_back(space_node(x - 1, y + 1, true, model::packed_coordinates::DIR_NORTHWEST, current.depth + 1));
    
  if ((south && west) &&
      (current.dir != model::packed_coordinates::DIR_NORTHEAST) &&
      map_.is_walkable(x - 1, y - 1))
    children().push_back(space_node(x - 1, y - 1, true, model::packed_coordinates::DIR_SOUTHWEST, current.depth + 1));
}

auto ares::zone::a_star::search_state::path() -> std::vector<model::packed_coordinates> {
  std::vector<model::packed_coordinates> rslt;
  for (size_t i = 0; i < result().size(); ++i) {
    uint8_t dir;
    if (i < result().size() - 1) dir = result()[i + 1].dir;
    else dir = result()[i].dir;
    rslt.push_back({result()[i].x, result()[i].y, dir});
  }
  return rslt;
}

ares::zone::a_star::search_iterator::search_iterator(search_state* search, bool end) :
  search_(search),
  end_(end) {
  }

auto ares::zone::a_star::search_iterator::operator++() -> search_iterator& {
  auto& fringe = search_->fringe();
  auto& closed = search_->closed_set();
  auto& children = search_->children();
  static float move_cost{1.0};
  static float move_diagonal_cost{std::sqrt(float{2.0})};
  if (fringe.size() > 0) {
    auto path = fringe.begin()->second;
    fringe.erase(fringe.begin());
    space_node& current = *path.rbegin();
    if (!((current.x == search_->goal_.x) && (current.y == search_->goal_.y))) {
      if (closed.find(current) == closed.end()) {
        closed.insert(current);
        search_->refresh_children(current);
        for (space_node& child : children) {
          if (child.depth < search_->depth_limit_) {
            child.g = child.diag ? current.g + move_diagonal_cost : current.g + move_cost;
            float h = heuristic(child.x, child.y, search_->goal_.x, search_->goal_.y);
            float f = child.g + h;
            pmr::vector<space_node> new_path(path);
            new_path.push_back(child);
            fringe.insert(std::pair<const float, pmr::vector<space_node>>(f, std::move(new_path)));
          }
        }
      }
    } else {
      search_->result() = path;
      end_ = true;
    }
  } else {
    end_ = true;
  }
  return *this;
}
