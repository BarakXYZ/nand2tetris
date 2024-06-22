// Implemented by BarakXYZ 2024

#ifndef PUSH_POP_COMMANDS_PUSH_CONSTANT_H
#define PUSH_POP_COMMANDS_PUSH_CONSTANT_H
#include <string_view>

namespace PushPopCommands {

    // push constant i
    // *SP = i, SP++
    // D = @i, SP++, A--, *A = D
static constexpr std::string_view pushConstant {

    // 1. D = @i
        // @i
        "D=A"       "\n"

    // 2. SP++, A--
        "@SP"       "\n"
        "AM=M+1"    "\n"
        "A=A-1"     "\n"

    // 3. *A = D
        "M=D"       "\n"
};

};

#endif
