#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

class SymbolTable {
  public:
    // Add a symbol to the table with its associated address
    void add_symbol(const std::string &symbol, int address)
    {
        table[symbol] = address;
    }

    // Retrieve the address associated with a symbol
    // Returns -1 if the symbol is not found
    int get_address(const std::string &symbol) const
    {
        auto it = table.find(symbol);
        if (it != table.end()) {
            return it->second;
        }
        else {
            return -1;
        }
    }

    // Check if a symbol is in the table
    bool contains(const std::string &symbol) const
    {
        return table.find(symbol) != table.end();
    }

  private:
    std::unordered_map<std::string, int> table{
        // Initializtion: Pre-Defined Symbols
        {"R0", 0},         {"R1", 1},      {"R2", 2},   {"R3", 3},   {"R4", 4},   {"R5", 5},   {"R6", 6},   {"R7", 7},
        {"R8", 8},         {"R9", 9},      {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
        {"SCREEN", 16384}, {"KBD", 24576}, {"SP", 0},   {"LCL", 1},  {"ARG", 2},  {"THIS", 3}, {"THAT", 4}};
};

class Parser {
    // First-Pass; if instruction,
    // unpacks each instruction into its underlying fields
    // Should be able to ignore white spaces (i.e. comments, multi comments?, spaces)
    // Examples:
    // input: D=A
    // output: dest = D, comp = A, jump = null
    //
    // input: AM=M-1
    // output: dest = AM, comp = M-1, jump = null
    //
    // input: 0;JMP
    // output: dest = null, comp = 0, jump = JMP
  private:
    // fields[0] = dest, fields[1] = comp, fields[2] = jump
    std::array<std::string, 3> fields;

  public:
    void parse_file(std::ifstream &file)
    {
        // First-Pass: Add the label symbols (i.e. (xxx))
        size_t line_number{0};
        std::string line;
        // alt
        while (std::getline(file, line)) {
            for (char c : line) {
                if (c == '(')
                    std::cout << line_number << ": " << line << std::endl;
            }
            // std::cout << line;
            /* if (!file.eof()) */
            /*     std::cout << std::endl; */
            ++line_number;
        }
        // Second-Pass: Add the var. symbols
    }
};


int main()
{
    std::ifstream file;
    std::string file_name{"test_files/Max.asm"};

    file.open(file_name);
    if (!file)
        exit(1);

    Parser parser;
    parser.parse_file(file);

    file.close();
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
