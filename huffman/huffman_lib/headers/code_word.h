#ifndef HUFFMAN_CODE_WORD_H
#define HUFFMAN_CODE_WORD_H

#include "constants.h"

struct code_word {
    code_word() = default;
    code_word(ull code, size_t len);

    void add_high_bit(bool bit);
    void remove_high_bit();
    void add_low_bit(bool bit);
    void remove_low_bit();

    void clear();

    bool get_bit(size_t index) const;
    bool get_high_bit() const;
    bool get_low_bit() const;

    ull get_code() const;
    ull get_len() const;

private:
    ull code = 0;
    size_t len = 0;
};

#endif //HUFFMAN_CODE_WORD_H
