cmake_minimum_required (VERSION 2.6)
project(rrdtp)

include_directories("${PROJECT_SOURCE_DIR}/source")

file(GLOB_RECURSE rrdtp_SRC "${PROJECT_SOURCE_DIR}/source/*.h" "${PROJECT_SOURCE_DIR}/source/*.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/static")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/shared")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

#Core RRDTP library
add_library(rrdtp_lib OBJECT ${rrdtp_SRC})

add_library(rrdtp_static STATIC $<TARGET_OBJECTS:rrdtp_lib>)
add_library(rrdtp_module MODULE $<TARGET_OBJECTS:rrdtp_lib>)

#Tests
add_executable(server-socket-test "${PROJECT_SOURCE_DIR}/test/ServerSocketTest.cpp")
target_link_libraries (server-socket-test rrdtp_static)
add_executable(client-socket-test "${PROJECT_SOURCE_DIR}/test/ClientSocketTest.cpp")
target_link_libraries (client-socket-test rrdtp_static)

add_executable(local-store-test "${PROJECT_SOURCE_DIR}/test/LocalStoreTest.cpp")
target_link_libraries (local-store-test rrdtp_static)

add_executable(data-buffer-test "${PROJECT_SOURCE_DIR}/test/DataBufferTest.cpp")
target_link_libraries (data-buffer-test rrdtp_static)

add_executable(linked-list-test "${PROJECT_SOURCE_DIR}/test/LinkedListTest.cpp")
target_link_libraries (linked-list-test rrdtp_static)

add_executable(full-server-test "${PROJECT_SOURCE_DIR}/test/FullServerTest.cpp")
target_link_libraries (full-server-test rrdtp_static)
add_executable(full-client-test "${PROJECT_SOURCE_DIR}/test/FullClientTest.cpp")
target_link_libraries (full-client-test rrdtp_static)
