cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(common LANGUAGES C CXX)
get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

include(${ARES_DIR}/projects/CMake/shared.cmake)

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
  src/string_conv.cpp
  src/random.cpp
  src/config.cpp
  src/grf.cpp
  src/memory.cpp
  )

add_library(common STATIC ${SOURCES})

set_target_properties(common PROPERTIES
  COMPILE_DEFINITIONS "${ARES_COMPILE_DEFINITIONS}"
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_LIBRARIES "${ARES_LINK_LIBRARIES}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  C_STANDARD 11
  CXX_STANDARD 17
  )

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${CONTRIB}/json/include
  ${CONTRIB}/asio/asio/include
  ${CONTRIB}/spdlog/include
  ${CONTRIB}/lzma_lite_lib
  )

target_link_libraries(common ${ARES_LINK_LIBRARIES})
target_include_directories(common PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(common PRIVATE ${ARES_COMPILE_DEFINITIONS})
