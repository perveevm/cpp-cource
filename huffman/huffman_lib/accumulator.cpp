#include <cstring>
#include "headers/accumulator.h"

accumulator::accumulator() {
    memset(cnt, 0, sizeof(cnt));
}

void accumulator::add_data(const std::vector<symbol> &data) {
    for (symbol c : data) {
        ++cnt[c];
    }
}

void accumulator::add_data(symbol data) {
    ++cnt[data];
}

ull accumulator::get_cnt(symbol index) const {
    return cnt[index];
}