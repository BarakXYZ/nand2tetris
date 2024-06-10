// Implemented by BarakXYZ 2024

#ifndef LOGICAL_COMMANDS_AND_OR_NOT_H
#define LOGICAL_COMMANDS_AND_OR_NOT_H
#include <string_view>

namespace ArithmeticLogicalCommands {

    // -------------------------------------------------------
    // 1. D = *(SP--)
    // 2. D & *(A--)
    constexpr std::string_view logicalAnd =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. D & *(A--)
            "A=A-1"     "\n"
            "M=D&M"     "\n"
        ;


    // -------------------------------------------------------
    // 1. D = *(SP--)
    // 2. D & *(A--)
    constexpr std::string_view logicalOr =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. D & *(A--)
            "A=A-1"     "\n"
            "M=D|M"     "\n"
        ;


    // -------------------------------------------------------
    // 1. *SP = !(*SP)
    constexpr std::string_view logicalNot =

        // 1. *SP = !(*SP)
            "@SP"       "\n"
            "A=M-1"     "\n"
            "M=!M"      "\n"
        ;

};

#endif
