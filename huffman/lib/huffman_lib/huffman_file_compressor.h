#ifndef HUFFMAN_HUFFMAN_FILE_COMPRESSOR_H
#define HUFFMAN_HUFFMAN_FILE_COMPRESSOR_H

#include "../../constants.h"
#include "huffman_encoder.h"
#include <string>

struct huffman_file_compressor {
    huffman_file_compressor();

    void compress(const std::string& input_file, const std::string& output_file);

private:
    huffman_encoder encoder;
};

#endif //HUFFMAN_HUFFMAN_FILE_COMPRESSOR_H
