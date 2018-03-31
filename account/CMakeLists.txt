cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ares_account LANGUAGES C CXX)

get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

if (NOT ARES_PACKET_SET)
  message(FATAL_ERROR "ARES_PACKET_SET is not defined. Set it with -DARES_PACKET_SET=your_version command line argument to CMake")
else()
  message("Compiling for packet version ${ARES_PACKET_SET}")
endif()

include(${ARES_DIR}/projects/CMake/shared.cmake)

set(NETWORK "${ARES_DIR}/network")
set(COMMON "${ARES_DIR}/common")
set(MODEL "${ARES_DIR}/model")
set(PACKETS "${ARES_DIR}/packets")
set(DATABASE "${ARES_DIR}/database")
set(CONTRIB "${ARES_DIR}/contrib")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(SOURCES
  ${SRC}/mono/CA_SSO_LOGIN_REQ.cpp
  ${SRC}/mono/CA_EXE_HASHCHECK.cpp
  ${SRC}/mono/ARES_HA_LOGIN_REQ.cpp
  ${SRC}/client/state.cpp

  ${SRC}/main.cpp
  ${SRC}/server.cpp
  ${SRC}/config.cpp
  ${SRC}/session.cpp
  ${SRC}/character_server/state.cpp

  ${SRC}/character_server/ARES_HA_PING_REQ.cpp
  ${SRC}/character_server/ARES_HA_ONLINE_ACCOUNTS.cpp
  ${SRC}/character_server/ARES_HA_USER_COUNT.cpp
  ${SRC}/character_server/ARES_HA_ACCOUNT_AUTH_REQ.cpp
  ${SRC}/mono/state.cpp
  )

if (NOT TARGET common) 
  add_subdirectory(${COMMON} common)
endif()

if (NOT TARGET database) 
  add_subdirectory(${DATABASE} database)
endif()

if (NOT TARGET lzma)
  add_subdirectory(${CONTRIB}/lzma_lite_lib lzma)
endif()

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${NETWORK}/include
  ${CONFIG}/include
  ${COMMON}/include
  ${MODEL}/include
  ${PACKETS}/include
  ${DATABASE}/include
  ${CONTRIB}/asio/asio/include
  ${CONTRIB}/spdlog/include
  ${CONTRIB}/json/include
  ${CONTRIB}/network_buffer/include
  ${CONTRIB}/RxCpp/Rx/v2/src
  ${CONTRIB}/memory/include
  ${ARES_DIR})

add_executable(ares_account ${SOURCES})
set_target_properties(ares_account PROPERTIES
  COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
  LINK_FLAGS "${ARES_LINK_FLAGS}"
  C_STANDARD 11
  CXX_STANDARD 17
  )
  
target_link_libraries(ares_account ${ARES_LINK_LIBRARIES} common database lzma )
target_include_directories(ares_account PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(ares_account PRIVATE ${ARES_COMPILE_DEFINITIONS})
