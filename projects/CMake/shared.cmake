option(LOG_TRACE "Enable trace in spdlog" OFF)

set(ARES_COMPILE_DEFINITIONS "")

if (ARES_PACKET_VER)
  set(ARES_COMPILE_DEFINITIONS ${ARES_COMPILE_DEFINITIONS} "ARES_PACKET_VER=${ARES_PACKET_VER}")
endif()  

if (LOG_TRACE)
  message("Trace log macro is on")
  set(ARES_COMPILE_DEFINITIONS ${ARES_COMPILE_DEFINITIONS} "SPDLOG_TRACE_ON")
else()
  message("Trace log macro is off")
endif()
  
find_package(Boost 1.57.0 REQUIRED system)
if(NOT Boost_FOUND)
  message(FATAL_ERROR "Could not find boost. Try setting BOOST_INCLUDEDIR and BOOST_LIBRARYDIR. See https://cmake.org/cmake/help/v3.0/module/FindBoost.html for more info.")
endif()
set(ARES_INCLUDE_DIRECTORIES ${ARES_INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS})
set(ARES_LINK_LIBRARIES ${ARES_LINK_LIBRARIES} ${Boost_SYSTEM_LIBRARY})

find_package(Threads REQUIRED)
set(ARES_LINK_LIBRARIES ${ARES_LINK_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

include("${ARES_DIR}/projects/CMake/libpq.cmake")
include("${ARES_DIR}/projects/CMake/libpqxx.cmake")

if (WIN32)
  if (MINGW)
    set(ARES_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Werror -Wunused")
  elseif(MSVC)
    set(ARES_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -std:c++latest")
  endif()
  set(ARES_COMPILE_DEFINITIONS "${ARES_COMPILE_DEFINITIONS}; NOMINMAX")  
else()
  set(ARES_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Werror -Wunused")
endif()

set(ARES_INCLUDE_DIRECTORIES ${ARES_INCLUDE_DIRECTORIES} ${ARES_POSTGRES_INCLUDE_DIRS})
set(ARES_LINK_LIBRARIES ${ARES_LINK_LIBRARIES} ${ARES_POSTGRES_LIBS})
    
message("Ares include directories: ${ARES_INCLUDE_DIRECTORIES}")
message("Ares link libraries: ${ARES_LINK_LIBRARIES}")
message("Ares compile definitins: ${ARES_COMPILE_DEFINITIONS}")
