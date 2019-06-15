#include "headers/huffman_file_compressor.h"
#include "headers/buffered_reader.h"

huffman_file_compressor::huffman_file_compressor() : encoder() {}

void huffman_file_compressor::compress(const std::string &input_file, const std::string &output_file) {
    buffered_reader reader(input_file);

    while (!reader.is_eof()) {
        encoder.add_data(reader.next_char());
    }

    encoder.prepare_for_encoding();

    reader.close();
    reader.open(input_file);

    buffered_writer writer(output_file);

    // Write number of bits in code
    writer.print_long(encoder.get_bits_count());

    auto tree_encoding = encoder.encode_tree();
    // Write number of bits in tree code
    writer.print_long(tree_encoding.first.size());

    // Write tree code
    compress_bools(tree_encoding.first, writer);

    // Write tree permutation
    for (symbol c : tree_encoding.second) {
        writer.print_char(c);
    }

    // Write code
    code_word buffer;
    while (!reader.is_eof()) {
        code_word current_code = encoder.encode_data(reader.next_char());

        while (current_code.get_len() > 0) {
            if (buffer.get_len() == 8) {
                writer.print_char(static_cast<symbol>(buffer.get_code()));
                buffer.clear();
            }
            buffer.add_low_bit(current_code.get_high_bit());
            current_code.remove_high_bit();
        }
    }

    if (buffer.get_len() > 0) {
        while (buffer.get_len() < 8) {
            buffer.add_low_bit(false);
        }

        writer.print_char(static_cast<symbol>(buffer.get_code()));
    }

    reader.close();
    writer.close();
}

void huffman_file_compressor::compress_bools(const std::vector<bool> &data, buffered_writer& writer) {
    code_word buffer;
    compress_bools(data, writer, buffer);
}

void huffman_file_compressor::compress_bools(const std::vector<bool> &data, buffered_writer &writer, code_word& buffer) {
    for (bool c : data) {
        if (buffer.get_len() == 8) {
            writer.print_char(static_cast<symbol>(buffer.get_code()));
            buffer.clear();
        }
        buffer.add_low_bit(c);
    }

    if (buffer.get_len() > 0) {
        while (buffer.get_len() < 8) {
            buffer.add_low_bit(false);
        }

        writer.print_char(static_cast<symbol>(buffer.get_code()));
    }
}