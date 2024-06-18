// Implemented by BarakXYZ 2024

#ifndef PARSER_H
#define PARSER_H

#include "HelpersVM.h"
#include "CodeWriter.h"
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <array>

class Parser{

private:
    std::unique_ptr<std::ifstream> inFile;
    // std::ifstream inFile;
    std::string currentCommand{};
    std::string_view currentCommandView{};
    std::array<std::string_view, 3> commandFields;

public:
    Parser() = default;
    Parser(std::unique_ptr<std::ifstream> inFile);
    ~Parser(){ inFile->close(); };
    
    auto initEntry(const std::string &fileName) -> bool;
    auto resetEntry() -> void;
    auto isCommentLine() -> bool;
    auto advance() -> void;
    auto hasMoreCommands() -> bool;
    auto splitCommandToFields(char = ' ') -> void;
        auto trim(std::string_view) -> std::string_view;
    auto commandType() -> HelpersVM::vmCommand;
    [[nodiscard]]
    auto getArg(unsigned short) -> std::string_view;
    // pt2
    // auto setInFile(std::ifstream)
};

#endif
