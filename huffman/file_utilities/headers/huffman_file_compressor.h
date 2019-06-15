#ifndef HUFFMAN_HUFFMAN_FILE_COMPRESSOR_H
#define HUFFMAN_HUFFMAN_FILE_COMPRESSOR_H

#include "../../huffman_lib/headers/constants.h"
#include "../../huffman_lib/headers/huffman_encoder.h"
#include "buffered_writer.h"
#include <string>

struct huffman_file_compressor {
    huffman_file_compressor();

    void compress(const std::string& input_file, const std::string& output_file);

private:
    huffman_encoder encoder;

    void compress_bools(const std::vector<bool>& data, buffered_writer& writer);
    void compress_bools(const std::vector<bool>& data, buffered_writer& writer, code_word& buffer);
};

#endif //HUFFMAN_HUFFMAN_FILE_COMPRESSOR_H
