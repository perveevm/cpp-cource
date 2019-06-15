#include "headers/code_word.h"

code_word::code_word(ull code, size_t len) : code(code), len(len) {}

void code_word::add_high_bit(bool bit) {
    if (bit) {
        code |= 1ull << len;
    }
    ++len;
}

void code_word::remove_high_bit() {
    code &= UINT64_MAX ^ (1ull << (--len));
}

void code_word::add_low_bit(bool bit) {
    code <<= 1ull;
    if (bit) {
        code |= 1;
    }
    ++len;
}

void code_word::remove_low_bit() {
    code >>= 1;
    code &= UINT64_MAX ^ (1ull << 63);
    --len;
}

void code_word::clear() {
    code = 0;
    len = 0;
}

bool code_word::get_bit(size_t index) const {
    return static_cast<bool>((code >> index) & 1);
}

bool code_word::get_high_bit() const {
    return get_bit(len - 1);
}

bool code_word::get_low_bit() const {
    return get_bit(0);
}

ull code_word::get_code() const {
    return code;
}

ull code_word::get_len() const {
    return len;
}