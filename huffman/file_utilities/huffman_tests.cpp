#include <iostream>
#include <random>

#include "headers/buffered_reader.h"
#include "headers/buffered_writer.h"

#include "headers/huffman_file_compressor.h"
#include "headers/huffman_file_decompressor.h"

bool compare_files(const std::string& name1, const std::string& name2) {
    buffered_reader reader1(name1);
    buffered_reader reader2(name2);

    while (!reader1.is_eof() && !reader2.is_eof()) {
        symbol c1 = reader1.next_char();
        symbol c2 = reader2.next_char();

        if (c1 != c2) {
            return false;
        }
    }

    bool res = !(!reader1.is_eof() || !reader2.is_eof());

    reader1.close();
    reader2.close();

    return res;
}

void generate_test(size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    buffered_writer writer("test.txt");

    for (size_t i = 0; i < size; ++i) {
        auto c = static_cast<symbol>(dis(gen));
        writer.print_char(c);
    }

    writer.close();
}

void check_test(size_t size) {
    generate_test(size);

    huffman_file_compressor compressor;
    compressor.compress("test.txt", "compressed.txt");

    huffman_file_decompressor decompressor;
    decompressor.decompress("compressed.txt", "decompressed.txt");

    if (compare_files("test.txt", "decompressed.txt")) {
        std::cout << "Size " << size << ": OK!" << std::endl;
    } else {
        std::cout << "Size " << size << ": Error!" << std::endl;
        exit(0);
    }
}

int main() {
    check_test(0);
    check_test(10);
    check_test(25);
    check_test(150);
    check_test(1024);
    check_test(1500);
    check_test(2000);
    check_test(100000);
    check_test(1000000);
    return 0;
}