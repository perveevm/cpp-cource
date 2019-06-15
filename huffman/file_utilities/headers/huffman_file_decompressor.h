#ifndef HUFFMAN_HUFFMAN_FILE_DECOMPRESSOR_H
#define HUFFMAN_HUFFMAN_FILE_DECOMPRESSOR_H

#include <string>
#include "huffman_decoder.h"

struct huffman_file_decompressor {
    huffman_file_decompressor();

    void decompress(const std::string& input_file, const std::string& output_file);

private:
    huffman_decoder decoder;
};

#endif //HUFFMAN_HUFFMAN_FILE_DECOMPRESSOR_H
