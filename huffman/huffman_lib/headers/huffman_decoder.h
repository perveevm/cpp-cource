#ifndef HUFFMAN_HUFFMAN_DECODER_H
#define HUFFMAN_HUFFMAN_DECODER_H

#include <vector>
#include "constants.h"
#include "huffman_tree.h"


struct huffman_decoder {
    huffman_decoder();

    void prepare_for_decoding(const std::vector<bool> &encoding, const std::vector<symbol> &permutation);
    std::pair<symbol, bool> add_bit(bool bit);
    void check() const;
private:
    bool ready = false;
    huffman_tree tree;
    size_t tree_ptr = 0;
};

#endif //HUFFMAN_HUFFMAN_DECODER_H
