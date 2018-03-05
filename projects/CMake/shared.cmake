option(LOG_TRACE "Enable trace in spdlog" OFF)

set(ARES_COMPILE_DEFINITIONS "ASIO_STANDALONE")

if (ARES_PACKET_SET)
  set(ARES_COMPILE_DEFINITIONS ${ARES_COMPILE_DEFINITIONS} "ARES_PACKET_SET=${ARES_PACKET_SET}")
endif()  

if (LOG_TRACE)
  message("Trace log macro is on")
  set(ARES_COMPILE_DEFINITIONS ${ARES_COMPILE_DEFINITIONS} "SPDLOG_TRACE_ON")
else()
  message("Trace log macro is off")
endif()

find_package(Threads REQUIRED)
set(ARES_LINK_LIBRARIES ${ARES_LINK_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

include("${ARES_DIR}/projects/CMake/libpq.cmake")
include("${ARES_DIR}/projects/CMake/libpqxx.cmake")

if (WIN32)
  if (MINGW)
    set(ARES_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Werror -Wunused")
  elseif(MSVC)
    set(ARES_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -std:c++latest")
  endif()
  set(ARES_COMPILE_DEFINITIONS "${ARES_COMPILE_DEFINITIONS}; NOMINMAX")  
else()
  set(ARES_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Werror -Wunused -Wno-unused-private-field")
endif()

set(ARES_INCLUDE_DIRECTORIES ${ARES_INCLUDE_DIRECTORIES} ${ARES_POSTGRES_INCLUDE_DIRS})
set(ARES_LINK_LIBRARIES ${ARES_LINK_LIBRARIES} ${ARES_POSTGRES_LIBS})
    
message("Ares include directories: ${ARES_INCLUDE_DIRECTORIES}")
message("Ares link libraries: ${ARES_LINK_LIBRARIES}")
message("Ares compile definitins: ${ARES_COMPILE_DEFINITIONS}")
