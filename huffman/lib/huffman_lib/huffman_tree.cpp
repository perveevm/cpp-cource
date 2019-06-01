#include "huffman_tree.h"
#include <set>

void huffman_tree::build_huffman_tree(const accumulator &accum) {
    tree.resize(MAX_SYMBOL + 1);
    std::set<std::pair<ull, size_t>> q;

    for (symbol c = 0; c <= MAX_SYMBOL; ++c) {
        tree[c] = node(c);
        q.insert(std::make_pair(accum.get_cnt(c), c));
    }

    while (q.size() > 1) {
        auto left = *q.begin();
        q.erase(q.begin());
        auto right = *q.begin();
        q.erase(q.begin());

        tree.emplace_back(left.second, right.second);
        q.insert(std::make_pair(left.first + right.first, tree.size() - 1));
    }

    root = q.begin()->second;

    code_word code;
    dfs(root, code);
}

code_word huffman_tree::get_code(symbol c) const {
    return codes[c];
}

std::vector<bool> huffman_tree::get_tree_code() const {
    return tree_encoding;
}

void huffman_tree::dfs(size_t v, code_word& code) {
    if (tree[v].link[0] == NONE && tree[v].link[1] == NONE) {
        codes[tree[v].c] = code;
        return;
    }

    code.add_high_bit(false);
    tree_encoding.push_back(true);

    dfs(tree[v].link[0], code);

    tree_encoding.push_back(false);
    code.remove_high_bit();
    code.add_high_bit(true);
    tree_encoding.push_back(true);

    dfs(tree[v].link[0], code);

    tree_encoding.push_back(false);
    code.remove_high_bit();
}