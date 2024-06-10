// Implemented by BarakXYZ 2024

#ifndef PUSH_POP_COMMANDS_PUSH_POP_SEGMENT_H
#define PUSH_POP_COMMANDS_PUSH_POP_SEGMENT_H
#include <string_view>

namespace PushPopCommands {

    // -------------------------------------------------------
    // push segment i
    // addr = segmentPointer + i, *SP = *addr, SP++
    // local (LCL) / argument (ARG) / this (THIS) / that (THAT)
    // D = segmentPointer + i, SP++, A--, *A = D
    static constexpr std::string_view pushSegment =

        // 1. D = segmentPointer + i  // (implemented in CodeWriter)
            // @segmentPointer
            // D=M
            // @i
            "A=D+A"     "\n"
            "D=M"       "\n"
        
        // 2. SP++, A--
            "@SP"       "\n"
            "AM=M+1"    "\n"
            "A=A-1"     "\n"

        // 3. *A = D
            "M=D"       "\n"
        ;


    // -------------------------------------------------------
    // pop segment i
    // addr = segmentPointer + i, SP--, *addr = *SP
    // R13 = segmentPointer + i, D = *(SP--), R13 = D
    static constexpr std::string_view popSegment =
        // 1. R13 = segmentPointer + i
            // @segmentPointer
            // D=M
            // @i
            "D=D+A"     "\n"
            "@R13"      "\n"
            "M=D"       "\n"

        // 2. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 3. R13 = D
            "@R13"      "\n"
            "A=M"       "\n"
            "M=D"       "\n"
        ;


    // -------------------------------------------------------

};

#endif
