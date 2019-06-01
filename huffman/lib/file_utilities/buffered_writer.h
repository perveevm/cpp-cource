#ifndef HUFFMAN_BUFFERED_WRITER_H
#define HUFFMAN_BUFFERED_WRITER_H

#include "../../constants.h"
#include <fstream>
#include <string>

struct buffered_writer {
    buffered_writer() = delete;
    explicit buffered_writer(const std::string& file_name);
    ~buffered_writer();

    void print_char(symbol c);
    void close();
private:
    std::ofstream writer;
    char buffer[FILE_BUFFER_SIZE];
    size_t ptr = 0;
};

#endif //HUFFMAN_BUFFERED_WRITER_H
