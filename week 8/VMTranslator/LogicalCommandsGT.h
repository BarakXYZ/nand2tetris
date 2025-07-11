// Implemented by BarakXYZ 2024

#ifndef LOGICAL_COMMANDS_GT_H
#define LOGICAL_COMMANDS_GT_H
#include <string_view>

namespace ArithmeticLogicalCommands {

    constexpr std::string_view logicalGtPt1 =
        // 1. D = *(SP--)
            "@SP"       "\n"
            "AM=M-1"    "\n"
            "D=M"       "\n"

        // 2. D -= *(A--)
            "A=A-1"     "\n"
            "D=M-D"     "\n"

        // 3.1. if(D > 0) -> Goto GREATER_i
            "@GREATER_"

        ;

    constexpr std::string_view logicalGtPt2 =
        // 3.1. if(D > 0) -> Goto GREATER_i
                        "\n"
            "D;JGT"     "\n"

        // 4.1. else(D <= 0) -> D = 0
            "D=0"       "\n"
            "@NOT_GREATER_"
        ;

    constexpr std::string_view logicalGtPt3 =
                        "\n"
            "0;JMP"     "\n"

        // 3.2. D = -1
            "(GREATER_"
        ;
    
    constexpr std::string_view logicalGtPt4 =
        // 3.2. D = -1
            ")"         "\n"
            "D=-1"      "\n"

        // 4.2. Goto NOT_GREATER_i
            "(NOT_GREATER_"
        ;
    
    constexpr std::string_view logicalGtPt5 =
        // 4.2. Goto NOT_GREATER_i
            ")"         "\n"

        // 5. *((SP)--) = D
            "@SP"       "\n"
            "A=M-1"     "\n"
            "M=D"       "\n"
        ;

    // -------------------------------------------------------
    // 1. D = *(SP--)
    // 2. D -= *(A--)
    // 3. if(D > 0) -> Goto GREATER_i, D = -1
    // 4. else(D <= 0) -> D = 0, Goto NOT_GREATER_i
    // 5. *((SP)--) = D
    // static constexpr std::string_view logicalGt =
    //     // 1. D = *(SP--)
    //         "@SP"       "\n"
    //         "AM=M-1"    "\n"
    //         "D=M"       "\n"

    //     // 2. D -= *(A--)
    //         "A=A-1"     "\n"
    //         "D=M-D"     "\n"

    //     // 3.1. if(D > 0) -> Goto GREATER_i
    //         "@GREATER"  "\n"
    //         "D;JGT"     "\n"

    //     // 4.1. else(D <= 0) -> D = 0
    //         "D=0"       "\n"
    //         "@NOT_GREATER""\n"
    //         "0;JMP"     "\n"

    //     // 3.2. D = -1
    //         "(GREATER)" "\n"
    //         "D=-1"      "\n"

    //     // 4.2. Goto NOT_GREATER_i
    //         "(NOT_GREATER)""\n"

    //     // 5. *((SP)--) = D
    //         "@SP"       "\n"
    //         "A=M-1"     "\n"
    //         "M=D"       "\n"
    //     ;


    // -------------------------------------------------------


};

#endif
