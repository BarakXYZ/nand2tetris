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
    std::string_view inFileNameView{argv[1]};
    std::string outFileName;

    if (argc == 3) {  // User has defined source and dest (e.g. source.vm out.asm)
        outFileName = argv[2];
    }
    else {  // User has defined only source (e.g. ./VMTranslator source.vm)
        outFileName = HelpersVM::replaceExtension(inFileNameView, ".asm");
    }

    if (!std::filesystem::exists(inFileName)) {
        std::cerr << "Path does not exist: " << inFileName << '\n';
        return 1;
    }

    // Psuedo Code:
    // Construct CodeWriter:
    // if regular single file (e.g. ../../fileName.vm)
    //
    //     1. replace the in file extension with .asm
    //         i.e. ../../fileName.vm  == ../../fileName.asm
    //     2. set fileName to fileName (for static vars)
    //     Both steps to be executed by the CodeWriter's constructor
    //
    // else directory (e.g. ../../dirName/)
    //
    //     Do Once:
    //         1. Open file to write, add dirName.asm as suffix to the entire path
    //             (i.e. ../dirName/ == ../dirName/dirName.asm)
    //         To be executed via the constructor constructor
    //
    //     Per Entry:
    //         set fileName to dirName (for static vars)
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

    // Construct Parser & CodeWriter
    Parser parser;
    parser.initNewEntry(inFileName);
    CodeWriter codeWriter{outFileName, cleanProgramName(argv[1])};
    
    if(!processEntry(parser, codeWriter))
        return 1;

    // HelpersVM::getNumOfCmdsWritten();  // Debug
}  // return 0;

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

