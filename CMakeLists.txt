cmake_minimum_required(VERSION 3.10.0)
project(file_system VERSION 0.1.0 LANGUAGES C CXX)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(file_system src/main.cpp
src/disk.cpp
src/inode.cpp
src/directory.cpp
src/inode_manager.cpp
src/fs.cpp
src/fileop.cpp
)

add_executable(test_disk test/test_disk.cpp
src/disk.cpp
)

add_executable(test_inode test/test_inode.cpp
src/disk.cpp
src/inode.cpp)

add_executable(test_directory test/test_directory.cpp
src/directory.cpp)

add_executable(test_fs test/test_fs.cpp
src/fs.cpp
src/inode_manager.cpp
src/disk.cpp
src/inode.cpp
src/directory.cpp)
include(CTest)
enable_testing()

