// Implemented by BarakXYZ 2024
// as part of the NAND2TETRIS course offered by Noam Nisan and Shimon Schocken

#include "Parser.h"
#include "CodeWriter.h"
#include <filesystem>

auto main(int argc, char *argv[]) -> int {
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: " << argv[0] << HelpersVM::usage;
        return 1;
    }

    const std::string inFileName{argv[1]};
    std::string outFileName;

    if (argc == 3) {  // User define source and dest
        outFileName = argv[2];
    } else {  // User define only source (auto dest)
        outFileName = HelpersVM::replaceExtension(inFileName, ".asm");
    }

    if (!std::filesystem::exists(inFileName)) {
        std::cerr << "Path does not exist: " << inFileName << '\n';
        return 1;
    }

    if (std::filesystem::is_directory(inFileName)) {
        std::cout << "File is a directory: " << inFileName << '\n';  // debug
        std::string outFile = HelpersVM::extractDirName(inFileName);
        std::cout  // debug
            << std::right << std::setw(4) << std::cout.fill(' ')
            << "Out file: " << outFile << '\n';

        for (const auto &entry : std::filesystem::directory_iterator(inFileName)) {
            if (std::filesystem::is_regular_file(entry.path()) && entry.path().extension() == ".vm") {
                std::string inFile = entry.path().string();
                std::cout  // debug
                    << std::right << std::setw(4) << std::cout.fill(' ')
                    << "File within directory: " << inFile << '\n';

                // processFile(inFile, outFile);
            }
        }
    } else if (std::filesystem::is_regular_file(inFileName) && std::filesystem::path(inFileName).extension() == ".vm") {
        std::cout << "Is file (single): " << inFileName << '\n';  // debug
        std::string outFile = HelpersVM::extractDirName(inFileName);
        std::cout << "Out file: " << outFile << '\n';  // debug
        // processFile(inFileName, outFileName);
    } else {
        std::cerr << "The path is neither a regular file nor a directory, or the file does not have a .vm extension.\n";
        return 1;
    }

    return 0;  // END TEMP DEBUG

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
