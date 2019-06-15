#include "headers/huffman_tree.h"
#include <set>

void huffman_tree::build_huffman_tree(const accumulator &accum) {
    tree.resize(MAX_SYMBOL + 1);
    std::set<std::pair<ull, size_t>> q;

    for (size_t c = 0; c <= MAX_SYMBOL; ++c) {
        tree[c] = node(static_cast<symbol>(c & 255));
        q.insert(std::make_pair(accum.get_cnt(static_cast<symbol>(c & 255)), c));
    }

    while (q.size() > 1) {
        auto left = *q.begin();
        q.erase(q.begin());
        auto right = *q.begin();
        q.erase(q.begin());

        tree.push_back(node(left.second, right.second));
        q.insert(std::make_pair(left.first + right.first, tree.size() - 1));
    }

    root = q.begin()->second;

    code_word code;
    dfs(root, code);
}

void huffman_tree::build_huffman_tree(const std::vector<bool>& encoding, const std::vector<symbol>& permutation) {
    tree_encoding = encoding;
    symbol_permutation = permutation;

    code_word buffer;
    tree.push_back(node());

    size_t tree_ptr = 0;
    size_t permutation_ptr = 0;
    build_dfs(root, buffer, tree_ptr, permutation_ptr);
}

code_word huffman_tree::get_code(symbol c) const {
    return codes[c];
}

std::vector<bool> huffman_tree::get_tree_code() const {
    return tree_encoding;
}

std::vector<symbol> huffman_tree::get_permutation() const {
    return symbol_permutation;
}

size_t huffman_tree::get_tree_root() const {
    return root;
}

std::pair<symbol, bool> huffman_tree::add_bit(size_t& v, bool bit) const {
    size_t to = bit ? 1 : 0;

    if (tree[v].link[to] == NONE) {
        throw std::runtime_error("Decompression error: no such link in huffman tree");
    }

    v = tree[v].link[to];

    if (tree[v].link[0] == NONE && tree[v].link[1] == NONE) {
        symbol c = tree[v].c;
        v = root;

        return std::make_pair(c, true);
    } else {
        return std::make_pair(0, false);
    }
}

void huffman_tree::dfs(size_t v, code_word& code) {
    if (tree[v].link[0] == NONE && tree[v].link[1] == NONE) {
        codes[tree[v].c] = code;
        symbol_permutation.push_back(tree[v].c);
        return;
    }

    code.add_low_bit(false);
    tree_encoding.push_back(true);

    dfs(tree[v].link[0], code);

    tree_encoding.push_back(false);
    code.remove_low_bit();

    code.add_low_bit(true);
    tree_encoding.push_back(true);

    dfs(tree[v].link[1], code);

    tree_encoding.push_back(false);
    code.remove_low_bit();
}

void huffman_tree::build_dfs(size_t v, code_word& code, size_t& tree_ptr, size_t& permutation_ptr) {
    while (true) {
        if (tree_ptr == tree_encoding.size()) {
            if (v != root || permutation_ptr != symbol_permutation.size()) {
                throw std::runtime_error("Failed to build huffman tree: tree code has been corrupted");
            }

            break;
        }

        if (tree_encoding[tree_ptr]) {
            ++tree_ptr;
            tree.push_back(node());

            if (tree[v].link[0] == NONE) {
                tree[v].link[0] = tree.size() - 1;
                code.add_low_bit(false);

                build_dfs(tree[v].link[0], code, tree_ptr, permutation_ptr);

                code.remove_low_bit();
            } else if (tree[v].link[1] == NONE) {
                tree[v].link[1] = tree.size() - 1;
                code.add_low_bit(true);

                build_dfs(tree[v].link[1], code, tree_ptr, permutation_ptr);

                code.remove_low_bit();
            } else {
                throw std::runtime_error("Failed to build huffman tree: tree code has been corrupted");
            }
        } else {
            ++tree_ptr;

            if (tree[v].link[0] == NONE && tree[v].link[1] == NONE) {
                if (permutation_ptr == symbol_permutation.size()) {
                    throw std::runtime_error("Failed to build huffman tree: tree code has been corrupted");
                }

                tree[v].c = symbol_permutation[permutation_ptr++];
                codes[tree[v].c] = code;
            }

            break;
        }
    }
}