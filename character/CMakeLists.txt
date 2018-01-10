cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(account LANGUAGES C CXX)

get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

if (NOT ARES_PACKET_VER)
  message(FATAL_ERROR "ARES_PACKET_VER is not defined. Set it with -DARES_PACKET_VER=your_version command line argument to CMake")
else()
  message("Compiling for packet version ${ARES_PACKET_VER}")
endif()

include(${ARES_DIR}/projects/CMake/shared.cmake)

set(NETWORK "${ARES_DIR}/network")
set(CONFIG "${ARES_DIR}/config")
set(COMMON "${ARES_DIR}/common")
set(PACKETS "${ARES_DIR}/packets")
set(DATABASE "${ARES_DIR}/database")
set(CONTRIB "${ARES_DIR}/contrib")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
  ${SRC}/main.cpp
  ${SRC}/server.cpp
  ${SRC}/config.cpp
  ${SRC}/session.cpp
  ${SRC}/session_timers.cpp
  ${SRC}/recv_handler.cpp
  ${SRC}/send_handler.cpp
  ${SRC}/account_server/state.cpp
  ${SRC}/account_server/timers.cpp
  ${SRC}/account_server/ATHENA_AH_LOGIN_RESULT.cpp
  ${SRC}/account_server/ATHENA_AH_PING_ACK.cpp
  ${SRC}/account_server/ATHENA_AH_AID_AUTH_RESULT.cpp
  ${SRC}/mono/state.cpp
  ${SRC}/mono/CH_ENTER.cpp
  ${SRC}/mono/ATHENA_ZH_LOGIN_REQ.cpp
  ${SRC}/client/state.cpp
  ${SRC}/zone_server/state.cpp
  ${SRC}/zone_server/ATHENA_ZH_MAP_NAMES.cpp
  ${SRC}/zone_server/ATHENA_ZH_ONLINE_USERS.cpp
  ${SRC}/zone_server/ATHENA_ZH_USER_COUNT.cpp
  ${SRC}/zone_server/ATHENA_ZH_GAME_RATES.cpp
  ${SRC}/zone_server/ATHENA_ZH_PING_REQ.cpp
  ${SRC}/database/database.cpp
  )

if (NOT TARGET config) 
  add_subdirectory(${CONFIG} config)
endif()
if (NOT TARGET common) 
  add_subdirectory(${COMMON} common)
endif()

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${NETWORK}/include
  ${CONFIG}/include
  ${COMMON}/include
  ${PACKETS}/include
  ${DATABASE}/include
  ${CONTRIB}/spdlog/include
  ${CONTRIB}/json
  ${CONTRIB}/network_buffer/include
  ${ARES_DIR})

message("Character server compile definitions: ${ARES_COMPILE_DEFINITIONS}")

add_executable(ares_character ${SOURCES})
set_target_properties(ares_character PROPERTIES
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  )
target_link_libraries(ares_character config common ${ARES_LINK_LIBRARIES})
target_include_directories(ares_character PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(ares_character PRIVATE ${ARES_COMPILE_DEFINITIONS})
