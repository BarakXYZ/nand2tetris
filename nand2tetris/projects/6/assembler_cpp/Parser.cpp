// Implemented by BarakXYZ 2024
#include "Parser.h"
#include "SymbolTable.h"

#include <algorithm>
#include <bitset>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Parser {

void remove_whitespace(std::string &line) {
    line.erase(std::remove_if(line.begin(), line.end(), [](const char c) { return std::isspace(c); }), line.end());
}

std::string decimal_to_binary(int n) {
    return std::bitset<16>(n).to_string();
}

// First-Pass: Add the label symbols (i.e. (xxx))
bool parse_first_pass(std::ifstream &file) {

    size_t line_number{0};
    std::string line;

    while (std::getline(file >> std::ws, line)) {
        // Potentially we can maybe optimize with deleting those comment lines for the second pass
        if (line.front() == '/') {
            std::cout << "Comment line\n";
            continue;
        }

        if (line.front() == '(') {
            // line.erase(std::remove_if(line.begin(), line.end(), [](const char c){
            //     return std::isspace(c);
            // }), line.end());
            auto end_label_pos = line.find(')');
            std::cout << line_number << ": " << line << '\n';

            // Extract label's content
            std::string content = line.substr(1, end_label_pos - 1);
            std::cout << "Label Content: " << content << '\n';
            SymbolTable::add_symbol(content, line_number);
            continue;
        }
        ++line_number;
    }
    file.clear();
    file.seekg(0, std::ios::beg);
    if (!file) {
        std::cerr << "Failed to reset file position after first pass." << std::endl;
        return false;
    }
    return true;
}

// Second Pass: Resolve all label symbols, variables and C-instructions.
bool parse_second_pass(std::ifstream &file, std::stringstream &parsed_file)
{
    size_t line_number{0};
    std::string line{};

    while (std::getline(file >> std::ws, line)) {
        // std::cout << line << '\n';  // Debug
        if (line.front() == '/') {
            // std::cout << "Comment line\n";  // Debug
            continue;
        }

        remove_whitespace(line);

        /* if (A-instruction):
               if Address (convert to binary)
               if Variable (look it up, if it exists, assign the address,
               if not, allocate new address starting with 16) */
        if (line.front() == '@') {
            // std::cout << "A-instruction: " << line << '\n';  // Debug
            line = line.erase(0, 1);  // Erase '@' from the beginning

            std::cout << "ATOI: " << atoi(line.c_str()) << '\n';
            if (std::ranges::all_of(line, [](const char c) { return std::isdigit(c); })) {
                std::cout << "Found ATOI digit\n";
                parsed_file << decimal_to_binary(atoi(line.c_str())) << '\n';
            }
            else {
                size_t variable_address{SymbolTable::parse_variable(line)};
                parsed_file << decimal_to_binary(variable_address) << '\n';  // Production
                // parsed_file << Code::decimal_to_binary(variable_address) << ' ' << line <<'\n';  // Debug
            }
            ++line_number;
            continue;
        }

        // if Label Declaration -> Skip
        if (line.front() == '(') {
            ++line_number;
            continue;
        }

        // if C-instruction (convert to the symbolic underlying fields)
        auto equal_pos{line.find('=')};
        if (equal_pos != std::string::npos) {
            std::cout << "C-instruction (dest + comp): " << line << '\n';
            parsed_file << "111";  // Always start with "111"

            // Computation ('comp')
            std::string comp_symbol{line.substr(equal_pos + 1, line.length())};
            std::cout << "Comp: " << comp_symbol << '\n';
            auto comp_binary{SymbolTable::lookup(SymbolTable::comp_table, comp_symbol)};
            if (comp_binary != std::nullopt)
                parsed_file << *comp_binary;
            else {
                std::cout << "Invalid comp symbol: " << " at line: " << line_number << '\n';
                return false;
            }

            // Destination ('dest')
            std::string dest_symbol{line.substr(0, equal_pos)};
            std::cout << "Dest: " << dest_symbol << '\n';
            std::optional<std::string> dest_binary{SymbolTable::lookup(SymbolTable::dest_table, dest_symbol)};
            if (dest_binary != std::nullopt)
                parsed_file << *dest_binary;
            else {
                std::cout << "Invalid dest symbol: " << " at line: " << line_number << '\n';
                return false;
            }

            // Jump Condition ('jump') -> always null in this case
            std::string jump{"null"};
            std::cout << "Jump: " << jump << '\n';
            parsed_file << *SymbolTable::lookup(SymbolTable::jump_table, jump);
            // parsed_file << ' ' << line << '\n';  // Debug
            parsed_file << '\n';  // Production
            ++line_number;
            continue;
        }

        std::size_t semicolon_pos = line.find(';');
        if (semicolon_pos != std::string::npos) {
            parsed_file << "111";
            std::cout << "C-instruction (jump): " << line << '\n';

            // Erase all non-alpha characters from the string
            // std::ranges::remove_if(line, [](const char c){
            //     return !std::isalpha(c);
            // });

            std::string comp_symbol{line.substr(0, semicolon_pos)};
            std::string comp_binary = *SymbolTable::lookup(SymbolTable::comp_table, comp_symbol);
            line.erase(0, semicolon_pos + 1);

            std::string dest_binary{"000"};  // Always null when jump

            line.erase(std::remove_if(line.begin(), line.end(), [](const char c) { return !std::isalpha(c); }),
                       line.end());

            std::string jump_symbol{line};  // Isolate the jump code
            // std::string jump_symbol{line.substr(semicolon_pos + 1, line.length() - 2)};  // Isolate the jump code
            std::optional<std::string> jump_binary{SymbolTable::lookup(SymbolTable::jump_table, jump_symbol)};
            if (jump_binary != std::nullopt) {
                parsed_file << comp_binary << dest_binary << *jump_binary << '\n';
                // parsed_file << comp << dest << " Jump: " << jump << '\n';  // Debug 1
                // parsed_file << comp << dest << jump << ' ' << line << '\n';  // Debug 2
                ++line_number;
                continue;
            }
            else {
                std::cout << "Invalid jump symbol: " << jump_symbol << " at line: " << line_number << '\n';
                return false;
            }
        }
    }
    return true;  // Post-Loop
}
}  // namespace Parser
