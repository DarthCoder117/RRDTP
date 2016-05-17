cmake_minimum_required (VERSION 2.6)
project(rrdtp)

include_directories("${PROJECT_SOURCE_DIR}/source")

file(GLOB_RECURSE rrdtp_SRC "${PROJECT_SOURCE_DIR}/source/*.h" "${PROJECT_SOURCE_DIR}/source/*.cpp")

#Output directory CPU architecture extension (x86 or x64 on Windows MSVC)
set(archExtension "")

message(STATUS "Checking for architecture")
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	message(STATUS "Using MSVC compiler")
	if (${CMAKE_GENERATOR} MATCHES "Win64")
		message(STATUS "Building for x64")
		set(archExtension "x64")
	elseif (${CMAKE_GENERATOR} MATCHES "ARM")
		message(STATUS "Building for ARM")
		set(archExtension "ARM")
	else()
		message(STATUS "Building for x86")
		set(archExtension "x86")
	endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_CXX_COMPILER_ID}/${archExtension}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_CXX_COMPILER_ID}/${archExtension}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_CXX_COMPILER_ID}/${archExtension}")

#Core RRDTP library
add_library(rrdtp_static STATIC ${rrdtp_SRC})
add_library(rrdtp_module MODULE ${rrdtp_SRC})
#TODO: Link the static version into the module so we don't have to build twice.

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
