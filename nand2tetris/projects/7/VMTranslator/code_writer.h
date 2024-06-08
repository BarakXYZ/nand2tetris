#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include "utilities.h"
#include <fstream>
#include <memory>

class CodeWriter{

private:
    std::unique_ptr<std::ofstream> outFile;
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
            "D=A+D\n"
            "@13\n"
            "M=D\n"

            "// *SP = *addr\n"
            "A=M\n"
            "D=M\n"
            "@SP\n"
            "A=M\n"
            "M=D\n"

            "// SP++\n"
            "@SP\n"
            "M=M+1\n";
    };


public:
    CodeWriter(std::unique_ptr<std::ofstream> outFile);
    ~CodeWriter() { outFile->close(); }; 

    auto writePush(std::string_view, std::string_view) -> void;
    auto writePushConstant(std::string_view, int) -> void;
    auto writePushSegment(std::string_view, int) -> void;

    auto writePop(std::string_view, std::string_view) -> void;
    auto writeArithmetic(std::string_view) -> void;
};

#endif
