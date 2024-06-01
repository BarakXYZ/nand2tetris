// Implemented by BarakXYZ 2024
#include "Parser.h"

#include <fstream>
#include <iostream>
#include <sstream>

auto main(int argc, char *argv[]) -> int {

    // Debug
    // std::string infile_name{"test_files/test.asm"};
    // std::string outfile_name{"out_test.hack"};

    // Production
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }
    std::string infile_name{argv[1]};
    std::string outfile_name{argv[2]};


    std::ifstream infile(infile_name);
    if (!infile) {
        std::cerr << "Error opening input file: " << infile_name << std::endl;
        return 1;
    }

    // First-Pass: Parse Label Declaration i.e. (xxx)
    if (!Parser::parse_first_pass(infile)) {
        std::cerr << "First pass parsing failed." << std::endl;
        return 1;
    }

    // Reset the file stream to the beginning
    infile.clear();
    infile.seekg(0, std::ios::beg);

    // Parse Forward Declarations (@LOOP), @values (@R1, 1) & C-Instructions (dest=comp;jump)
    std::stringstream parsed_file;
    if (!Parser::parse_second_pass(infile, parsed_file)) {
        std::cerr << "Second pass parsing failed." << std::endl;
        return 1;
    }

    // Post-Parse
    infile.close();

    // Clear and Write Parsed String Stream to the Outfile
    std::ofstream outfile(outfile_name, std::ios::trunc);
    if (!outfile) {
        std::cerr << "Error opening output file: " << outfile_name << std::endl;
        return 1;
    }

    // Write
    std::string line;
    while (std::getline(parsed_file >> std::ws, line)) {
        outfile << line << '\n';
    }

    // Close Output File
    outfile.close();
}
