// Implemented by BarakXYZ 2024

#ifndef PROGRAM_CONTROL_WRITE_FUNCTION_COMMAND_H
#define PROGRAM_CONTROL_WRITE_FUNCTION_COMMAND_H
#include <string_view>

namespace ProgramControlCommands {

// 1. (functionName)      // declares a label for the function entry
// 2. repeat nVars times: // nVars = number of local variables
// 3. push 0              // initializes the local variables to 0
// Example Command: function Bar.mult 2 (2 aka i aka nVars)
// -------------------------------------------------------
static constexpr std::string_view writeFunctionCommand =

    // 1. (functionName) -> implemented in CodeWriter
        
    // 2-3. repeat nVars times: // nVars = number of local variables
    // Loop nVar times and output to file:
            "@SP"       "\n"
            "AM=M+1"    "\n"
            "A=A-1"     "\n"
            "M=0"       "\n"
    ;

}
#endif
