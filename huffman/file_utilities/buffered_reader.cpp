#include "headers/buffered_reader.h"

buffered_reader::buffered_reader(const std::string& file_name) : reader(file_name, std::ifstream::binary), ptr(0), len(0) {
    if (reader.fail()) {
        reader.close();
        throw std::runtime_error("No such file");
    }
}

buffered_reader::~buffered_reader() {
    if (reader.is_open()) {
        reader.close();
    }
}

void buffered_reader::close() {
    if (!reader.is_open()) {
        throw std::runtime_error("File is not opened");
    }

    reader.close();
}

void buffered_reader::open(const std::string &file_name) {
    if (reader.is_open()) {
        throw std::runtime_error("File is opened");
    }

    reader.open(file_name);
}

bool buffered_reader::is_eof() {
    return (ptr == len) && (reader.peek() == EOF);
}

symbol buffered_reader::next_char() {
    if (is_eof()) {
        throw std::runtime_error("Expected char, but found EOF");
    }

    if (ptr == len) {
        reader.read(buffer, FILE_BUFFER_SIZE);
        len = static_cast<size_t>(reader.gcount());
        ptr = 0;
    }
    return static_cast<symbol>(buffer[ptr++]);
}

ull buffered_reader::next_long() {
    ull res = 0;
    for (size_t i = 0; i < 8; ++i) {
        res |= static_cast<ull>(next_char()) << (8 * i);
    }
    return res;
}