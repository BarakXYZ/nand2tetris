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
        // outFileName = HelpersVM::replaceExtension(inFileNameView, ".asm");
        outFileName = argv[1];
    }

    if (!std::filesystem::exists(inFileName)) {
        std::cerr << "Path does not exist: " << inFileName << '\n';
        return 1;
    }

    if (std::filesystem::is_directory(inFileName)) {
        std::cout << "File is a directory: " << inFileName << '\n';  // debug
        // outFileName = HelpersVM::extractDirName(inFileName);  // src/dir/ == dir.asm
        outFileName = HelpersVM::makeDirOutFileName(inFileName);
        std::cout  // debug
            << std::right << std::setw(4) << std::cout.fill(' ')
            << "Out file: " << outFileName << '\n';

        Parser parser;
        CodeWriter codeWriter{outFileName};
        codeWriter.writeInit();

        
        for (const auto &entry : std::filesystem::directory_iterator(inFileName)) {
            if (entry.path().extension() == ".vm") {
                std::string newFileVM = entry.path().string();
                std::cout  // debug
                    << std::right << std::setw(4) << std::cout.fill(' ')
                    << "File within directory: " << newFileVM << '\n';

                // Raw path (e.g. /path/folder/Main.vm)
                parser.resetCurrentEntry();
                parser.initNewFileVM(newFileVM);

                // For the code writer, we want a clean cut of the name
                // e.g. /path/folder/Main.vm == Main
                // This is a different name from the outFileName
                // (which was set before and does not change).
                // This is for the static vars and other symbols
                codeWriter.setFileName(HelpersVM::cleanProgramName(newFileVM));

                if(!processEntry(parser, codeWriter))
                    return 1;
            }
        }
    } else if (std::filesystem::is_regular_file(inFileName) && std::filesystem::path(inFileName).extension() == ".vm") {
        std::cout << "Single file: " << inFileName << '\n';  // debug
        outFileName = HelpersVM::replaceExtension(inFileNameView, ".asm");
        std::cout << "Out file: " << outFileName << '\n';  // debug

        // Construct Parser & CodeWriter
        Parser parser;
        parser.initNewFileVM(inFileName);
        CodeWriter codeWriter{outFileName, cleanProgramName(argv[1])};
        
        if(!processEntry(parser, codeWriter))
            return 1;

    }
    else {
        std::cerr << "The path is neither a regular file nor a directory, or the file does not have a \".vm\" extension.\n";
        return 1;
    }

    HelpersVM::getNumOfCmdsWritten();  // Debug
}  // return 0;

auto processEntry(Parser &parser, CodeWriter &codeWriter) -> bool {
        // Write booting
        // codeWriter.writeInit();  // Call when we're reading a directory?
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

                case vmCommand::C_LABEL:
                    codeWriter.writeLabel(parser.getArg(1));
                    break;

                case vmCommand::C_GOTO:
                    codeWriter.writeGoto(parser.getArg(1));
                    break;

                case vmCommand::C_IF:
                    codeWriter.writeIf(parser.getArg(1));
                    break;

                case vmCommand::C_FUNCTION:
                    codeWriter.writeFunction(parser.getArg(1), HelpersVM::strViewToInt(parser.getArg(2)));
                    break;

                case vmCommand::C_CALL:
                    codeWriter.writeCall(parser.getArg(1), HelpersVM::strViewToInt(parser.getArg(2)));
                    break;

                case vmCommand::C_RETURN:
                    codeWriter.writeReturn();
                    break;

                default:
                    std::cerr << "Invalid command\n";
                    return false;
            }
        } return true;
}

