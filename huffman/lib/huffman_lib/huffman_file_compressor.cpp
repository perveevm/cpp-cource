#include "huffman_file_compressor.h"
#include "../file_utilities/buffered_reader.h"
#include "../file_utilities/buffered_writer.h"

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
        writer.print_char(static_cast<symbol>(buffer.get_code()));
    }

    reader.close();
    writer.close();
}