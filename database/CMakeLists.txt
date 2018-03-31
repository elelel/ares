cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(database LANGUAGES C CXX)
get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

include(${ARES_DIR}/projects/CMake/shared.cmake)

set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(MODEL "${ARES_DIR}/model")
set(CONTRIB "${ARES_DIR}/contrib")

set(SOURCES
  ${SRC}/database.cpp
  ${SRC}/accounts/exists.cpp
  ${SRC}/accounts/create.cpp
  ${SRC}/accounts/create_data.cpp
  ${SRC}/accounts/password_matches.cpp
  ${SRC}/accounts/storage.cpp
  ${SRC}/accounts/slots.cpp
  ${SRC}/accounts/num_chars.cpp
  ${SRC}/accounts/user_data.cpp
  ${SRC}/characters/info.cpp
  ${SRC}/characters/create.cpp
  ${SRC}/characters/delete_date.cpp
  ${SRC}/maps/update.cpp
  ${SRC}/maps/ids_and_names.cpp
  ${SRC}/maps/id_by_name.cpp
  ${SRC}/maps/info.cpp
  )

add_library(database STATIC ${SOURCES})

set_target_properties(database PROPERTIES
  COMPILE_DEFINITIONS "${ARES_COMPILE_DEFINITIONS}"
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_LIBRARIES "${ARES_LINK_LIBRARIES}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  C_STANDARD 11
  CXX_STANDARD 17
  )

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${CONTRIB}/spdlog/include
  ${CONTRIB}/lzma_lite_lib
  ${MODEL}/include
  )

target_include_directories(database PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(database PRIVATE ${ARES_COMPILE_DEFINITIONS})
target_link_libraries(database lzma ${ARES_LINK_LIBRARIES})

message("Database include dirs ${INCLUDE_DIRS}")
