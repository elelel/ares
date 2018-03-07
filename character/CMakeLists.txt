cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(account LANGUAGES C CXX)

get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

if (NOT ARES_PACKET_SET)
  message(FATAL_ERROR "ARES_PACKET_SET is not defined. Set it with -DARES_PACKET_SET=your_version command line argument to CMake")
else()
  message("Compiling for packet version ${ARES_PACKET_SET}")
endif()

include(${ARES_DIR}/projects/CMake/shared.cmake)

find_package(ZLIB REQUIRED)

set(NETWORK "${ARES_DIR}/network")
set(COMMON "${ARES_DIR}/common")
set(MODEL "${ARES_DIR}/model")
set(PACKETS "${ARES_DIR}/packets")
set(DATABASE "${ARES_DIR}/database")
set(CONTRIB "${ARES_DIR}/contrib")
set(DATABASE "${ARES_DIR}/database")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
  ${SRC}/server.cpp
  ${SRC}/auth_request_manager.cpp
  ${SRC}/maps_manager.cpp
  ${SRC}/main.cpp

  ${SRC}/session.cpp
  ${SRC}/config.cpp
  ${SRC}/mono/CH_ENTER.cpp
  ${SRC}/account_server/state.cpp  
  ${SRC}/account_server/ATHENA_AH_LOGIN_RESULT.cpp
  ${SRC}/account_server/ATHENA_AH_PING_ACK.cpp
  ${SRC}/account_server/ARES_AH_AID_AUTH_RESULT.cpp
  ${SRC}/account_server/ATHENA_AH_ACCOUNT_DATA_RESULT.cpp
  ${SRC}/account_server/ATHENA_AH_KICK_AID.cpp
  ${SRC}/client/state.cpp
  ${SRC}/client/PING.cpp
  ${SRC}/client/CH_MAKE_CHAR.cpp
  ${SRC}/client/CH_CHAR_PAGE_REQ.cpp
  ${SRC}/client/CH_SELECT_CHAR.cpp
  ${SRC}/mono/state.cpp

  ${SRC}/mono/ATHENA_ZH_LOGIN_REQ.cpp
  ${SRC}/zone_server/state.cpp
  ${SRC}/zone_server/ATHENA_ZH_ONLINE_USERS.cpp
  ${SRC}/zone_server/ATHENA_ZH_USER_COUNT.cpp
  ${SRC}/zone_server/ATHENA_ZH_GAME_RATES.cpp
  ${SRC}/zone_server/ATHENA_ZH_PING_REQ.cpp
  ${SRC}/zone_server/ARES_ZH_MAP_IDS_REQ.cpp
  ${SRC}/zone_server/ARES_ZH_CID_AUTH_REQ.cpp
  )

if (NOT TARGET common) 
  add_subdirectory(${COMMON} common)
endif()

if (NOT TARGET database) 
  add_subdirectory(${DATABASE} database)
endif()

add_subdirectory(${CONTRIB}/memory memory)
add_subdirectory(${CONTRIB}/lzma lzma)

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${NETWORK}/include
  ${MODEL}/include
  ${CONFIG}/include
  ${COMMON}/include
  ${PACKETS}/include
  ${DATABASE}/include
  ${CONTRIB}/asio/asio/include
  ${CONTRIB}/spdlog/include
  ${CONTRIB}/json/include
  ${CONTRIB}/network_buffer/include
  ${CONTRIB}/RxCpp/Rx/v2/src
  ${CONTRIB}/memory/include
  ${DATABASE}/include
  ${ARES_DIR})

message("Character server compile definitions: ${ARES_COMPILE_DEFINITIONS}")

add_executable(ares_character ${SOURCES})
set_target_properties(ares_character PROPERTIES
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  )

target_link_libraries(ares_character foonathan_memory config common database ${ZLIB_LIBRARIES} stdc++fs lzma ${ARES_LINK_LIBRARIES})
target_include_directories(ares_character PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(ares_character PRIVATE ${ARES_COMPILE_DEFINITIONS})
