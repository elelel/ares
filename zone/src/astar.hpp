#pragma once

#include <cmath>
#include <set>

#include <foonathan/memory/container.hpp>
#include <foonathan/memory/memory_pool.hpp>

#include <ares/model>

namespace ares {
  namespace zone {
    namespace a_star {
      struct space_node {
        constexpr static float move_cost{10.0};
        constexpr static float move_diagonal_cost{14.0};
        
        inline space_node(const uint16_t x,
                          const uint16_t y,
                          const bool diag,
                          const uint8_t dir,
                          const size_t depth,
                          const uint16_t goal_x,
                          const uint16_t goal_y,
                          const uint16_t acc_g) :
          x(x),
          y(y),
          dir(dir),
          depth(depth),
          h(heuristic(x, y, goal_x, goal_y)),
          g(diag ? acc_g + move_diagonal_cost : acc_g + move_cost) {
        }

        const uint16_t x;
        const uint16_t y;
        const uint8_t dir;
        const size_t depth;
        const float h;
        const float g;

      private:
        static inline float heuristic(const uint16_t x, const uint16_t y, const uint16_t g_x, const uint16_t g_y) {
          // Use Euclidian (consistent heuristic), though the client uses Manhattan which breaks A* Graph search
          // optimality guarantees when using closed set
          // TODO: Check the consequences later
          return std::hypot(float(g_x) - float(x), float(g_y) - float(y));
        }
        
      };

      inline bool operator<(const space_node& lhs, const space_node& rhs) {
        return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
      }
      
      using pool_type = foonathan::memory::memory_pool<foonathan::memory::node_pool>;
      
      struct nodes_vector {
        static pool_type& pool_instance();
        nodes_vector();
        nodes_vector(const std::initializer_list<space_node>& init);
        foonathan::memory::vector<space_node, pool_type> data;
      };

      struct nodes_set {
        static pool_type& pool_instance();
        nodes_set();
        foonathan::memory::set<space_node, pool_type> data;
      };
      
      struct search_iterator;
      
      struct search_state {
        friend search_iterator;

        search_state(const model::map_info& map, const space_node& start, const space_node& end);

        search_iterator begin();
        search_iterator end();

        inline void refresh_children() {
          children_.data.clear();
          const auto x = current_.x;
          const auto y = current_.y;
          bool north{false};
          bool south{false};
          bool east{false};
          bool west{false};
          if ((y < map_.y_size()) && (map_.static_flags(x, y + 1).data() & model::map_cell_flags::walkable)) {
            auto dyn_flags = map_.dynamic_flags(x, y + 1);
            if ((!dyn_flags) || (dyn_flags->data() & model::map_cell_flags::walkable)) {
              north = true;
            }
          }
          if ((y > 0) && (map_.static_flags(x, y - 1).data() & model::map_cell_flags::walkable)) {
            auto dyn_flags = map_.dynamic_flags(x, y - 1);
            if ((!dyn_flags) || (dyn_flags->data() & model::map_cell_flags::walkable)) {
              south = true;
            }
          }
          // East
          if ((x < map_.x_size()) && (map_.static_flags(x + 1, y).data() & model::map_cell_flags::walkable)) {
            auto dyn_flags = map_.dynamic_flags(x + 1, y);
            if ((!dyn_flags) || (dyn_flags->data() & model::map_cell_flags::walkable)) {
              east = true;
            }
          }
          // West
          if ((x > 0) && (map_.static_flags(x - 1, y).data() & model::map_cell_flags::walkable)) {
            auto dyn_flags = map_.dynamic_flags(x - 1, y);
            if ((!dyn_flags) || (dyn_flags->data() & model::map_cell_flags::walkable)) {
              west = true;
            }
          }

          if (north) children_.data.push_back(space_node(x, y + 1, false, model::packed_coordinates::DIR_NORTH,
                                                         current_.depth + 1, goal_.x, goal_.y, current_.g));
          if (south) children_.data.push_back(space_node(x, y - 1, false, model::packed_coordinates::DIR_SOUTH,
                                                         current_.depth + 1, goal_.x, goal_.y, current_.g));
          if (east) children_.data.push_back(space_node(x + 1, y, false, model::packed_coordinates::DIR_EAST,
                                                         current_.depth + 1, goal_.x, goal_.y, current_.g));
          if (west) children_.data.push_back(space_node(x - 1, y, false, model::packed_coordinates::DIR_WEST,
                                                         current_.depth + 1, goal_.x, goal_.y, current_.g));

          if (south && east) children_.data.push_back(space_node(x + 1, y - 1, true, model::packed_coordinates::DIR_NORTHEAST,
                                                                 current_.depth + 1, goal_.x, goal_.y, current_.g));
          if (north && east) children_.data.push_back(space_node(x + 1, y + 1, true, model::packed_coordinates::DIR_NORTHEAST,
                                                                 current_.depth + 1, goal_.x, goal_.y, current_.g));
          if (north && west) children_.data.push_back(space_node(x - 1, y + 1, true, model::packed_coordinates::DIR_NORTHWEST,
                                                                 current_.depth + 1, goal_.x, goal_.y, current_.g));
          if (south && west) children_.data.push_back(space_node(x - 1, y - 1, true, model::packed_coordinates::DIR_NORTHWEST,
                                                                 current_.depth + 1, goal_.x, goal_.y, current_.g));
          
        }

        //        const std::vector<space_state>& result() const;
        
      private:
        static thread_local std::multimap<uint32_t, nodes_vector> fringe_;
        static thread_local nodes_set closed_;
        static thread_local nodes_vector children_;
        const model::map_info& map_;
        space_node current_;
        space_node goal_;
        //        std::vector<space_node> result_;
      };

      struct search_iterator : std::iterator<std::forward_iterator_tag, search_state> {
        search_iterator(search_state* search, const bool end = false);

        inline search_iterator& operator++() {
          auto& fringe = search_->fringe_;
          auto& closed = search_->closed_;
          if (fringe.size() > 0) {
            auto path = fringe.begin()->second;
            fringe.erase(fringe.begin());
            auto& node = *path.data.rbegin();
            if ((node.x != search_->goal_.x) && (node.y != search_->goal_.y)) {
              if (closed.data.find(node) == closed.data.end()) {
                closed.data.insert(node);
                //for (const space_state& child : search_.children(
              }
            } else {
              //              std::copy(path.data.begin(), path.data.end(), std::back_inserter(search_->result_));
            }
          } else {
            end_ = true;
          }
          return *this;
        }

      private:
        
        search_state* search_;
        bool end_;
      };
    }
  }
}
