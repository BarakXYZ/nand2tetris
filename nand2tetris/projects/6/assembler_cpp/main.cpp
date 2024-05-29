#include <array>
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class SymbolTable {
  public:
    // Add a symbol to the table with its associated address
    static void add_symbol(const std::string symbol, size_t address)
    {
        table[symbol] = address;
    }

    // Retrieve the address associated with a symbol
    // Adds the symbol and return an address if it wasn't found
    static size_t unique_symbols_counter;
    static size_t parse_variable(const std::string &symbol)
    {
        auto it = table.find(symbol);
        if (it == table.end()) {
            std::cout << "Parsed unrecognized variable '" << symbol
                      << "' assigning & returning address: " << SymbolTable::unique_symbols_counter << '\n';
            add_symbol(symbol, SymbolTable::unique_symbols_counter);
            ++SymbolTable::unique_symbols_counter;
            return table.find(symbol)->second;
        }
        else {
            std::cout << "Parsed recognized variable '" << symbol << "', returning address: " << it->second << '\n';
            return it->second;
        }
    }

    // Check if a symbol is in the table
    static bool contains(const std::string &symbol)
    {
        return table.find(symbol) != table.end();
    }

    static std::unordered_map<std::string, size_t> table;
};

std::size_t SymbolTable::unique_symbols_counter{16};
std::unordered_map<std::string, size_t> SymbolTable::table = {
    {"R0", 0},         {"R1", 1},      {"R2", 2},   {"R3", 3},   {"R4", 4},   {"R5", 5},   {"R6", 6},   {"R7", 7},
    {"R8", 8},         {"R9", 9},      {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
    {"SCREEN", 16384}, {"KBD", 24576}, {"SP", 0},   {"LCL", 1},  {"ARG", 2},  {"THIS", 3}, {"THAT", 4}};

class Parser {
  public:
    // First-Pass: Add the label symbols (i.e. (xxx))
    static bool parse_first_pass(std::ifstream &file)
    {

        size_t line_number{0};
        std::string line;

        while (std::getline(file >> std::ws, line)) {
            // Potentially we can maybe optimize with deleting those comment lines for the second pass
            if (line.front() == '/') {
                std::cout << "Comment line\n";
                continue;
            }

            if (line.front() == '(') {
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
    static bool parse_second_pass(std::ifstream &file, std::stringstream &parsed_file)
    {

        size_t line_number{0};
        std::string line{};

        while (std::getline(file >> std::ws, line)) {
            if (line.front() == '/') {
                std::cout << "Comment line\n";
                continue;
            }
            // std::cout << line << '\n';

            // if A-instruction
            // if Address (convert to binary)
            // if Variable (look it up, if it exists, assign the address, if not, allocate new address starting with 16)
            if (line.front() == '@') {
                // std::cout << "A-instruction: " << line << '\n';
                line = line.erase(0, 1);
                if (line.find('\n')) {
                    line.pop_back();
                    // std::cout << "End line removed\n";
                }
                size_t variable_address{SymbolTable::parse_variable(line)};
                parsed_file << variable_address << '\n';
            }
            else if (line.front() == '(')
                continue;

            // if C-instruction (convert to the symbolic underlying fields)
            else {
                std::cout << "C-instruction: " << line << '\n';
            }
            ++line_number;
        }
        return true;
    }
};

class Code {
  public:
    static std::string decimal_to_binary(int n)
    {
        return std::bitset<16>(n).to_string();
    }
};

int main()
{
    std::ifstream infile;
    std::string infile_name{"test_files/Max.asm"};

    infile.open(infile_name);
    if (!infile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    if (!Parser::parse_first_pass(infile)) {
        std::cerr << "First pass parsing failed." << std::endl;
        return 1;
    }

    std::stringstream parsed_file{};
    if (!Parser::parse_second_pass(infile, parsed_file)) {
        std::cerr << "Second pass parsing failed." << std::endl;
        return 1;
    }

    // Close the input file after parsing
    infile.close();

    // Open the output file
    std::ofstream outfile;
    std::string outfile_name{"output.txt"};

    outfile.open(outfile_name);
    if (!outfile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    // Write the contents of SymbolTable::table to the output file
    for (const auto &column : SymbolTable::table) {
        // outfile << column.first << ' ' << Code::decimal_to_binary(column.second) << '\n';
        outfile << column.first << ' ' << column.second << '\n';
    }

    // Close the output file
    outfile.close();

    return 0;
}

/*
@256
D=A
@SP
M=D
@133
0;JMP
@R15
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=0
@END_EQ
D;JNE
@SP
A=M-1
M=-1
(END_EQ)
@R15
A=M
0;JMP
@R15
M=D
@SP

*/
