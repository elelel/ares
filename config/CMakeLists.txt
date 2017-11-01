cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(config LANGUAGES C CXX)
get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

include(${ARES_DIR}/projects/CMake/shared.cmake)

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
  src/config.cpp
  )

add_library(config STATIC ${SOURCES})

set_target_properties(config PROPERTIES
  COMPILE_DEFINITIONS "${ARES_COMPILE_DEFINITIONS}"
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_LIBRARIES "${ARES_LINK_LIBRARIES}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  )

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${CONTRIB}/json
  ${CONTRIB}/spdlog/include
  )

target_include_directories(config PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(config PRIVATE ${ARES_COMPILE_DEFINITIONS})
