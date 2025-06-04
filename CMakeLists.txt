cmake_minimum_required(VERSION 3.10.0)
project(file_system VERSION 0.1.0 LANGUAGES C CXX)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(file_system main.cpp)

include(CTest)
enable_testing()

