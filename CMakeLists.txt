cmake_minimum_required(VERSION 3.5)

project(PONG VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_DEBUG_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(Vendor/SDL)

file(GLOB_RECURSE PONG_SOURCES CONFIGURE_DEPENDS "Game/*.c")

add_executable(PONG ${PONG_SOURCES})
target_include_directories(PONG PUBLIC Game/ Vendor/SDL)
target_link_libraries(PONG PUBLIC PUBLIC SDL2::SDL2)
