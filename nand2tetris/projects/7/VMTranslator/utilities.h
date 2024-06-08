#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <charconv>
#include <iostream>

enum class vmCommand {
    C_ARITHMETIC,
    C_PUSH, C_POP,
    C_LABEL, C_GOTO,
    C_IF, C_FUNCTION,
    C_RETURN, C_CALL,
};

int stringViewToInt(std::string_view str);

#endif
