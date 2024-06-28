// Implemented by BarakXYZ 2024

#ifndef PUSH_POP_COMMANDS_PUSH_POP_POINTER_H
#define PUSH_POP_COMMANDS_PUSH_POP_POINTER_H
#include <string_view>

namespace PushPopCommands {

// -------------------------------------------------------
// push pointer 0/1
// *SP = THIS/THAT, SP++
// D = THIS/THAT, SP++, A--, *A = D
static constexpr std::string_view pushPointer =

    // 1. D = THIS/THAT (implemented in CodeWriter)
        // if  (0) -> @THIS
        // else(1) -> @THAT
        // "A=M"       "\n"  // Do we want *THIS/THAT?
        "D=M"       "\n"

    // 2. SP++, A--
        "@SP"       "\n"
        "AM=M+1"    "\n"
        "A=A-1"     "\n"

    // 3. *A = D
        "M=D"       "\n"
    ;


// -------------------------------------------------------
// pop pointer 0/1
// | SP--, THIS/THAT = *SP 
// | D = *(SP--), THIS/THAT = D
static constexpr std::string_view popPointer =

    // 1. D = *(SP--)
        "@SP"       "\n"
        "AM=M-1"    "\n"
        "D=M"       "\n"
    
    // 2. THIS/THAT = D (implemented in CodeWriter)
        // if  (0)  -> @THIS
        // else(1)  -> @THAT
        // "A=M"  // not sure
        // "M=D"
    ;
};

#endif
