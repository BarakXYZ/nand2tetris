#include "parser.h"
#include "code_writer.h"
#include <iostream>

auto main(int argc, char *argv[]) -> int {

    // Debug
    // std::string inFileName{"test_files/test.asm"};
    // std::string outFileName{"out_test.hack"};

    // Production
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    // char inFileName[strlen(argv[1]) + 1];
    // char outFileName[strlen(argv[2]) + 1];
    std::string inFileName{argv[1]};
    std::string outFileName{argv[2]};

    std::unique_ptr<std::ifstream> inFile{std::make_unique<std::ifstream>(inFileName)};
    if (!inFile) {
        std::cerr << "Error opening input file: " << inFileName << std::endl;
        return 1;
    }
    // Method A: Using peek() and eof()
    if (inFile->peek() == std::ifstream::traits_type::eof()) {
        std::cout << "File is empty.\n";
    }

    // Clear and Write Parsed String Stream to the outFile
    std::unique_ptr<std::ofstream> outFile{std::make_unique<std::ofstream>(outFileName, std::ios::trunc)};
    if (!outFile) {
        std::cerr << "Error opening output file: " << outFileName << std::endl;
        return 1;
    }

    // Construct Parser & CodeWriter
    // Parser parser{std::move(inFile)};
    CodeWriter code_writer{std::move(outFile)};

    std::string line{};
    while (std::getline(*inFile >> std::ws, line)) {
        std::cout << line << '\n';
    }

}
