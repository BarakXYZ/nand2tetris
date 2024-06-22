// Implemented by BarakXYZ 2024

#ifndef PROGRAM_CONTROL_WRITE_RETURN_COMMAND_H
#define PROGRAM_CONTROL_WRITE_RETURN_COMMAND_H
#include <string_view>

namespace ProgramControlCommands {

// 1. endFrame = LCL               // endFrame is a temporary variable
// 2. retAddr = *(endFrame - 5)    // gets the return address
// 3. *ARG = pop()                 // Repositions the return value for the caller
// 4. SP = ARG + 1                 // Repositions SP of the caller
// 5. THAT = *(endFrame - 1)       // Restore THAT of the caller
// 6. THIS = *(endFrame - 1)       // Restore THIS of the caller
// 7. ARG = *(endFrame - 1)        // Restore ARG of the caller
// 8. LCL = *(endFrame - 1)        // Restore LCL of the caller
// 9. goto retAddr                 // goes to return address in the caller's code
static constexpr std::string_view writeReturnCommand {
    // 1. endFrame = LCL
        "@LCL"      "\n"
        "D=M"       "\n"
        "@R13"      "\n"  // endFrame == R13 (temp reg)
        "M=D"       "\n"
        
    // 2. retAddr = *(endFrame - 5)
        "@5"        "\n"
        "D=A"       "\n"
        "@R13"      "\n"
        "A=M-D"     "\n"
        "D=M"       "\n"
        "@R14"      "\n"  // retAddr == R14 (temp reg)
        "M=D"       "\n"

    // 3. *ARG = pop()
        "@SP"       "\n"
        "A=M-1"     "\n"
        "D=M"       "\n"
        "@ARG"      "\n"
        "A=M"       "\n"
        "M=D"       "\n"


    // 4. SP = ARG + 1                 // Repositions SP of the caller
        // We are already at A of ARG so:
        "D=A+1"     "\n"
        "@SP"       "\n"
        "M=D"       "\n"

    // 5. THAT = *(endFrame - 1)       // Restore THAT of the caller
        "@R13"      "\n"
        "AM=M-1"    "\n"
        "D=M"       "\n"
        "@THAT"     "\n"
        "M=D"       "\n"


    // 6. THIS = *(endFrame - 1)       // Restore THIS of the caller
        "@R13"      "\n"
        "AM=M-1"    "\n"
        "D=M"       "\n"
        "@THIS"     "\n"
        "M=D"       "\n"

    // 7. ARG = *(endFrame - 1)        // Restore ARG of the caller
        "@R13"      "\n"
        "AM=M-1"    "\n"
        "D=M"       "\n"
        "@ARG"      "\n"
        "M=D"       "\n"

    // 8. LCL = *(endFrame - 1)        // Restore LCL of the caller
        "@R13"      "\n"
        "AM=M-1"    "\n"
        "D=M"       "\n"
        "@LCL"      "\n"
        "M=D"       "\n"

        
    // 9. goto retAddr                // goes to return address in the caller's code
        "@R14"      "\n"
        "A=M"       "\n"  // We will jump to A (*R14) in this case (not R14)
        "0;JMP"     "\n"
};

}
#endif
