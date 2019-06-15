#include "headers/buffered_writer.h"

buffered_writer::buffered_writer(const std::string& file_name) : writer(file_name, std::ofstream::binary), ptr(0) {}

buffered_writer::~buffered_writer() {
    if (writer.is_open()) {
        writer.write(buffer, ptr);
        writer.close();
    }
}

void buffered_writer::print_char(symbol c) {
    if (ptr == FILE_BUFFER_SIZE) {
        writer.write(buffer, FILE_BUFFER_SIZE);
        ptr = 0;
    }
    buffer[ptr++] = static_cast<char>(c);
}

void buffered_writer::print_long(ull c) {
    for (size_t i = 0; i < 8; ++i) {
        print_char(static_cast<symbol>(c & 255));
        c >>= 8;
    }
}

void buffered_writer::close() {
    writer.write(buffer, ptr);
    writer.close();
}