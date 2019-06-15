#ifndef HUFFMAN_BUFFERED_READER_H
#define HUFFMAN_BUFFERED_READER_H

#include <string>
#include <fstream>
#include "../../huffman_lib/headers/constants.h"

struct buffered_reader {
    buffered_reader() = delete;
    explicit buffered_reader(const std::string& file_name);
    ~buffered_reader();

    void close();
    void open(const std::string& file_name);
    bool is_eof();
    symbol next_char();
    ull next_long();

private:
    std::ifstream reader;
    char buffer[FILE_BUFFER_SIZE];
    size_t ptr = 0, len = 0;
};

#endif //HUFFMAN_BUFFERED_READER_H
