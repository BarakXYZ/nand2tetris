// Implemented by BarakXYZ 2024

#ifndef LOGICAL_COMMANDS_EQ_H
#define LOGICAL_COMMANDS_EQ_H
#include <string_view>

namespace ArithmeticLogicalCommands {

    constexpr std::string_view logicalEqPt1 =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. D -= *(A--)
            "A=A-1"     "\n"
            "D=M-D"     "\n"

        // 3.1. if(D == 0) -> Goto EQUAL_i
            "@EQUAL_"
        ;

    constexpr std::string_view logicalEqPt2 =
        // 3.1. if(D == 0) -> Goto EQUAL_i
                        "\n"
            "D;JEQ"     "\n"

        // 4.1. else(D != 0) -> D = 0
            "D=0"       "\n"
            "@NOT_EQUAL_"
        ;

    constexpr std::string_view logicalEqPt3 =
                        "\n"
            "0;JMP"     "\n"

        // 3.2. D = -1
            "(EQUAL_"
        ;
    
    constexpr std::string_view logicalEqPt4 =
        // 3.2. D = -1
            ")"         "\n"
            "D=-1"      "\n"

        // 4.2. Goto NOT_EQUAL_i
            "(NOT_EQUAL_"
        ;
    
    constexpr std::string_view logicalEqPt5 =
        // 4.2. Goto NOT_EQUAL_i
            ")"         "\n"

        // 5. *((SP)--) = D
            "@SP"       "\n"
            "A=M-1"     "\n"
            "M=D"       "\n"
        ;

    // -------------------------------------------------------
    // eq
    // 1. D = *(SP--)
    // 2. D -= *(A--)
    // 3. if(D == 0) -> Goto EQUAL_i, D = -1
    // 4. else(D != 0) -> D = 0, Goto NOT_EQUAL_i
    // 5. *((SP)--) = D
    // constexpr std::string_view logicalEq =
    //     // 1. D = *(SP--)
    //         "@SP"       "\n"
    //         "AM=M-1"    "\n"
    //         "D=M"       "\n"

    //     // 2. D -= *(A--)
    //         "A=A-1"     "\n"
    //         "D=M-D"     "\n"

    //     // 3.1. if(D == 0) -> Goto EQUAL_i
    //         "@EQUAL"    "\n"
    //         "D;JEQ"     "\n"

    //     // 4.1. else(D != 0) -> D = 0
    //         "D=0"       "\n"
    //         "@NOT_EQUAL""\n"
    //         "0;JMP"     "\n"

    //     // 3.2. D = -1
    //         "(EQUAL)"   "\n"
    //         "D=-1"      "\n"

    //     // 4.2. Goto NOT_EQUAL_i
    //         "(NOT_EQUAL)""\n"

    //     // 5. *((SP)--) = D
    //         "@SP"       "\n"
    //         "A=M-1"       "\n"
    //         "M=D"       "\n"
    //     ;

};

#endif
