// Implemented by BarakXYZ 2024

#ifndef PROGRAM_CONTROL_FUNCTION_CALL_COMMANDS_H
#define PROGRAM_CONTROL_FUNCTION_CALL_COMMANDS_H
#include <string_view>

namespace ProgramControlCommands {

// 1. push returnAddress  // (Using the label declared below)
// 2. push LCL            // Saves LCL of the caller
// 3. push ARG            // Saves ARG of the caller
// 4. push THIS           // Saves THIS of the caller
// 5. push THAT           // Saves THAT of the caller
// 6. ARG = SP-5-nArgs    // Repositions ARG
// 7. LCL = SP            // Repositions LCL
// 8. goto functionName   // Transfers control to the called function
// 9. (returnAddress)     // Declares a label for the return-address to jump back to
// Example Command: call Bar.mult 2 (2 aka 'i')
// -------------------------------------------------------
static constexpr std::string_view writeCallPt1 =

    // 1. push returnAddress  // (Using the label declared below)
        // D = SP - i, *(@file$ret.x) = D, D = @file$ret.x, *SP = D, SP++
        // "@i"        "\n" // Implemented in CodeWriter (arg2)
        "D=A"       "\n"
        "@SP"       "\n"
        "D=A-D"     "\n"
    ;

static constexpr std::string_view writeCallPt2 =
        // @file$ret.x
        "M=D"       "\n"
        "D=A"       "\n"
        
        // *SP = D, SP++
        "@SP"       "\n"
        "AM=M+1"    "\n"
        "A=A-1"     "\n"
        "M=D"       "\n"
        
    // 2. push LCL  // Saves LCL of the caller
        // D = LCL, *SP = D, SP++
        "@LCL"      "\n"
        "D=A"       "\n"
        "@SP"       "\n"
        "AM=M+1"    "\n"
        "A=A-1"     "\n"
        "M=D"       "\n"

    // 3. push ARG  // Saves ARG of the caller
        // D = ARG, *SP = D, SP++
        "@ARG"      "\n"
        "D=A"       "\n"
        "@SP"       "\n"
        "AM=M+1"    "\n"
        "A=A-1"     "\n"
        "M=D"       "\n"

    // 4. push THIS  // Saves THIS of the caller
        // D = THIS, *SP = D, SP++
        "@THIS"     "\n"
        "D=A"       "\n"
        "@SP"       "\n"
        "AM=M+1"    "\n"
        "A=A-1"     "\n"
        "M=D"       "\n"
    // 5. push THAT  // Saves THAT of the caller
        // D = THAT, *SP = D, SP++
        "@THAT"     "\n"
        "D=A"       "\n"
        "@SP"       "\n"
        "AM=M+1"    "\n"
        "A=A-1"     "\n"
        "M=D"       "\n"
    ;
static constexpr std::string_view writeCallPt3 =
    // 6. ARG = SP-5-nArgs  // Repositions ARG
        // D = i+5, D = SP-D, ARG = D
        // "@i"        "\n"  // Implemented in CodeWriter
        "D=A"       "\n"
        "@5"        "\n"
        "D=D+A"     "\n"
        "@SP"       "\n"
        "D=A-D"     "\n"
        "@ARG"      "\n"
        "M=D"       "\n"
    // 7. LCL = SP  // Repositions LCL
        // D = SP, LCL = D
        "@SP"       "\n"
        "D=A"       "\n"
        "@LCL"      "\n"
        "M=D"       "\n"
    
    // Implemented in CodeWriter:
    // 8. goto functionName  // Transfers control to the called function
        // @arg1(func name e.g. Bar.mult)
        // 0;JMP

    // 9. (@file$ret.x)  // Declares a label for the return to jump back to
    ;


// -------------------------------------------------------



}
#endif
