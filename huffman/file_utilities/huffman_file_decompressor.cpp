#include "headers/huffman_file_decompressor.h"
#include "headers/buffered_writer.h"
#include "headers/buffered_reader.h"
#include <algorithm>

huffman_file_decompressor::huffman_file_decompressor() : decoder() {}

void huffman_file_decompressor::decompress(const std::string &input_file, const std::string &output_file) {
    buffered_reader reader(input_file);

    ull bits_count = reader.next_long();
    ull tree_size = reader.next_long();

    std::vector<bool> tree_encoding;
    std::vector<symbol> permutation;

    for (ull i = 0; i < tree_size; i += 8) {
        symbol c = reader.next_char();

        for (ull j = 0; j < std::min((ull)8, tree_size - i); ++j) {
            tree_encoding.push_back(static_cast<bool>((c >> (7 - j)) & 1));
        }
    }

    for (size_t i = 0; i <= MAX_SYMBOL; ++i) {
        permutation.push_back(reader.next_char());
    }

    decoder.prepare_for_decoding(tree_encoding, permutation);

    buffered_writer writer(output_file);

    for (ull i = 0; i < bits_count; i += 8) {
        symbol c = reader.next_char();

        for (ull j = 0; j < std::min((ull)8, bits_count - i); ++j) {
            auto res = decoder.add_bit(static_cast<bool>((c >> (7 - j)) & 1));

            if (res.second) {
                writer.print_char(res.first);
            }
        }
    }

    decoder.check();

    reader.close();
    writer.close();
}