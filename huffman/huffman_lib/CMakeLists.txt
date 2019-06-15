cmake_minimum_required(VERSION 3.13)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2 -pedantic")
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

include_directories(headers)

set(LIB_HEADERS headers/accumulator.h
                headers/code_word.h
                headers/constants.h
                headers/huffman_decoder.h
                headers/huffman_encoder.h
                headers/huffman_tree.h)

set(LIB_SOURCES accumulator.cpp
                code_word.cpp
                huffman_decoder.cpp
                huffman_encoder.cpp
                huffman_tree.cpp)

add_library(huffman_lib ${LIB_HEADERS} ${LIB_SOURCES})