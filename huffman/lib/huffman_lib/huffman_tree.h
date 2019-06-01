#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "code_word.h"
#include "accumulator.h"

struct huffman_tree {
    huffman_tree() = default;

    void build_huffman_tree(const accumulator& accum);
    code_word get_code(symbol c) const;
    std::vector<bool> get_tree_code() const;
private:
    struct node {
        std::vector<size_t> link;
        symbol c = 0;

        node() : link(2, NONE) {}
        explicit node(symbol c) : link(2, NONE), c(c) {}
        node(size_t l, size_t r) {
            link.resize(2);
            link[0] = l;
            link[1] = r;
        }
    };

    void dfs(size_t v, code_word& code);

    std::vector<node> tree;
    std::vector<bool> tree_encoding;
    code_word codes[MAX_SYMBOL + 1];
    size_t root = 0;
};

#endif //HUFFMAN_TREE_H
