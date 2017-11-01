cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ares VERSION 1.0.0 LANGUAGES C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

get_filename_component(ARES_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)
get_filename_component(ARES_DIR "${ARES_DIR}" PATH)

add_subdirectory(${ARES_DIR}/account ${CMAKE_CURRENT_BINARY_DIR}/account_server)
#add_subdirectory(${ARES_DIR}/character ${CMAKE_CURRENT_BINARY_DIR}/character_server)


