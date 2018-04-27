#pragma once

#include <cmath>
#include <set>

#include <ares/model>
#include <ares/common/memory>

namespace ares {
  namespace zone {
    struct map;
    
    namespace a_star {
      /* Node in search space tree
       */
      struct space_node {
        inline space_node(const uint16_t x,
                          const uint16_t y) :
          space_node(x, y, 0, -1, 0) {
        }
        
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
      
      struct search_iterator;

      /*!
        Structure to hold search state
       */
      struct search_state {
        friend search_iterator;

        search_state(const zone::map& map, const space_node& start, const space_node& goal);

        search_iterator begin();
        search_iterator end();

        std::vector<model::packed_coordinates> run();

        /*!
          Refreshes the children of current node in search tree. 
         */
        void refresh_children(const space_node& current);
        
      private:
        /*!
          Construct a vector of coordinates/directions from search result
         */
        std::vector<model::packed_coordinates> path();


        pmr::multimap<float, pmr::vector<space_node>>& fringe();
        pmr::set<space_node>& closed_set();
        pmr::vector<space_node>& children();
        pmr::vector<space_node>& result();
        
        const zone::map& map_;
        space_node goal_;
        size_t depth_limit_{128};
      };

      /*!
        Iterator to perform single step in A* search
       */
      struct search_iterator : std::iterator<std::forward_iterator_tag, search_state> {
        search_iterator(search_state* search, const bool end = false);

        /*!
          Make single step in search space
        */
        search_iterator& operator++();

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
