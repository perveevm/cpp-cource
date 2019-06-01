#ifndef HUFFMAN_CONSTANTS_H
#define HUFFMAN_CONSTANTS_H

#include <cstdlib>
#include <cstdint>

typedef unsigned char symbol;
typedef uint64_t ull;

const size_t FILE_BUFFER_SIZE = 1024;
const size_t MAX_SYMBOL = 255;
const size_t NONE = 1u << 31;

#endif //HUFFMAN_CONSTANTS_H
