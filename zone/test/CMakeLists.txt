cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ares_zone_test LANGUAGES C CXX)

get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ARES_DIR "${ARES_DIR}" PATH)

include(${ARES_DIR}/projects/CMake/shared.cmake)

include(CTest)

enable_testing()


if (BUILD_TESTING)
  set(CONTRIB "${ARES_DIR}/contrib")
  set(COMMON "${ARES_DIR}/common")
  set(CATCH "${ARES_DIR}/contrib/Catch")
  set(MODEL "${ARES_DIR}/model")
  set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
  set(ZONE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../src")
  
  set(INCLUDE_DIRS
    ${ARES_INCLUDE_DIRECTORIES}
    ${CATCH}/include
    ${MODEL}/include
    ${MODEL}/include
    ${COMMON}/include
    ${CONTRIB}/memory/include
    ${ZONE_SRC}
    )

  set(TEST_SOURCES
    ${SRC}/zone_test.cpp
    ${ZONE_SRC}/astar.cpp
    )
  
  add_subdirectory(${CONTRIB}/memory memory)
  add_subdirectory(${COMMON} common)
  
  add_executable(zone_test ${TEST_SOURCES})
  set_target_properties(zone_test PROPERTIES
    COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
    LINK_FLAGS "${ARES_LINK_FLAGS}"
    C_STANDARD 11
    CXX_STANDARD 17
    )
  target_link_libraries(zone_test common ${ARES_LINK_LIBRARIES})
  target_include_directories(zone_test PRIVATE ${INCLUDE_DIRS})
  target_compile_definitions(zone_test PRIVATE ${ARES_COMPILE_DEFINITIONS} "CATCH_CONFIG_MAIN" "CATCH_CONFIG_COLOUR_NONE")

  
  add_test(NAME RunZoneTest WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" COMMAND zone_test)
endif()
