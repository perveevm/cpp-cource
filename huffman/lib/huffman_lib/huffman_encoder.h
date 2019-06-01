#ifndef HUFFMAN_HUFFMAN_ENCODER_H
#define HUFFMAN_HUFFMAN_ENCODER_H

#include "../../constants.h"
#include "code_word.h"
#include "huffman_tree.h"
#include <vector>

struct huffman_encoder {
    huffman_encoder();

    void add_data(const std::vector<symbol>& data);
    void add_data(symbol data);
    void prepare_for_encoding();
    std::vector<code_word> encode_data(const std::vector<symbol>& data) const;
    code_word encode_data(symbol data) const;
    std::vector<bool> encode_tree() const;
    ull get_bits_count() const;

private:
    huffman_tree tree;
    accumulator accum;
    bool ready = false;
};

#endif //HUFFMAN_HUFFMAN_ENCODER_H
