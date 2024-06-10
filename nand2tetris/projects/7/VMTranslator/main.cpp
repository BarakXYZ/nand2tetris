#include "parser.h"
#include "code_writer.h"
#include "utilities.h"
#include <iostream>

auto main(int argc, char *argv[]) -> int {

    constexpr std::string_view usage{" <input_file> <output_file>\n"};
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << usage;
        return 1;
    }

    const std::string inFileName{argv[1]};
    const std::string outFileName{argv[2]};

    std::unique_ptr<std::ifstream> inFile{
        std::make_unique<std::ifstream>(inFileName)};
    if (!inFile) {
        std::cerr << "Error opening input file: " << inFileName << std::endl;
        return 1;
    }

    // Check if file is empty
    if (inFile->peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "File is empty.\n";
        return 1;
    }

    // Clear and Write Parsed String Stream to the outFile
    std::unique_ptr<std::ofstream> outFile{
        std::make_unique<std::ofstream>(outFileName, std::ios::trunc)};
    if (!outFile) {
        std::cerr << "Error opening output file: " << outFileName << std::endl;
        return 1;
    }

    // Construct Parser & CodeWriter
    Parser parser{std::move(inFile)};
    CodeWriter codeWriter{std::move(outFile), cleanProgramName(argv[1])};
    
    while(parser.hasMoreCommands()) {
        parser.advance();
        if(parser.isCommentLine())
            continue;
        parser.splitCommandToFields();
        switch(parser.commandType()) {
            case vmCommand::C_PUSH:
                std::cout << "Push Command\n";
                codeWriter.writePushPop(vmCommand::C_PUSH, parser.getArg(1), parser.getArg(2));
                break;

            case vmCommand::C_POP:
                std::cout << "Pop Command\n";
                codeWriter.writePushPop(vmCommand::C_POP, parser.getArg(1), parser.getArg(2));
                break;

            case vmCommand::C_ARITHMETIC:
                std::cout 
                    << "Arithmetic Command: " << "Arg0: " 
                    << parser.getArg(0) << '\n';
                codeWriter.writeArithmetic(parser.getArg(0));
                break;

            default:
                std::cerr << "Invalid command\n";
                return 1;
        }
    }
    codeWriter.getNumOfCmdsWritten();
}
