// Implemented by BarakXYZ 2024
#ifndef PARSER_H
#define PARSER_H

#include <string>

namespace Parser {

extern void remove_whitespace(std::string &line);
extern std::string decimal_to_binary(int n);

// First-Pass: Add the label symbols (i.e. (xxx))
extern bool parse_first_pass(std::ifstream &file);

// Second Pass: Resolve all label symbols, variables and C-instructions.
extern bool parse_second_pass(std::ifstream &file, std::stringstream &parsed_file);

}  // namespace Parser
#endif
