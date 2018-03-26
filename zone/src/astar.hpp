#pragma once

#include <cmath>
#include <set>

#include <foonathan/memory/container.hpp>
#include <foonathan/memory/memory_pool.hpp>

#include <ares/model>

namespace ares {
  namespace zone {
    namespace a_star {
      /* Node in search space tree
       */
      struct space_node {
       
        inline space_node(const uint16_t x,
                          const uint16_t y,
                          const bool diag,
                          const uint8_t dir,
                          const size_t depth) :
          x(x),
          y(y),
          diag(diag),
          dir(dir),
          depth(depth) {
        }

        space_node(const space_node& other);
        space_node& operator=(const space_node& other);

        uint16_t x;
        uint16_t y;
        /*!
          Cached value if the move to these coordinates was diagonal
         */
        bool diag;
        /*!
          Direction used to reach these coordinates from previos cell
        */
        uint8_t dir;
        /*!
          Search tree depth used to reach this cell
        */
        size_t depth;
        /*!
          Value of goal function at these coordinates
        */
        float g;
      };

      /*! Return value of heuristic
        \param x x coordinate of cell in subject
        \param y y coordinate of cell in subject
        \param g_x x coordinate of goal cell
        \param g_y y coordinate of goal cell
       */
      inline float heuristic(const uint16_t x, const uint16_t y, const uint16_t g_x, const uint16_t g_y) {
        // Use Euclidian (consistent heuristic), though the client uses Manhattan which breaks A* Graph search
        // optimality guarantees when using closed set
        // TODO: Check the consequences later
        return float(std::hypot(float(g_x) - float(x), float(g_y) - float(y)));
      }

      /*!
        Search tree node comparison for ordered containers
       */
      inline bool operator<(const space_node& lhs, const space_node& rhs) {
        return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
      }
      
      using pool_type = foonathan::memory::memory_pool<foonathan::memory::node_pool>;

      /*!
        Vector of search space nodes, implemented as an STL vector with thread local pool memory allocator
       */
      struct nodes_vector {
        static pool_type& pool_instance();
        nodes_vector();
        nodes_vector(const std::initializer_list<space_node>& init);
        foonathan::memory::vector<space_node, pool_type> data;
      };

      /*!
        Set of search space nodes, implemented as an STL set with thread local pool memory allocator
       */
      struct nodes_set {
        static pool_type& pool_instance();
        nodes_set();
        foonathan::memory::set<space_node, pool_type> data;
      };
      
      struct search_iterator;

      /*!
        Structure to hold search state
       */
      struct search_state {
        friend search_iterator;

        search_state(const model::map_info& map, const space_node& start, const space_node& goal);

        search_iterator begin();
        search_iterator end();

        std::vector<model::packed_coordinates> run();

        /*!
          Refreshes the children of current node in search tree. 
         */
        inline void refresh_children(const space_node& current) {
          children_.data.clear();
          const auto x = current.x;
          const auto y = current.y;
          bool north{false};
          bool south{false};
          bool east{false};
          bool west{false};
          if ((y < map_.y_size()) && (map_.static_flags(x, y + 1).data() & model::map_cell_flags::walkable)) {
            north = true;
          }
          if ((y > 0) && (map_.static_flags(x, y - 1).data() & model::map_cell_flags::walkable)) {
            south = true;
          }
          // East
          if ((x < map_.x_size()) && (map_.static_flags(x + 1, y).data() & model::map_cell_flags::walkable)) {
            east = true;
          }
          // West
          if ((x > 0) && (map_.static_flags(x - 1, y).data() & model::map_cell_flags::walkable)) {
            west = true;
          }

          // Since we are storing the direction in node state, we can use it to prune search tree early from steps backwards towards the root
          if (north && (current.dir != model::packed_coordinates::DIR_SOUTH))
            children_.data.push_back(space_node(x, y + 1, false, model::packed_coordinates::DIR_NORTH, current.depth + 1));
          if (south && (current.dir != model::packed_coordinates::DIR_NORTH))
            children_.data.push_back(space_node(x, y - 1, false, model::packed_coordinates::DIR_SOUTH, current.depth + 1));
          if (east && (current.dir != model::packed_coordinates::DIR_WEST))
            children_.data.push_back(space_node(x + 1, y, false, model::packed_coordinates::DIR_EAST, current.depth + 1));
          if (west && (current.dir != model::packed_coordinates::DIR_EAST))
            children_.data.push_back(space_node(x - 1, y, false, model::packed_coordinates::DIR_WEST, current.depth + 1));

          if ((south && east) &&
              (current.dir != model::packed_coordinates::DIR_NORTHWEST) &&
              (map_.static_flags(x + 1, y - 1).data() & model::map_cell_flags::walkable))
            children_.data.push_back(space_node(x + 1, y - 1, true, model::packed_coordinates::DIR_SOUTHEAST, current.depth + 1));
        
          if ((north && east) &&
              (current.dir != model::packed_coordinates::DIR_SOUTHWEST) &&
              (map_.static_flags(x + 1, y + 1).data() & model::map_cell_flags::walkable))
            children_.data.push_back(space_node(x + 1, y + 1, true, model::packed_coordinates::DIR_NORTHEAST, current.depth + 1));
      
          if ((north && west) &&
              (current.dir != model::packed_coordinates::DIR_SOUTHEAST) &&
              (map_.static_flags(x - 1, y + 1).data() & model::map_cell_flags::walkable))
            children_.data.push_back(space_node(x - 1, y + 1, true, model::packed_coordinates::DIR_NORTHWEST, current.depth + 1));
    
          if ((south && west) &&
              (current.dir != model::packed_coordinates::DIR_NORTHEAST) &&
              (map_.static_flags(x - 1, y - 1).data() & model::map_cell_flags::walkable))
            children_.data.push_back(space_node(x - 1, y - 1, true, model::packed_coordinates::DIR_SOUTHWEST, current.depth + 1));
        }

        const nodes_vector& result() const;
        
      private:
        /*!
          Construct a vector of coordinates/directions from search result
         */
        std::vector<model::packed_coordinates> path() const;
        
        static thread_local std::multimap<float, nodes_vector> fringe_;
        static thread_local nodes_set closed_;
        static thread_local nodes_vector children_;
        const model::map_info& map_;
        space_node goal_;
        size_t depth_limit_{128};
        nodes_vector result_;
      };

      /*!
        Iterator to perform single step in A* search
       */
      struct search_iterator : std::iterator<std::forward_iterator_tag, search_state> {
        search_iterator(search_state* search, const bool end = false);

        inline search_iterator& operator++() {
          auto& fringe = search_->fringe_;
          auto& closed = search_->closed_;
          static float move_cost{1.0};
          static auto move_diagonal_cost = float(sqrt(2.0));
          if (fringe.size() > 0) {
            auto path = fringe.begin()->second;
            fringe.erase(fringe.begin());
            space_node& current = *path.data.rbegin();
            if (!((current.x == search_->goal_.x) && (current.y == search_->goal_.y))) {
              if (closed.data.find(current) == closed.data.end()) {
                closed.data.insert(current);
                search_->refresh_children(current);
                for (space_node& child : search_->children_.data) {
                  if (child.depth < search_->depth_limit_) {
                    child.g = child.diag ? current.g + move_diagonal_cost : current.g + move_cost;
                    float h = heuristic(child.x, child.y, search_->goal_.x, search_->goal_.y);
                    float f = child.g + h;
                    nodes_vector new_path(path);
                    new_path.data.push_back(child);
                    fringe.insert({f, std::move(new_path)});
                  }
                }
              }
            } else {
              search_->result_ = path;
              end_ = true;
            }
          } else {
            end_ = true;
          }
          return *this;
        }

        /*!
          A* iterator comparison is used only to test if we are at the end of the search
         */
        inline bool operator==(const search_iterator& other) const {
          return end_ == other.end_;
        }

        inline bool operator!=(const search_iterator& other) const {
          return !(*this == other);
        }

      private:
        
        search_state* search_;
        bool end_;
      };
    }
  }
}
