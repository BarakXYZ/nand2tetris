#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <charconv>
#include <iostream>

enum vmCommand {
    C_ARITHMETIC,
    C_PUSH, C_POP,
    C_LABEL, C_GOTO,
    C_IF, C_FUNCTION,
    C_RETURN, C_CALL,
};

struct vmCodeToAssembly {
    // push constant i == *SP = i, SP++
    static constexpr std::string_view pushConstant = 
        // "// *SP = i\n"
        // "@i\n"
        "D=A\n"
        "@SP\n"
        "A=M\n"

        "// SP++\n"
        "M=D\n"
        "M=M+1\n";

    // addr = segmentPointer + i, *SP = *addr, SP++
    // local (LCL) / argument (ARG) / this (THIS) / that (THAT)

    static constexpr std::string_view pushSegment =
        // addr = segmentPointer + i\n
        // @segmentPointer
        // D=A
        // @i
        "A=D+A\n"
        // "D=A+D\n"
        // "@R13\n"
        // "M=D\n"

        "// *SP = *addr\n"
        // "A=M\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"

        "// SP++\n"
        "@SP\n"
        "M=M+1\n";

    // addr = segmentPointer + i, SP--, *addr = *SP
    static constexpr std::string_view popSegment =
        // addr = segmentPointer + i\n
        // @segmentPointer
        // D=A
        // @i
        "D=D+A\n"
        "@R13\n"
        "M=D\n"

        "// SP--\n"  // Debug
        "@SP\n"
        "M=M-1\n"

        "// *addr = *SP\n"  // Debug
        "A=M\n"
        "D=M\n"
        "@R13\n"
        "A=M\n"
        "M=D\n";


    // *SP = *fileName.i, SP++
    static constexpr std::string_view pushStatic =
        // @fileName.i
        // D=M
        "@SP\n"
        "A=M\n"
        "M=D\n"

        "// SP++\n"  // Debug
        "@SP\n"
        "M=M+1\n";
    
    // SP--, *fileName.i = *SP
    static constexpr std::string_view popStatic =
        // SP--
        "@SP\n"
        "M=M-1\n"
        "A=M\n"
        "D=M\n"
        // *fileName.i = *SP
        // @fileName.i
        // A=M
        // M=D
        ;

    // addr = 5 + i, *SP = *addr, SP++
    static constexpr std::string_view pushTemp =
        // "// addr = 5 + i\n" // Debug
        // @i
        "D=A\n"
        "@5\n"
        "A=D+A\n"
        "D=M\n"

        "// *SP = *addr\n" // Debug
        "@SP\n"
        "A=M\n"
        "M=D\n"

        "// SP++\n" // Debug
        "@SP\n"
        "M=M+1\n";

    // addr = 5 + i, SP--, *addr = *SP
    static constexpr std::string_view popTemp =
        
        // addr = 5 + i
        // @i
        "D=A\n"
        "@5\n"
        "D=D+A\n"
        "@13\n"
        "M=D\n"

        // SP--
        "@SP\n"
        "M=M-1\n"
        
        // *addr = *SP
        "A=M\n"
        "D=M\n"
        "@13\n"
        "A=M\n"
        "M=D\n";


    // *SP = THIS/THAT, SP++
    static constexpr std::string_view pushPointer =
        // "// *SP = THIS/THAT\n" // Debug
        // if 0
        // "@3\n"
        // else
        // "@4\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"

        "// SP++\n" // Debug
        "@SP\n"
        "M=M+1\n";
};

int stringViewToInt(std::string_view str);

#endif
