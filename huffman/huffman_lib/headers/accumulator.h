#ifndef HUFFMAN_ACCUMULATOR_H
#define HUFFMAN_ACCUMULATOR_H

#include "constants.h"
#include <vector>

struct accumulator {
    accumulator();

    void add_data(const std::vector<symbol>& data);
    void add_data(symbol data);
    ull get_cnt(symbol index) const;

private:
    ull cnt[MAX_SYMBOL + 1];
};

#endif //HUFFMAN_ACCUMULATOR_H
