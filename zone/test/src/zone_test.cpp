#include "catch.hpp"

#include <ares/model>

#include "astar.hpp"

SCENARIO("Test dense lattice") {
  GIVEN("size_t 0xff00") {
    THEN("Signficant bits should be 16 (max 0xffff)") {
      REQUIRE(ares::model::detail::count_significant_bits(size_t(0xff00)) == 16);
    }
  }
  
  GIVEN("Map 255x255") {
    ares::model::map_info i(255, 255);
    THEN("Size has to be 0xffff + 1") {
      REQUIRE(i.static_flags().size() == 0x10000);
      THEN("Index(123, 200) should be 0xc87b") {
        REQUIRE(i.static_flags().xy_to_index(123, 200) == 0xc87b);
        THEN("Index(0x013b) should be {3, 39}") {
          REQUIRE(i.static_flags().index_to_xy(0xc87b) == (std::tuple<size_t, size_t>(123, 200)));
        }
      }
    }
  }

  GIVEN("Map 7x45 (111b x 101101b)") {
    ares::model::map_info i(7, 45);
    THEN("Size has to be 0x1ff + 1") {
      REQUIRE(i.static_flags().size() == 0x200);
      THEN("Index(3, 39) should be 100111.011 0x013b") {
        REQUIRE(i.static_flags().xy_to_index(3, 39) == 0x013b);
        THEN("Index(0x013b) should be {3, 39}") {
          REQUIRE(i.static_flags().index_to_xy(0x013b) == (std::tuple<size_t, size_t>(3, 39)));
        }
      }
    }
  }

}


SCENARIO("A* search") {
  using namespace ares::model;
  auto walkable = map_cell_flags::from_uint8(map_cell_flags::walkable);
  auto non_walkable = map_cell_flags::from_uint8(0);
  GIVEN("Map 4x3") {
    map_info map(4, 3);
    map.static_flags(0, 0) = walkable;
    map.static_flags(1, 0) = walkable;
    map.static_flags(2, 0) = walkable;
    map.static_flags(3, 0) = walkable;

    map.static_flags(0, 1) = walkable;
    map.static_flags(1, 1) = walkable;
    map.static_flags(2, 1) = non_walkable;
    map.static_flags(3, 1) = non_walkable;
      
    map.static_flags(0, 2) = walkable;
    map.static_flags(1, 2) = walkable;
    map.static_flags(2, 2) = walkable;
    map.static_flags(3, 2) = walkable;
    THEN(R"(Walkability test
O O O S
O O X X
O O O G
)") {
      auto start = ares::zone::a_star::space_node(3, 2, 0, -1, 0);
      auto goal = ares::zone::a_star::space_node(3, 0, 0, -1, 0);
      REQUIRE(start.x == 3);
      REQUIRE(start.y == 2);
      REQUIRE(goal.x == 3);
      REQUIRE(goal.y == 0);

      auto search = ares::zone::a_star::search_state(map, start, goal);
      auto path = search.run();
      std::vector<packed_coordinates> expected
      { packed_coordinates{3, 2, packed_coordinates::DIR_WEST},
          packed_coordinates{2, 2, packed_coordinates::DIR_WEST},
            packed_coordinates{1, 2, packed_coordinates::DIR_SOUTH},
              packed_coordinates{1, 1, packed_coordinates::DIR_SOUTH},
                packed_coordinates{1, 0, packed_coordinates::DIR_EAST},
                  packed_coordinates{2, 0, packed_coordinates::DIR_EAST},
                    packed_coordinates{3, 0, packed_coordinates::DIR_EAST} };

      REQUIRE(path.size() == expected.size());
      REQUIRE(path == expected);
    }

    THEN(R"(Walkability test
S O O O
O O X X
O O O G
)") {
      auto start = ares::zone::a_star::space_node(0, 2, 0, -1, 0);
      auto goal = ares::zone::a_star::space_node(3, 0, 0, -1, 0);
      REQUIRE(start.x == 0);
      REQUIRE(start.y == 2);
      REQUIRE(goal.x == 3);
      REQUIRE(goal.y == 0);

      auto search = ares::zone::a_star::search_state(map, start, goal);
      auto path = search.run();
      std::vector<packed_coordinates> expected
      { packed_coordinates{0, 2, packed_coordinates::DIR_SOUTHEAST},
          packed_coordinates{1, 1, packed_coordinates::DIR_SOUTH},
            packed_coordinates{1, 0, packed_coordinates::DIR_EAST},
              packed_coordinates{2, 0, packed_coordinates::DIR_EAST},
                packed_coordinates{3, 0, packed_coordinates::DIR_EAST} };

      REQUIRE(path.size() == expected.size());
      REQUIRE(path == expected);
    }
  }

  GIVEN("Map 5x4") {
    map_info map(4, 3);
    map.static_flags(0, 0) = walkable;
    map.static_flags(1, 0) = walkable;
    map.static_flags(2, 0) = walkable;
    map.static_flags(3, 0) = walkable;
    map.static_flags(4, 0) = walkable;

    map.static_flags(0, 1) = walkable;
    map.static_flags(1, 1) = non_walkable;
    map.static_flags(2, 1) = non_walkable;
    map.static_flags(3, 1) = non_walkable;
    map.static_flags(4, 1) = walkable;
      
    map.static_flags(0, 2) = walkable;
    map.static_flags(1, 2) = non_walkable;
    map.static_flags(2, 2) = walkable;
    map.static_flags(3, 2) = non_walkable;
    map.static_flags(4, 2) = walkable;

    map.static_flags(0, 3) = walkable;
    map.static_flags(1, 3) = walkable;
    map.static_flags(2, 3) = walkable;
    map.static_flags(3, 3) = walkable;
    map.static_flags(4, 3) = walkable;
    
    THEN(R"(Walkability test
0 0 0 0 0
0 X S X 0
0 X X X 0
0 0 0 G 0
)") {
      auto start = ares::zone::a_star::space_node(2, 2, 0, -1, 0);
      auto goal = ares::zone::a_star::space_node(3, 0, 0, -1, 0);
      auto path = ares::zone::a_star::search_state(map, start, goal).run();
      std::vector<packed_coordinates> expected
      { packed_coordinates{2, 2, packed_coordinates::DIR_NORTH},
          packed_coordinates{2, 3, packed_coordinates::DIR_EAST},
            packed_coordinates{3, 3, packed_coordinates::DIR_EAST},
              packed_coordinates{4, 3, packed_coordinates::DIR_SOUTH},
                packed_coordinates{4, 2, packed_coordinates::DIR_SOUTH},
                  packed_coordinates{4, 1, packed_coordinates::DIR_SOUTH},
                    packed_coordinates{4, 0, packed_coordinates::DIR_WEST},
                      packed_coordinates{3, 0, packed_coordinates::DIR_WEST} };
      REQUIRE(path.size() == expected.size());
      REQUIRE(path == expected);
    }
  }
}




