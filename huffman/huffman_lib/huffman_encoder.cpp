#include <stdexcept>
#include "headers/huffman_encoder.h"

huffman_encoder::huffman_encoder() : tree(), accum() {}

void huffman_encoder::add_data(const std::vector<symbol>& data) {
    if (ready) {
        throw std::runtime_error("Tree has already been built");
    }

    accum.add_data(data);
}

void huffman_encoder::add_data(symbol data) {
    if (ready) {
        throw std::runtime_error("Tree has already been built");
    }

    accum.add_data(data);
}

void huffman_encoder::prepare_for_encoding() {
    if (ready) {
        throw std::runtime_error("Tree has already been built");
    }

    tree.build_huffman_tree(accum);
    ready = true;
}

std::vector<code_word> huffman_encoder::encode_data(const std::vector<symbol>& data) const {
    if (!ready) {
        throw std::runtime_error("Tree hasn't been built");
    }

    std::vector<code_word> encoding;

    encoding.reserve(data.size());
    for (symbol c : data) {
        encoding.push_back(tree.get_code(c));
    }

    return encoding;
}

code_word huffman_encoder::encode_data(symbol data) const {
    if (!ready) {
        throw std::runtime_error("Tree hasn't been built");
    }

    return tree.get_code(data);
}

std::pair<std::vector<bool>, std::vector<symbol>> huffman_encoder::encode_tree() const {
    if (!ready) {
        throw std::runtime_error("Tree hasn't been built");
    }

    return std::make_pair(tree.get_tree_code(), tree.get_permutation());
}

ull huffman_encoder::get_bits_count() const {
    ull res = 0;

    for (size_t c = 0; c <= MAX_SYMBOL; ++c) {
        res += accum.get_cnt(static_cast<symbol>(c)) * tree.get_code(static_cast<symbol>(c)).get_len();
    }

    return res;
}