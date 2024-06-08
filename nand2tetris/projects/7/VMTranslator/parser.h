#ifndef PARSER_H
#define PARSER_H

#include "utilities.h"
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <array>

class Parser{

private:
    std::unique_ptr<std::ifstream> inFile;
    std::string currentCommand{};
    std::string_view currentCommandView{};
    std::array<std::string_view, 3> commandFields;

public:
    Parser(std::unique_ptr<std::ifstream> inFile);
    ~Parser(){ inFile->close(); };
    
    auto advance() -> void;
    auto hasMoreCommands() -> bool;
    auto splitCommandToFields(char = ' ') -> void;
    auto commandType() -> vmCommand;
    [[nodiscard]]
    auto getArg(unsigned short) -> std::string_view;
};

#endif
