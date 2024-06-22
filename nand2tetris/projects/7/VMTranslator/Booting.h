// Implemented by BarakXYZ 2024

#ifndef PROGRAM_CONTROL_COMMANDS_BOOTING_H
#define PROGRAM_CONTROL_COMMANDS_BOOTING_H
#include <string_view>

namespace ProgramControlCommands {

// SP=256
// Call Sys.init
constexpr std::string_view booting {
    "@256"          "\n"
    "D=A"           "\n"
    "@SP"           "\n"
    "M=D"           "\n"

    // "call Sys.init" "\n"
    "@Sys.Sys.init"     "\n"
    "0;JMP"         "\n"
};

}
#endif
