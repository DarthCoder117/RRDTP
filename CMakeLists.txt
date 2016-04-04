cmake_minimum_required (VERSION 2.6)
project(rrdtp-lib)

include_directories("${PROJECT_SOURCE_DIR}/source")

file(GLOB rrdtp_SRC "${PROJECT_SOURCE_DIR}/source/*.h" "${PROJECT_SOURCE_DIR}/source/*.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/shared)

add_library(rrdtp-lib STATIC ${rrdtp_SRC})