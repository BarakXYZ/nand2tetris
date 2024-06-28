// Implemented by BarakXYZ 2024

#ifndef PUSH_POP_COMMANDS_PUSH_POP_TEMP_H
#define PUSH_POP_COMMANDS_PUSH_POP_TEMP_H
#include <string_view>

namespace PushPopCommands {

// addr = 5 + i, *SP = *addr, SP++
// D = *(temp + i), SP++, *(A--) = D
static constexpr std::string_view pushTemp =

    // 1. D = *(temp + 1)
        // @i
        "D=A"       "\n"
        "@R5"       "\n"
        "A=D+A"     "\n"
        "D=M"       "\n"

    // 2. SP++
        "@SP"       "\n"
        "AM=M+1"    "\n"

    // 3. *(A--) = D
        "A=A-1"     "\n"
        "M=D"       "\n"
    ;


// -------------------------------------------------------
// addr = 5 + i, SP--, *addr = *SP
// R13 = temp + i, D = *(SP--), *(temp+i) = D
static constexpr std::string_view popTemp =
    
    // 1. R13 = temp + i
        // @i
        "D=A"       "\n"
        "@R5"       "\n"
        "D=D+A"     "\n"
        "@R13"      "\n"
        "M=D"       "\n"

    // 2. D = *(SP--)
        "@SP"       "\n"
        "AM=M-1"    "\n"
        "D=M"       "\n"
    
    // 3. *(temp+i) = D
        "@R13"      "\n"
        "A=M"       "\n"
        "M=D"       "\n"
    ;

};

#endif
