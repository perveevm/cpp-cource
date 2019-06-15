#include "headers/huffman_file_compressor.h"
#include "headers/huffman_file_decompressor.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Wrong number of arguments! Expected 3, found " << argc - 1 << std::endl;
        return 0;
    }

    std::string mode = argv[1];
    std::string input = argv[2];
    std::string output = argv[3];

    if (mode != "-c" && mode != "-d") {
        std::cerr << "Wrong mode! Expected -c or -d, found: " << mode << std::endl;
        return 0;
    }

    if (mode == "-c") {
        huffman_file_compressor compressor;
        try {
            compressor.compress(input, output);
        } catch (const std::runtime_error& e) {
            std::cerr << "Failed to compress" << std::endl;
            e.what();
        }
    } else {
        huffman_file_decompressor decompressor;
        try {
            decompressor.decompress(input, output);
        } catch (const std::runtime_error& e) {
            std::cerr << "Failed to decompress" << std::endl;
            e.what();
        }
    }

    std::cout << "Success!" << std::endl;

    return 0;
}