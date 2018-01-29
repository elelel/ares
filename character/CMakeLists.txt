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
set(COMMON "${ARES_DIR}/common")
set(PACKETS "${ARES_DIR}/packets")
set(DATABASE "${ARES_DIR}/database")
set(CONTRIB "${ARES_DIR}/contrib")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
  ${SRC}/main.cpp
  ${SRC}/state.cpp
  ${SRC}/server.cpp
  ${SRC}/config.cpp
  ${SRC}/session.cpp
  ${SRC}/recv_handler.cpp
  ${SRC}/send_handler.cpp
  ${SRC}/account_server/state.cpp
  ${SRC}/account_server/timers.cpp
  ${SRC}/account_server/ATHENA_AH_LOGIN_RESULT.cpp
  ${SRC}/account_server/ATHENA_AH_PING_ACK.cpp
  ${SRC}/account_server/ATHENA_AH_AID_AUTH_RESULT.cpp
  ${SRC}/account_server/ATHENA_AH_ACCOUNT_DATA_RESULT.cpp
  ${SRC}/client/state.cpp
  ${SRC}/client/PING.cpp
  ${SRC}/client/CH_MAKE_CHAR.cpp
  ${SRC}/client/CH_CHAR_PAGE_REQ.cpp
  ${SRC}/client/CH_SELECT_CHAR.cpp
  ${SRC}/account_server/ATHENA_AH_KICK_AID.cpp
  ${SRC}/mono/state.cpp
  ${SRC}/mono/CH_ENTER.cpp
  ${SRC}/mono/ATHENA_ZH_LOGIN_REQ.cpp
  ${SRC}/zone_server/state.cpp
  ${SRC}/zone_server/ATHENA_ZH_MAP_NAMES.cpp
  ${SRC}/zone_server/ATHENA_ZH_ONLINE_USERS.cpp
  ${SRC}/zone_server/ATHENA_ZH_USER_COUNT.cpp
  ${SRC}/zone_server/ATHENA_ZH_GAME_RATES.cpp
  ${SRC}/zone_server/ATHENA_ZH_PING_REQ.cpp
  ${SRC}/database/database.cpp
  ${SRC}/database/account_create.cpp
  ${SRC}/database/account_storage.cpp
  ${SRC}/database/account_slots.cpp
  ${SRC}/database/num_chars_for_aid.cpp
  ${SRC}/database/character_info.cpp
  ${SRC}/database/make_char.cpp
  )

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
  ${CONTRIB}/asio/include
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
