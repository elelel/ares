cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(account LANGUAGES C CXX)

get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

if (NOT ARES_PACKET_SET)
  message(FATAL_ERROR "ARES_PACKET_SET is not defined. Set it with -DARES_PACKET_SET=your_version command line argument to CMake")
else()
  message("Compiling for packet version ${ARES_PACKET_SET}")
endif()

include(${ARES_DIR}/projects/CMake/shared.cmake)

set(NETWORK "${ARES_DIR}/network")
set(COMMON "${ARES_DIR}/common")
set(PACKETS "${ARES_DIR}/packets")
set(DATABASE "${ARES_DIR}/database")
set(CONTRIB "${ARES_DIR}/contrib")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
  ${SRC}/session.cpp
  ${SRC}/mono/state.cpp
  ${SRC}/mono/CZ_ENTER.cpp
  ${SRC}/character_server/ARES_HZ_MAP_NAMES.cpp
  ${SRC}/character_server/ATHENA_HZ_LOGIN_RESULT.cpp
  ${SRC}/character_server/ATHENA_HZ_PRIVATE_MSG_NAME.cpp
  ${SRC}/character_server/ATHENA_HZ_PING_ACK.cpp
  ${SRC}/character_server/state.cpp
  ${SRC}/client/state.cpp
  ${SRC}/config.cpp
  ${SRC}/database/database.cpp
  ${SRC}/database/map_index.cpp

  ${SRC}/main.cpp
  ${SRC}/server.cpp
  )

if (NOT TARGET common) 
  add_subdirectory(${COMMON} common)
endif()

add_subdirectory(${CONTRIB}/memory memory)

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${NETWORK}/include
  ${CONFIG}/include
  ${COMMON}/include
  ${PACKETS}/include
  ${DATABASE}/include
  ${CONTRIB}/asio/asio/include
  ${CONTRIB}/spdlog/include
  ${CONTRIB}/json
  ${CONTRIB}/network_buffer/include
  ${CONTRIB}/RxCpp/Rx/v2/src
  ${CONTRIB}/memory/include
  ${ARES_DIR})

message("Zone server compile definitions: ${ARES_COMPILE_DEFINITIONS}")

add_executable(ares_zone ${SOURCES})
set_target_properties(ares_zone PROPERTIES
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  )
target_link_libraries(ares_zone foonathan_memory config common ${ARES_LINK_LIBRARIES})
target_include_directories(ares_zone PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(ares_zone PRIVATE ${ARES_COMPILE_DEFINITIONS})
