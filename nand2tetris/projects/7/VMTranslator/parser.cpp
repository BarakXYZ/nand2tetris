#include "parser.h"


Parser::Parser(std::unique_ptr<std::ifstream> inFile): inFile(std::move(inFile)) {};

auto Parser::advance() -> void {
    std::getline(*inFile >> std::ws, currentCommand);
}

auto Parser::hasMoreCommands() -> bool {
    // Method A: Using peek() and eof()
    return inFile->peek() == std::ifstream::traits_type::eof();

}
