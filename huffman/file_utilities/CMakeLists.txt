cmake_minimum_required(VERSION 3.13)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2 -pedantic")
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

include_directories(headers)
include_directories(../huffman_lib/headers)

set(HEADERS headers/buffered_reader.h
            headers/buffered_writer.h
            headers/huffman_file_compressor.h
            headers/huffman_file_decompressor.h)

set(SOURCES1 buffered_reader.cpp
             buffered_writer.cpp
             huffman_file_compressor.cpp
             huffman_file_decompressor.cpp
             main.cpp)

set(SOURCES2 buffered_reader.cpp
             buffered_writer.cpp
             huffman_file_compressor.cpp
             huffman_file_decompressor.cpp
             huffman_tests.cpp)

add_executable(huffman ${HEADERS} ${SOURCES1})
add_executable(huffman_tests ${HEADERS} ${SOURCES2})
target_link_libraries(huffman huffman_lib)
target_link_libraries(huffman_tests huffman_lib)