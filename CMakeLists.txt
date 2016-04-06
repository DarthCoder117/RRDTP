cmake_minimum_required (VERSION 2.6)
project(rrdtp)

include_directories("${PROJECT_SOURCE_DIR}/source")

file(GLOB_RECURSE rrdtp_SRC "${PROJECT_SOURCE_DIR}/source/*.h" "${PROJECT_SOURCE_DIR}/source/*.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/static")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/shared")

#Core RRDTP library
add_library(rrdtp-lib STATIC ${rrdtp_SRC})

#Tests
add_executable(socket-test "${PROJECT_SOURCE_DIR}/test/SocketTest.cpp")
target_link_libraries (socket-test rrdtp-lib)
