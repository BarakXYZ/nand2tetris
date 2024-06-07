#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <memory>
#include <string>

class Parser{

public:
    std::unique_ptr<std::ifstream> inFile;
    std::string currentCommand{};
    Parser(std::unique_ptr<std::ifstream> inFile);

    auto advance() -> void;

    auto hasMoreCommands() -> bool;

};

#endif
