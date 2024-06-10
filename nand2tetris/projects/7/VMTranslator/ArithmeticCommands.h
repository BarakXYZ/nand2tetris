// Implemented by BarakXYZ 2024

#ifndef ARITHMETIC_COMMANDS_LT_H
#define ARITHMETIC_COMMANDS_LT_H
#include <string_view>

namespace ArithmeticLogicalCommands {

    // add | D = *(SP--), A--, *A += D
    static constexpr std::string_view arithmeticAdd =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. A--, *A += D
            "A=A-1"     "\n"
            "M=M+D"     "\n"
        ;


    // -------------------------------------------------------
    // sub | D = *(SP--), A--, *A -= D
    static constexpr std::string_view arithmeticSub =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. A--, *A -= D
            "A=A-1"     "\n"
            "M=M-D"     "\n"
        ;


    // -------------------------------------------------------
    // neg | *((SP)--) = -(*((SP)--))
    static constexpr std::string_view arithmeticNeg =

        // 1. *((SP)--) = -(*((SP)--))
            "@SP"       "\n"
            "A=M-1"     "\n"
            "M=-M"      "\n"
        ;
};

#endif
