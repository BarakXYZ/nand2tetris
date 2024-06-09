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

    // push constant i
    // *SP = i, SP++
    // D = @i, SP++, A--, *A = D
    static constexpr std::string_view pushConstant = 

        // 1. D = @i
            // @i
            "D=A"       "\n"

        // 2. SP++, A--
            "@SP"       "\n"
            "AM=M+1"    "\n"
            "A=A-1"     "\n"

        // 3. *A = D
            "M=D"       "\n"
        ;


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
    // push static i
    // *SP = fileName.i, SP++
    // D = fileName.i, SP++, A--, *A = D
    static constexpr std::string_view pushStatic =

        // 1. D = fileName.i  (implemented in CodeWriter)
            // @fileName.i
            // D=M
        
        // 2. SP++, A--
            "@SP"       "\n"
            "AM=M+1"    "\n"
            "A+A-1"     "\n"

        // 3. *A = D
            "M=D"       "\n"
        ;


    // -------------------------------------------------------
    // SP--, *fileName.i = *SP
    // D = *(SP--), *fileName.i = *SP
    static constexpr std::string_view popStatic =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. *fileName.i = *SP (implemented in CodeWriter)
            // *fileName.i = *SP
            // @fileName.i
            // M=D
        ;

    // -------------------------------------------------------
    // addr = 5 + i, *SP = *addr, SP++
    // D = *(temp + i), SP++, *(A--) = D
    static constexpr std::string_view pushTemp =

        // 1. D = *(temp + 1)
            // @i
            "D=A"       "\n"
            "@R5"       "\n"
            "A=D+A"     "\n"
            "D=M"       "\n"

        // 2. SP++
            "@SP"       "\n"
            "AM=M+1"    "\n"

        // 3. *(A--) = D
            "A=A-1"     "\n"
            "M=D"       "\n"
        ;

    // -------------------------------------------------------
    // addr = 5 + i, SP--, *addr = *SP
    // R13 = temp + i, D = *(SP--), *(temp+i) = D
    static constexpr std::string_view popTemp =
        
        // 1. R13 = temp + i
            // @i
            "D=A"       "\n"
            "@R5"       "\n"
            "D=D+A"     "\n"
            "@R13"      "\n"
            "M=D"       "\n"

        // 2. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"
        
        // 3. *(temp+i) = D
            "@R13"      "\n"
            "A=M"       "\n"
            "M=D"       "\n"
        ;


    // -------------------------------------------------------
    // *SP = THIS/THAT, SP++
    // D = THIS/THAT, SP++, A--, *A = THIS/THAT
    static constexpr std::string_view pushPointer =

        // 1. D = THIS/THAT (implemented in CodeWriter)
            // if(0) -> @3
            // else  -> @4
            "D=M"       "\n"

        // 2. SP++
            "@SP"       "\n"
            "AM=M-1"    "\n"

        // 3. A--
            "A=A-1"     "\n"

        // 4. *A = THIS/THAT
            "M=D"       "\n"
        ;


    // -------------------------------------------------------
    // SP--, THIS/THAT = *SP
    // D = *(SP--), THIS/THAT = D
    static constexpr std::string_view popPointer =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"
        
        // 2. THIS/THAT = D (implemented in CodeWriter)
            // if  (0)  -> @3
            // else(1)  -> @4
            // "M=D"
        ;

    // -------------------------------------------------------
    // SP--, addr = *SP, SP--, *SP = *SP + addr, SP++
    // D = *(SP--), A--, *A += D
    static constexpr std::string_view add =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. A--
            "A=A-1"     "\n"

        // 3. *A += D
            "M=M+D"     "\n"
        ;

    // -------------------------------------------------------
    // SP--, addr = *SP, SP--, *SP = addr - *SP, SP++
    // D = *(SP--), A--, *A -= D
    static constexpr std::string_view sub =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. A--
            "A=A-1"     "\n"

        // 3. *A -= D
            "M=M-D"     "\n"
        ;


    // SP--, *SP = -(*SP), SP++
    static constexpr std::string_view neg =
    // SP--
    "@SP\n"
    "M=M-1\n"

    // *SP = -(*SP)
    "A=M\n"
    "M=-M\n"

    // SP++
    "@SP\n"
    "M=M+1\n"
    ;

    /*
    SP--, addr = *SP, SP--
    *SP = *SP - addr
    if(*SP == 0)
        addr = -1
    else
        addr = 0
    *SP = addr
    SP++
    */
    static constexpr std::string_view eq =
    // SP--
    "@SP\n"
    "M=M-1\n"
    
    // addr = *SP
    "A=M\n"
    "D=M\n"

    // SP--
    "@SP\n"
    "M=M-1\n"

    // addr = *SP - addr
    "A=M\n"
    "D=M-D\n"
    "@EQUAL\n"
    "D;JEQ\n"

    // else -> addr = 0
    "D=0\n"
    "@NOT_EQUAL\n"
    "0;JMP\n"

    // if -> addr = -1
    "(EQUAL)\n"
    "D=-1\n"

    "(NOT_EQUAL)\n"

    // *SP = addr
    "@SP\n"
    "A=M\n"
    "M=D\n"

    // SP++
    "@SP\n"
    "M=M+1\n"
    ;
};

int stringViewToInt(std::string_view str);

#endif
