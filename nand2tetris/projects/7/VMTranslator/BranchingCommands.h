// Implemented by BarakXYZ 2024

#ifndef PROGRAM_CONTROL_COMMANDS_BRANCHING_COMMANDS_H
#define PROGRAM_CONTROL_COMMANDS_BRANCHING_COMMANDS_H
#include <string_view>

namespace ProgramControlCommands {

// Un-conditional jump
// @arg2 (i.e. label)
// 0;JMP
constexpr std::string_view writeGotoCommand {
    // "@SP"           "\n"
    // @arg2 implemenet in CodeWriter
    "0;JMP"         "\n"
};

// D = *((SP)--))  -> fetch the condition in the top of the stack
// @arg2 (i.e. label)  -> label to jump to
// Jump if less than 0 (true == -1)  -> if cond==true, jump
constexpr std::string_view writeIfGotoCommand {
    "@SP"           "\n"
    "AM=M-1"         "\n"
    // "A=A-1"         "\n"
    "D=M"           "\n"
    // @arg2 (i.e. label)  -> implement in CodeWriter
    // "D;JNE"         "\n"
};


}
#endif
