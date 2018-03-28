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

include("${ARES_DIR}/projects/CMake/libpqxx.cmake")
include("${ARES_DIR}/projects/CMake/libpq.cmake")
include("${ARES_DIR}/projects/CMake/zlib.cmake")

if (WIN32)
  include("${ARES_DIR}/projects/CMake/winver.cmake")
  if (MINGW)
    set(ARES_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wunused")
  elseif(MSVC)
    set(ARES_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ")
    set(ARES_COMPILE_DEFINITIONS ${ARES_COMPILE_DEFINITIONS}
                                 "_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING"
                                 "_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING"
                                 "_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING" )
  endif()
  set(ARES_COMPILE_DEFINITIONS ${ARES_COMPILE_DEFINITIONS} "NOMINMAX")
else()
  set(ARES_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wunused -Wno-unused-private-field")
  set(ARES_LINK_LIBRARIES ${ARES_LINK_LIBRARIES} stdc++fs)
endif()

set(ARES_INCLUDE_DIRECTORIES ${ARES_INCLUDE_DIRECTORIES} ${ARES_POSTGRES_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
set(ARES_LINK_LIBRARIES ${ARES_LINK_LIBRARIES} ${ARES_POSTGRES_LIBS} ${ZLIB_LIBRARIES})

include("${ARES_DIR}/projects/CMake/platform_checks.cmake")

message("Ares compile definitions: ${ARES_COMPILE_DEFINITIONS}")
message("Ares compile flags: ${ARES_COMPILE_FLAGS}")
message("Ares include directories: ${ARES_INCLUDE_DIRECTORIES}")
message("Ares server link libraries: ${ARES_LINK_LIBRARIES}")


