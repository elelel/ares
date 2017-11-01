cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(server_test VERSION 2.2.4 LANGUAGES C CXX)
include(CTest)

get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

if (NOT ARES_PACKET_VER)
  message(FATAL_ERROR "ARES_PACKET_VER is not defined. Set it with -DARES_PACKET_VER=your_version command line argument to CMake")
else()
  message("Compiling for packet version ${ARES_PACKET_VER}")
endif()

include(${ARES_DIR}/projects/CMake/shared.cmake)

set(PACKETS "${ARES_DIR}/packets")
set(SERVER "${ARES_DIR}/server")
set(CONTRIB "${ARES_DIR}/contrib")

set(INCLUDE_DIRS
  ${ARES_INCLUDE_DIRECTORIES}
  ${PACKETS}/include
  ${CONTRIB}/spdlog/include
  ${CONTRIB}/network_buffer/include
  ${SERVER}/include
  )

enable_testing()

if (BUILD_TESTING)
  set(TEST_SOURCES
    test/src/cv_lock.cpp
    )

  set(INCLUDE_DIRS
    ${INCLUDE_DIRS}
    ${CONTRIB}/Catch/include

    )

  add_executable(server_test ${TEST_SOURCES})
  set_target_properties(server_test PROPERTIES
    COMPILE_FLAGS "${ARES_COMPILE_FLAGS}"
    LINK_FLAGS "${ARES_LINK_FLAGS}"
    )
  target_link_libraries(server_test ${ARES_LINK_LIBRARIES})
  target_include_directories(server_test PRIVATE ${INCLUDE_DIRS})
  target_compile_definitions(server_test PRIVATE ${ARES_COMPILE_DEFINITIONS} "CATCH_CONFIG_MAIN" "CATCH_CONFIG_COLOUR_NONE")

  add_test(NAME RunServerTests WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" COMMAND server_test)
endif()
