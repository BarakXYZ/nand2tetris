// Implemented by BarakXYZ 2024

#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include "HelpersVM.h"
#include "PushPopCommands.h"
#include "ArithmeticLogicalCommands.h"
#include <fstream>
#include <memory>
#include <array>

using namespace HelpersVM;

class CodeWriter{

private:
    std::unique_ptr<std::ofstream> outFile;
    std::string_view programName{};
    unsigned int countLogicalEQ{0}, countLogicalGT{0}, countLogicalLT{0};


public:
    CodeWriter(std::unique_ptr<std::ofstream>, std::string_view);
    ~CodeWriter() { outFile->close(); }; 

    auto writePushPop(vmCommand, std::string_view, std::string_view) -> void;
        auto writePushPopSegment(vmCommand, std::string_view, int) -> void;
        auto writePushConstant(int) -> void;  // *no pop for constant
        auto writePushPopStatic(vmCommand, int) -> void;
        auto writePushPopTemp(vmCommand, int) -> void;
        auto writePushPopPointer(vmCommand, int) -> void;

    auto writeArithmetic(std::string_view) -> void;
        auto writeLogicalEQ(unsigned int) -> void;
        auto writeLogicalGT(unsigned int) -> void;
        auto writeLogicalLT(unsigned int) -> void;

    auto addCommandAsComment(std::string_view arg0) -> void;
    auto addCommandAsComment(vmCommand cmd, std::string_view arg1, std::string_view arg2) -> void;
};

#endif
