#include <stdexcept>
#include "headers/huffman_decoder.h"

huffman_decoder::huffman_decoder() : tree() {}

void huffman_decoder::prepare_for_decoding(const std::vector<bool> &encoding, const std::vector<symbol> &permutation) {
    if (ready) {
        throw std::runtime_error("Tree has already been built");
    }

    ready = true;
    tree.build_huffman_tree(encoding, permutation);
    tree_ptr = tree.get_tree_root();
}

std::pair<symbol, bool> huffman_decoder::add_bit(bool bit) {
    if (!ready) {
        throw std::runtime_error("Tree has not been built");
    }

    return tree.add_bit(tree_ptr, bit);
}

void huffman_decoder::check() const {
    if (tree_ptr != tree.get_tree_root()) {
        throw std::runtime_error("Decompression failed: last position is not a root");
    }
}