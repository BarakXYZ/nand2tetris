// Implemented by BarakXYZ 2024
// as part of the NAND2TETRIS course offered by Noam Nisan and Shimon Schocken

#include "Parser.h"
#include "CodeWriter.h"
#include <filesystem>

auto processEntry(Parser &parser, CodeWriter &codeWriter) -> bool;

auto main(int argc, char *argv[]) -> int {
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: " << argv[0] << HelpersVM::usage;
        return 1;
    }

    std::string inFileName{argv[1]};
    std::string outFileName;

    if (argc == 3) {  // The user has defined source and dest
        outFileName = argv[2];
    } else {  // User has defined only source (e.g. ./VMTranslator source.vm)
        outFileName = HelpersVM::replaceExtension(inFileName, ".asm");
    }

    if (!std::filesystem::exists(inFileName)) {
        std::cerr << "Path does not exist: " << inFileName << '\n';
        return 1;
    }

    // Construct CodeWriter:
    // if regular single file (e.g. ../../fileName.vm)
    //     should just replace the in file extension with .asm
    //     i.e. ../../fileName.vm  == ../../fileName.asm
    //     and set programName to fileName (for static vars)
    //     Can be achieved with one simple constructor
    //
    // else directory (e.g. ../../dirName/)
    //
    //     Do Once:
    //         add dirName.asm as suffix to the entire path (file is written to dir)
    //         i.e. ../../dirName/ == ../../dirName/dirName.asm
    //         constructor ^^
    //
    //     Per Entry:
    //         set programName to dirName (for static vars)
    //         function per Entry ^^

    // if (std::filesystem::is_directory(inFileName)) {
    //     std::cout << "File is a directory: " << inFileName << '\n';  // debug

    //     outFileName = HelpersVM::extractDirName(inFileName);
    //     std::cout  // debug
    //         << std::right << std::setw(4) << std::cout.fill(' ')
    //         << "Out file: " << outFileName << '\n';
        
    //     for (const auto &entry : std::filesystem::directory_iterator(inFileName)) {
    //         if (entry.path().extension() == ".vm") {
    //             std::string subFile = entry.path().string();
    //             std::cout  // debug
    //                 << std::right << std::setw(4) << std::cout.fill(' ')
    //                 << "File within directory: " << subFile << '\n';

    //             // Pass in file to Parser
    //             // Parse file
    //             // Write out file (outFileName)
    //         }
    //     }
    // } else if (std::filesystem::is_regular_file(inFileName) && std::filesystem::path(inFileName).extension() == ".vm") {
    //     std::cout << "Single file: " << inFileName << '\n';  // debug
    //     outFileName = HelpersVM::extractDirName(inFileName);
    //     std::cout << "Out file: " << outFileName << '\n';  // debug
    //     // processFile(inFileName, outFileName);
    // } else {
    //     std::cerr << "The path is neither a regular file nor a directory, or the file does not have a \".vm\" extension.\n";
    //     return 1;
    // }

    // return 0;  // END TEMP DEBUG

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
    // Parser parser{std::move(inFile)};
    Parser parser;
    parser.initNewEntry(inFileName);
    CodeWriter codeWriter{std::move(outFile), cleanProgramName(argv[1])};
    
    if(!processEntry(parser, codeWriter))
        return 1;

    // HelpersVM::getNumOfCmdsWritten();  // Debug
}

auto processEntry(Parser &parser, CodeWriter &codeWriter) -> bool {
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
                return false;
        }
    } return true;
}

