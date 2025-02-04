cmake_minimum_required(VERSION 3.16)

project(TM_TC_Encode_Decode_Test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    encode_decode.cpp
    test.cpp
    ../viterbi/viterbi.cpp)

add_executable(encode_decode_test ${SOURCES})

include_directories(../viterbi
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../reed_solomon
    ../bit_scrambling)
