// Implemented by BarakXYZ 2024
// as part of the NAND2TETRIS course offered by Noam Nisan and Shimon Schocken

#include "Parser.h"
#include "CodeWriter.h"

auto main(int argc, char *argv[]) -> int {
    constexpr std::string_view usage{" <input_file.vm> [output_file.asm]\n"};
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: " << argv[0] << usage;
        return 1;
    }

    const std::string inFileName{argv[1]};
    std::string outFileName;

    if (argc == 3) {
        outFileName = argv[2];
    } else {
        outFileName = HelpersVM::replaceExtension(inFileName, ".asm");
    }


    std::unique_ptr<std::ifstream> inFile{
        std::make_unique<std::ifstream>(inFileName)};
    if (!inFile) {
        std::cerr << "Error opening input file: " << inFileName << std::endl;
        return 1;
    }

    if (inFile->peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "File is empty.\n";
        return 1;
    }

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
        incrementNumOfCmds();
        parser.splitCommandToFields();
        switch(parser.commandType()) {
            case vmCommand::C_PUSH:
                codeWriter.writePushPop(vmCommand::C_PUSH, parser.getArg(1), parser.getArg(2));
                break;

            case vmCommand::C_POP:
                codeWriter.writePushPop(vmCommand::C_POP, parser.getArg(1), parser.getArg(2));
                break;

            case vmCommand::C_ARITHMETIC:
                codeWriter.writeArithmetic(parser.getArg(0));
                break;

            default:
                std::cerr << "Invalid command\n";
                return 1;
        }
    }

    // HelpersVM::getNumOfCmdsWritten();  // Debug
}
