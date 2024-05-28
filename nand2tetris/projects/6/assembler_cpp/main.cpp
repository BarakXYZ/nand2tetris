#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <bitset>

class SymbolTable {
  public:
    // Add a symbol to the table with its associated address
    static void add_symbol(const std::string symbol, int address)
    {
        table[symbol] = address;
    }

    // Retrieve the address associated with a symbol
    // Returns -1 if the symbol is not found
    static int get_address(const std::string &symbol)
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
    static bool contains(const std::string &symbol)
    {
        return table.find(symbol) != table.end();
    }

    static std::unordered_map<std::string, int> table;
};

std::unordered_map<std::string, int> SymbolTable::table = {
    {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7},
    {"R8", 8}, {"R9", 9}, {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15},
    {"SCREEN", 16384}, {"KBD", 24576}, {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4}
};

class Parser {
  public:
    void parse_file(std::ifstream &file)
    {
        size_t line_number{1};
        std::string line;
        // alt
        std::string::size_type start{0};
        
        std::size_t var_count{16};

        // First-Pass: Add the label symbols (i.e. (xxx))
        while (std::getline(file >> std::ws, line)) {
            if (line.front() == '(' && line.at(line.size() - 2) == ')') {
                std::cout << line_number << ": " << line << '\n';

                // Extract label's content
                std::string content = line.substr(1, line.size() - 3);
                std::cout << "Label Content: " << content << '\n';
                SymbolTable::add_symbol(content, line_number);
            }
            ++line_number;
        }

        line_number = 1;
        // Second-Pass: Add the var. symbols
        while (std::getline(file >> std::ws, line)) {
            if (line.front() == '(' && line.at(line.size() - 2) == ')') {
                std::cout << line_number << ": " << line << '\n';

                // Extract label's content
                std::string content = line.substr(1, line.size() - 3);
                std::cout << "Label Content: " << content << '\n';
                SymbolTable::add_symbol(content, line_number);
            }
            ++line_number;
        }

    }
};

class Code {
public:
    static std::string decimal_to_binary(int n) {
        return std::bitset<16>(n).to_string();
    }
};

int main() {
    std::ifstream infile;
    std::string infile_name{"test_files/Max.asm"};

    infile.open(infile_name);
    if (!infile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    Parser parser;
    parser.parse_file(infile);

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
        outfile << column.first << ' ' << Code::decimal_to_binary(column.second) << '\n';
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
