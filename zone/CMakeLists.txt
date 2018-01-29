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
  ${SRC}/session_timers.cpp
  ${SRC}/recv_handler.cpp
  ${SRC}/send_handler.cpp
  ${SRC}/database/database.cpp
  ${SRC}/mono/state.cpp
  ${SRC}/character_server/state.cpp
  ${SRC}/character_server/timers.cpp
  ${SRC}/character_server/ATHENA_HZ_LOGIN_RESULT.cpp
  ${SRC}/character_server/ATHENA_HZ_PRIVATE_MSG_NAME.cpp
  ${SRC}/character_server/ATHENA_HZ_PING_ACK.cpp
  ${SRC}/client/state.cpp
  ${SRC}/client/CZ_REQUEST_MOVE.cpp
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
  ${CONTRIB}/RxCpp/Rx/v2/src
  ${ARES_DIR})

message("Zone server compile definitions: ${ARES_COMPILE_DEFINITIONS}")

add_executable(ares_zone ${SOURCES})
set_target_properties(ares_zone PROPERTIES
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  )
target_link_libraries(ares_zone config common ${ARES_LINK_LIBRARIES})
target_include_directories(ares_zone PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(ares_zone PRIVATE ${ARES_COMPILE_DEFINITIONS})
