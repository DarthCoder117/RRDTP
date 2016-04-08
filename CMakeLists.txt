cmake_minimum_required (VERSION 2.6)
project(rrdtp)

include_directories("${PROJECT_SOURCE_DIR}/source")

file(GLOB_RECURSE rrdtp_SRC "${PROJECT_SOURCE_DIR}/source/*.h" "${PROJECT_SOURCE_DIR}/source/*.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/static")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/shared")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

#Core RRDTP library
add_library(rrdtp-lib STATIC ${rrdtp_SRC})

#Tests
add_executable(server-test "${PROJECT_SOURCE_DIR}/test/ServerTest.cpp")
target_link_libraries (server-test rrdtp-lib)

add_executable(client-test "${PROJECT_SOURCE_DIR}/test/ClientTest.cpp")
target_link_libraries (client-test rrdtp-lib)
