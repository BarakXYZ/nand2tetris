// Implemented by BarakXYZ 2024
#include "SymbolTable.h"

#include <iostream>

namespace SymbolTable {
    using sTableAlias = std::unordered_map<std::string, std::string>;

    void add_symbol(const std::string symbol, size_t address)
    {
        symbol_table[symbol] = address;
    }

    std::size_t unique_symbols_counter{16};

    std::unordered_map<std::string, std::size_t> symbol_table = {
        {"R0", 0},         {"R1", 1},      {"R2", 2},   {"R3", 3},   {"R4", 4},   {"R5", 5},   {"R6", 6},   {"R7", 7},
        {"R8", 8},         {"R9", 9},      {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
        {"SCREEN", 16384}, {"KBD", 24576}, {"SP", 0},   {"LCL", 1},  {"ARG", 2},  {"THIS", 3}, {"THAT", 4}
    };

    size_t parse_variable(const std::string &symbol)
    {
        auto it = symbol_table.find(symbol);
        if (it == symbol_table.end()) {
            std::cout << "Parsed unrecognized variable '" << symbol
                      << "' assigning & returning address: " << SymbolTable::unique_symbols_counter << '\n';
            add_symbol(symbol, SymbolTable::unique_symbols_counter);
            ++SymbolTable::unique_symbols_counter;
            return symbol_table.find(symbol)->second;
        }
        else {
            std::cout << "Parsed recognized variable '" << symbol << "', returning address: " << it->second << '\n';
            return it->second;
        }
    }

    const sTableAlias comp_table = {
        {"0", "0101010"}, {"1", "0111111"}, {"-1", "0111010"},
        {"D", "0001100"}, {"A", "0110000"}, {"M", "1110000"},
        {"!D", "0001101"}, {"!A", "0110001"}, {"!M", "1110001"},
        {"-D", "0001111"}, {"-A", "0110011"}, {"-M", "1110011"},
        {"D+1", "0011111"}, {"1+D", "0011111"},  // Can be written both ways
        {"A+1", "0110111"}, {"1+A", "0110111"},  // Can be written both ways
        {"M+1", "1110111"}, {"1+M", "1110111"},  // Can be written both ways
        {"D-1", "0001110"}, {"A-1", "0110010"}, {"M-1", "1110010"},
        {"D+A", "0000010"}, {"A+D", "0000010"},  // Can be written both ways
        {"D+M", "1000010"}, {"M+D", "1000010"},  // Can be written both ways
        {"D-A", "0010011"},
        {"D-M", "1010011"}, {"A-D", "0000111"}, {"M-D", "1000111"},
        {"D&A", "0000000"}, {"A&D", "0000000"},  // Can be written both ways
        {"D&M", "1000000"}, {"M&D", "1000000"},  // Can be written both ways
        {"D|A", "0010101"}, {"A|D", "0010101"},  // Can be written both ways
        {"D|M", "1010101"}, {"M|D", "0010101"},  // Can be written both ways
    };

    const sTableAlias dest_table = {
        {"null", "000"}, {"M", "001"}, {"D", "010"}, {"MD", "011"},
        {"A", "100"}, {"AM", "101"}, {"AD", "110"}, {"AMD", "111"}
    };

    const sTableAlias jump_table = {
        {"null", "000"}, {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"},
        {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}
    };

    std::optional<std::string> lookup(const sTableAlias& table, const std::string& key) {
            auto it = table.find(key);
            if (it != table.end()) {
                return it->second;
            }
            return std::nullopt;
        }
}
