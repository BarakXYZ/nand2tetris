#include "parser.h"

Parser::Parser(std::unique_ptr<std::ifstream> inFile) 
: inFile(std::move(inFile)) {};

// Check if reached to the end of the file (eof)
auto Parser::hasMoreCommands() -> bool {
    return inFile->peek() != std::ifstream::traits_type::eof();
}

// Advance to the next command, skip comment lines and assign String View
auto Parser::advance() -> void {
    std::getline(*inFile >> std::ws, currentCommand);
    // Skip comment lines
    if(currentCommand.front() == '/')
        Parser::advance();

    currentCommandView = currentCommand;
}

void Parser::splitCommandToFields(char delimiter) {
    size_t start = 0;
    size_t end = currentCommandView.find(delimiter);
    unsigned short index = 0;  // Field index

    /* Loop until no more delimiters are found
    if needed, we can check if 'index' is bigger than the array.size(),
    which indicates something is wrong;
    But for now we're assuming the VM Code is valid and will 
    always contain at least (and no more than) 1-3 fields. */
    while (true) {
        commandFields[index++] = currentCommandView.substr(start, end - start);
        start = end + 1;  // Offset 'start' to the beginning of the next field
        end = currentCommandView.find(delimiter, start);  // Check for another field
        if (end == std::string_view::npos) {  // Check if reached end of the string
            commandFields[index] = currentCommandView.substr(start);
            break;
        }
    }
}

auto Parser::commandType() -> vmCommand {
    // std::cout << "commandFields[0] = " << commandFields[0] << '\n';  // Debug
    if(commandFields[0] == "push") {
        return vmCommand::C_PUSH;
    }
    else if(commandFields[0] == "pop")
        return vmCommand::C_POP;
    else
        return vmCommand::C_ARITHMETIC;
}

auto Parser::getArg(unsigned short argIndex) -> std::string_view {
    return commandFields[argIndex];
}
