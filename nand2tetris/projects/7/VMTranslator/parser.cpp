// Implemented by BarakXYZ 2024

#include "Parser.h"
#include <algorithm>
#include <cctype>

Parser::Parser(std::unique_ptr<std::ifstream> inFile) 
: inFile(std::move(inFile)) {};

// Check if reached the end of the file (eof)
auto Parser::hasMoreCommands() -> bool {
    return inFile->peek() != std::ifstream::traits_type::eof();
}

auto Parser::isCommentLine() -> bool {
    return currentCommand.front() == '/';
}

// Advance to the next command, skip comment lines and assign String View
auto Parser::advance() -> void {
    std::getline(*inFile >> std::ws, currentCommand);
    currentCommandView = currentCommand;  // Set viewer
}

auto Parser::trim(std::string_view str) -> std::string_view {
    const auto start = std::find_if_not(str.begin(), str.end(), [](const char c) { return std::isspace(c); });
    const auto end = std::find_if_not(str.rbegin(), str.rend(), [](const char c) { return std::isspace(c); }).base();
    return (start < end) ? std::string_view(&*start, end - start) : std::string_view{};
}

auto Parser::splitCommandToFields(char delimiter) -> void {
    size_t start{0}, end{0};
    unsigned short index{0}, maxFields{3};

    // Loop until no more delimiters are found or we exceed MAX_FIELDS
    while (index < maxFields && (end = currentCommandView.find(delimiter, start)) != std::string_view::npos) {
        if (end != start) {
            commandFields[index++] = trim(currentCommandView.substr(start, end - start));
        }
        start = end + 1;
    }

    // Handle the last field
    if (index < maxFields && start < currentCommandView.size()) {
        commandFields[index] = trim(currentCommandView.substr(start));
    }
}

auto Parser::commandType() -> HelpersVM::vmCommand {
    // std::cout << "commandFields[0] = " << commandFields[0] << '\n';  // Debug
    if(commandFields[0] == "push") {
        return HelpersVM::vmCommand::C_PUSH;
    }
    else if(commandFields[0] == "pop")
        return HelpersVM::vmCommand::C_POP;
    else
        return HelpersVM::vmCommand::C_ARITHMETIC;
}

auto Parser::getArg(unsigned short argIndex) -> std::string_view {
    return commandFields[argIndex];
}
