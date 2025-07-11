// Implemented by BarakXYZ 2024

#ifndef PUSH_POP_COMMANDS_PUSH_POP_STATIC_H
#define PUSH_POP_COMMANDS_PUSH_POP_STATIC_H
#include <string_view>

namespace PushPopCommands {

    // push static i
    // *SP = fileName.i, SP++
    // D = fileName.i, SP++, A--, *A = D
    static constexpr std::string_view pushStatic =

        // 1. D = fileName.i  (implemented in CodeWriter)
            // @fileName.i
            "D=M"       "\n"
        
        // 2. SP++, A--
            "@SP"       "\n"
            "AM=M+1"    "\n"
            "A=A-1"     "\n"

        // 3. *A = D
            "M=D"       "\n"
        ;


    // -------------------------------------------------------
    // SP--, *fileName.i = *SP
    // D = *(SP--), *fileName.i = *SP
    static constexpr std::string_view popStatic =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. *fileName.i = *SP (implemented in CodeWriter)
            // *fileName.i = *SP
            // @fileName.i
            // M=D
        ;

};

#endif
