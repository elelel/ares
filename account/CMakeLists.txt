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
  ${SRC}/server.cpp
  ${SRC}/config.cpp
  ${SRC}/session.cpp
  ${SRC}/session_timers.cpp
  ${SRC}/recv_handler.cpp
  ${SRC}/send_handler.cpp
  ${SRC}/mono/state.cpp
  ${SRC}/mono/CA_EXE_HASHCHECK.cpp
  ${SRC}/mono/CA_SSO_LOGIN_REQ_0x64.cpp
  ${SRC}/mono/CA_SSO_LOGIN_REQ.cpp
  ${SRC}/mono/ATHENA_HA_LOGIN_REQ.cpp
  ${SRC}/mono/ATHENA_HA_PING_REQ.cpp
  ${SRC}/client/state.cpp
  ${SRC}/char_server/state.cpp
  ${SRC}/char_server/ATHENA_HA_PING_REQ.cpp
  ${SRC}/char_server/ATHENA_HA_ONLINE_AIDS.cpp
  ${SRC}/char_server/ATHENA_HA_USER_COUNT.cpp
  ${SRC}/char_server/ATHENA_HA_AID_AUTH_REQ.cpp
  ${SRC}/char_server/ATHENA_HA_ACCOUNT_DATA_REQ.cpp
  ${SRC}/char_server/ATHENA_HA_SET_AID_ONLINE.cpp
  ${SRC}/char_server/ATHENA_HA_SET_AID_OFFLINE.cpp
  ${SRC}/database/database.cpp
  ${SRC}/database/user_exists.cpp
  ${SRC}/database/create_user.cpp
  ${SRC}/database/password_matches.cpp
  ${SRC}/database/user_data.cpp
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
  ${CONTRIB}/spdlog/include
  ${CONTRIB}/json
  ${CONTRIB}/readerwriterqueue
  ${CONTRIB}/network_buffer/include
  ${ARES_DIR})

message("Account server compile definitions: ${ARES_COMPILE_DEFINITIONS}")

add_executable(ares_account ${SOURCES})
set_target_properties(ares_account PROPERTIES
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  )
target_link_libraries(ares_account config common ${ARES_LINK_LIBRARIES})
target_include_directories(ares_account PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(ares_account PRIVATE ${ARES_COMPILE_DEFINITIONS})
