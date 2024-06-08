#include <iostream>
#include <array>
#include <string_view>

enum class vmCommand {
    C_PUSH,
    C_POP,
    C_ARITHMETIC
};

class Parser {
public:
    void splitCommandToFields(std::string_view command, char delimiter = ' ');
    vmCommand commandType();
    
    // For testing, make this public
    std::array<std::string_view, 3> commandFields;
};

void Parser::splitCommandToFields(std::string_view command, char delimiter) {
    size_t start = 0;
    size_t end = command.find(delimiter);
    size_t index = 0;  // Index for the current field in the array

    // Clear previous command fields
    commandFields = {};

    // Debug output to check the current command
    std::cout << "Current command: " << command << std::endl;

    // Loop until no more delimiters are found or the array is full
    while (end != std::string_view::npos && index < commandFields.size() - 1) {
        // Add the current field (substring) to the fields array
        commandFields[index++] = command.substr(start, end - start);
        start = end + 1;  // Move the start index to the next character after the delimiter
        end = command.find(delimiter, start);  // Find the next occurrence of the delimiter
    }

    // Add the last field (substring) to the fields array
    if (start < command.size() && index < commandFields.size()) {
        commandFields[index] = command.substr(start);
    }

    // Debug output to check the split results
    std::cout << "Split command fields:" << std::endl;
    for (const auto& field : commandFields) {
        if (!field.empty()) {
            std::cout << "Field: " << field << std::endl;
        } else {
            std::cout << "Empty Field" << std::endl;
        }
    }
}

vmCommand Parser::commandType() {
    std::cout << "commandFields[0] = " << commandFields[0] << '\n';
    if (commandFields[0] == "push") {
        return vmCommand::C_PUSH;
    }
    else if (commandFields[0] == "pop") {
        return vmCommand::C_POP;
    }
    else {
        return vmCommand::C_ARITHMETIC;
    }
}

int main() {
    Parser parser;

    // Test with "pop constant 5"
    std::string_view command1 = "pop constant 5";
    parser.splitCommandToFields(command1);
    vmCommand commandType1 = parser.commandType();
    std::cout << "Command type: " << static_cast<int>(commandType1) << '\n';

    // Test with "add"
    std::string_view command2 = "add";
    parser.splitCommandToFields(command2);
    vmCommand commandType2 = parser.commandType();
    std::cout << "Command type: " << static_cast<int>(commandType2) << '\n';

    return 0;
}

