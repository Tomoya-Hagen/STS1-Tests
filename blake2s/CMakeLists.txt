cmake_minimum_required(VERSION 3.10)

project(blake2s_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(test_blake2s test.cpp Crypto.cpp BLAKE2s.cpp Hash.cpp)

target_include_directories(test_blake2s PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})