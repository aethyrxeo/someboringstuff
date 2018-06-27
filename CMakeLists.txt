project(static_patterns C CXX)
cmake_minimum_required(VERSION 3.8.2)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

find_package(Boost)

include(AddTest)

add_subdirectory(tests)