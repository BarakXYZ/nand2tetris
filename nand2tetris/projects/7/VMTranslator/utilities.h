#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <charconv>
#include <iostream>

int stringViewToInt(std::string_view str);
auto cleanProgramName(std::string_view str) -> std::string_view;

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
            "D=M"       "\n"
        
        // 2. SP++, A--
            "@SP"       "\n"
            "AM=M+1"    "\n"
            "A=A-1"     "\n"

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


    // -------------------------------------------------------
    // add | D = *(SP--), A--, *A += D
    static constexpr std::string_view arithmeticAdd =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. A--, *A += D
            "A=A-1"     "\n"
            "M=M+D"     "\n"
        ;


    // -------------------------------------------------------
    // sub | D = *(SP--), A--, *A -= D
    static constexpr std::string_view arithmeticSub =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. A--, *A -= D
            "A=A-1"     "\n"
            "M=M-D"     "\n"
        ;


    // -------------------------------------------------------
    // neg | *((SP)--) = -(*((SP)--))
    static constexpr std::string_view arithmeticNeg =

        // 1. *((SP)--) = -(*((SP)--))
            "@SP"       "\n"
            "A=M-1"     "\n"
            "M=-M"      "\n"
        ;


    // eq
    // 1. D = *(SP--)
    // 2. D -= *(A--)
    // 3. if(D == 0) -> Goto EQUAL, D = -1
    // 4. else(D != 0) -> D = 0, Goto NOT_EQUAL
    // 5. *((SP)--) = D
    static constexpr std::string_view logicalEq =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. D -= *(A--)
            "A=A-1"     "\n"
            "D=M-D"     "\n"

        // 3.1. if(D == 0) -> Goto EQUAL
            "@EQUAL"    "\n"
            "D;JEQ"     "\n"

        // 4.1. else(D != 0) -> D = 0
            "D=0"       "\n"
            "@NOT_EQUAL""\n"
            "0;JMP"     "\n"

        // 3.2. D = -1
            "(EQUAL)"   "\n"
            "D=-1"      "\n"

        // 4.2. Goto NOT_EQUAL
            "(NOT_EQUAL)""\n"

        // 5. *((SP)--) = D
            "@SP"       "\n"
            "A=M-1"       "\n"
            "M=D"       "\n"
        ;

    // 1. D = *(SP--)
    // 2. D -= *(A--)
    // 3. if(D > 0) -> Goto GREATER, D = -1
    // 4. else(D <= 0) -> D = 0, Goto NOT_GREATER
    // 5. *((SP)--) = D
    static constexpr std::string_view logicalGt =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. D -= *(A--)
            "A=A-1"     "\n"
            "D=M-D"     "\n"

        // 3.1. if(D > 0) -> Goto GREATER
            "@GREATER"  "\n"
            "D;JGT"     "\n"

        // 4.1. else(D <= 0) -> D = 0
            "D=0"       "\n"
            "@NOT_GREATER""\n"
            "0;JMP"     "\n"

        // 3.2. D = -1
            "(GREATER)" "\n"
            "D=-1"      "\n"

        // 4.2. Goto NOT_GREATER
            "(NOT_GREATER)""\n"

        // 5. *((SP)--) = D
            "@SP"       "\n"
            "A=M-1"     "\n"
            "M=D"       "\n"
        ;

    // 1. D = *(SP--)
    // 2. D -= *(A--)
    // 3. if(D < 0) -> Goto LESS_THAN, D = -1
    // 4. else(D >= 0) -> D = 0, Goto NOT_LESS_THAN
    // 5. *((SP)--) = D
    static constexpr std::string_view logicalLt =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. D -= *(A--)
            "A=A-1"     "\n"
            "D=M-D"     "\n"

        // 3.1. if(D > 0) -> Goto GREATER
            "@LESS_THAN""\n"
            "D;JLT"     "\n"

        // 4.1. else(D <= 0) -> D = 0
            "D=0"       "\n"
            "@NOT_LESS_THAN""\n"
            "0;JMP"     "\n"

        // 3.2. D = -1
            "(LESS_THAN)"   "\n"
            "D=-1"      "\n"

        // 4.2. Goto NOT_GREATER
            "(NOT_LESS_THAN)""\n"

        // 5. *((SP)--) = D
            "@SP"       "\n"
            "A=M-1"     "\n"
            "M=D"       "\n"
        ;


    // 1. D = *(SP--)
    // 2. if(D == 0) -> Goto FALSE, D = 0
    // 3. D = *(A--)
    // 4. if(D == 0) -> Goto FALSE, D = 0
    // 5. else -> D = -1, Goto TRUE
    // 6. *((SP)--) = D
    static constexpr std::string_view logicalAnd =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"
            "A=A-1"     "\n"
            "M=D&M"     "\n"

        // // 2.1. if(D == 0) -> Goto FALSE, D = 0
        //     "@FALSE"    "\n"
        //     "D;JEQ"     "\n"

        // // 3. D = *(A--)
        //     "A=A-1"     "\n"
        //     "D=M"       "\n"

        // // 4.1. if(D == 0) -> Goto FALSE, D = 0
        //     "@FALSE"    "\n"
        //     "D;JEQ"     "\n"

        // // 5. else -> D = -1, Goto TRUE
        //     "D=-1"      "\n"
        //     "@TRUE"     "\n"
        //     "0;JMP"     "\n"

        // // 2.2,4.2. Goto FALSE
        //     "(FALSE)"   "\n"
        //     "D=0"       "\n"

        // // 5.2. Goto TRUE
        //     "(TRUE)"    "\n"

        // // 6. *((SP)--) = D
        //     "@SP"       "\n"
        //     "A=M-1"     "\n"
        //     "M=D"       "\n"
        ;

    // 1. D = *(SP--)
    // 2. if(D != 0) -> Goto TRUE, D = -1
    // 3. D = *(A--)
    // 4. if(D != 0) -> Goto TRUE, D = -1
    // 5. else -> D = 0, Goto FALSE
    // 6. *((SP)--) = D
    static constexpr std::string_view logicalOr =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"
            "A=A-1"     "\n"
            "M=D|M"     "\n"

        // // 2.1. if(D != 0) -> Goto TRUE, D = -1
        //     "@TRUE"     "\n"
        //     "D;JNE"     "\n"

        // // 3. D = *(A--)
        //     "A=A-1"     "\n"
        //     "D=M"       "\n"

        // // 4.1. if(D != 0) -> Goto TRUE, D = -1
        //     "@TRUE"     "\n"
        //     "D;JNE"     "\n"

        // // 5. else -> D = -1, Goto FALSE
        //     "D=0"       "\n"
        //     "@FALSE"    "\n"
        //     "0;JMP"     "\n"

        // // 2.2,4.2. Goto TRUE
        //     "(TRUE)"    "\n"
        //     "D=-1"      "\n"

        // // 5.2. Goto FALSE
        //     "(FALSE)"   "\n"

        // // 6. *((SP)--) = D
        //     "@SP"       "\n"
        //     "A=M-1"     "\n"
        //     "M=D"       "\n"
        ;

    // 1. D = *((SP)--)
    // 2. if(D == 0) -> Goto False, D = 0
    // 3. else -> D = -1, Goto TRUE
    // 4. *((SP)--) = D
    static constexpr std::string_view logicalNot =

        // 1. D = *(SP--)
            "@SP"       "\n"
            "A=M-1"     "\n"
            "M=!M"      "\n"

        // // 2.1. if(D == 0) -> Goto TRUE, D = -1
        //     "@TRUE"     "\n"
        //     "D;JEQ"     "\n"

        // // 3.1 else -> D = 0, Goto FALSE
        //     "D=0"       "\n"
        //     "@FALSE"    "\n"
        //     "0;JMP"     "\n"

        // // 2.2. Goto TRUE
        //     "(TRUE)"    "\n"
        //     "D=-1"      "\n"

        // // 3.2. Goto FALSE
        //     "(FALSE)"   "\n"

        // // 6. *((SP)--) = D
        //     "@SP"       "\n"
        //     "A=M-1"     "\n"
        //     "M=D"       "\n"
        ;
};


#endif
