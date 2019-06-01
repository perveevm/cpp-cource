cmake_minimum_required(VERSION 3.13)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

add_executable(huffman main.cpp lib/file_utilities/buffered_reader.h constants.h lib/file_utilities/buffered_reader.cpp lib/file_utilities/buffered_writer.h lib/file_utilities/buffered_writer.cpp lib/huffman_lib/accumulator.h lib/huffman_lib/accumulator.cpp lib/huffman_lib/code_word.h lib/huffman_lib/code_word.cpp lib/huffman_lib/huffman_tree.h lib/huffman_lib/huffman_tree.cpp lib/huffman_lib/huffman_encoder.h lib/huffman_lib/huffman_encoder.cpp lib/huffman_lib/huffman_file_compressor.h lib/huffman_lib/huffman_file_compressor.cpp)