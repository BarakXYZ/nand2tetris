// Implemented by BarakXYZ 2024

#ifndef LOGICAL_COMMANDS_LT_H
#define LOGICAL_COMMANDS_LT_H
#include <string_view>

namespace ArithmeticLogicalCommands {

    constexpr std::string_view logicalLtPt1 =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. D -= *(A--)
            "A=A-1"     "\n"
            "D=M-D"     "\n"

        // 3.1. if(D < 0) -> Goto LESS_THAN_i
            "@LESS_THAN_"
        ;

    constexpr std::string_view logicalLtPt2 =
        // 3.1. if(D < 0) -> Goto LESS_THAN_i
                        "\n"
            "D;JLT"     "\n"

        // 4.1. else(D >= 0) -> D = 0
            "D=0"       "\n"
            "@NOT_LESS_THAN_"
        ;

    constexpr std::string_view logicalLtPt3 =
                        "\n"
            "0;JMP"     "\n"

        // 3.2. D = -1
            "(LESS_THAN_"
        ;
    
    constexpr std::string_view logicalLtPt4 =
        // 3.2. D = -1
            ")"         "\n"
            "D=-1"      "\n"

        // 4.2. Goto NOT_LESS_THAN_i
            "(NOT_LESS_THAN_"
        ;
    
    constexpr std::string_view logicalLtPt5 =
        // 4.2. Goto NOT_LESS_THAN_i
            ")"         "\n"

        // 5. *((SP)--) = D
            "@SP"       "\n"
            "A=M-1"     "\n"
            "M=D"       "\n"
        ;

    // -------------------------------------------------------
    // 1. D = *(SP--)
    // 2. D -= *(A--)
    // 3. if(D < 0) -> Goto LESS_THAN_i, D = -1
    // 4. else(D >= 0) -> D = 0, Goto NOT_LESS_THAN_i
    // 5. *((SP)--) = D
    // static constexpr std::string_view logicalLt =

    //     // 1. D = *(SP--)
    //         "@SP"       "\n"
    //         "AM=M-1"    "\n"
    //         "D=M"       "\n"

    //     // 2. D -= *(A--)
    //         "A=A-1"     "\n"
    //         "D=M-D"     "\n"

    //     // 3.1. if(D < 0) -> Goto LESS_THAN_i
    //         "@LESS_THAN""\n"
    //         "D;JLT"     "\n"

    //     // 4.1. else(D >= 0) -> D = 0
    //         "D=0"       "\n"
    //         "@NOT_LESS_THAN""\n"
    //         "0;JMP"     "\n"

    //     // 3.2. D = -1
    //         "(LESS_THAN)"   "\n"
    //         "D=-1"      "\n"

    //     // 4.2. Goto NOT_LESS_THAN_i
    //         "(NOT_LESS_THAN)""\n"

    //     // 5. *((SP)--) = D
    //         "@SP"       "\n"
    //         "A=M-1"     "\n"
    //         "M=D"       "\n"
    //     ;


    // -------------------------------------------------------

};

#endif
