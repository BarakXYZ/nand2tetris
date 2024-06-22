// Implemented by BarakXYZ 2024

#ifndef PROGRAM_CONTROL_COMMANDS_BOOTING_H
#define PROGRAM_CONTROL_COMMANDS_BOOTING_H
#include <string_view>

namespace ProgramControlCommands {

// 1. SP=256
// 2. Call Sys.init
constexpr std::string_view booting {

    // 1. SP=256
    "@256"          "\n"
    "D=A"           "\n"
    "@SP"           "\n"
    "M=D"           "\n"

    // 2. Call Sys.init -> implemented in CodeWriter
};

}
#endif
