#include "catch.hpp"

#include <ares/model>

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
