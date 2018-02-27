cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(database LANGUAGES C CXX)
get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

include(${ARES_DIR}/projects/CMake/shared.cmake)

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(MODEL "${ARES_DIR}/model")

set(SOURCES
  ${SRC}/database.cpp
  ${SRC}/account/account_create.cpp
  ${SRC}/account/account_storage.cpp
  ${SRC}/account/account_slots.cpp
  ${SRC}/account/num_chars_for_aid.cpp
  ${SRC}/account/create_user.cpp
  ${SRC}/account/password_matches.cpp
  ${SRC}/account/user_data.cpp
  ${SRC}/account/user_exists.cpp
  ${SRC}/character/character_info.cpp
  ${SRC}/character/make_char.cpp
  ${SRC}/character/delete_date.cpp
  ${SRC}/zone/map_index.cpp
  )

add_library(database STATIC ${SOURCES})

set_target_properties(database PROPERTIES
  COMPILE_DEFINITIONS "${ARES_COMPILE_DEFINITIONS}"
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_LIBRARIES "${ARES_LINK_LIBRARIES}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  )

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${CONTRIB}/spdlog/include
  ${MODEL}/include
  )

target_include_directories(database PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(database PRIVATE ${ARES_COMPILE_DEFINITIONS})
