// Implemented by BarakXYZ 2024

#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include "HelpersVM.h"
#include "PushPopCommands.h"
#include "ArithmeticLogicalCommands.h"
#include "ProgramControlCommands.h"
#include <fstream>
#include <memory>
#include <array>

using namespace HelpersVM;

class CodeWriter{

private:
    std::ofstream outFile;
    std::string_view fileName{};
    unsigned int countLogicalEQ{0}, countLogicalGT{0}, countLogicalLT{0};

public:
    CodeWriter() = default;
    CodeWriter(const std::string &outFileName, const std::string_view entry);
    ~CodeWriter();  // Close outFile

    auto initNewEntry(const std::string &fileName) -> bool;  // Not sure if needed
    auto resetCurrentEntry() -> void;
    auto setFileName(const std::string_view newFileName) -> void;
    auto writeInit() -> void;  // Writes the booting code first thing in the .asm

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

    auto writeLabel(const std::string_view label) -> void;
    auto writeGoto(const std::string_view label) -> void;
    auto writeIf(const std::string_view label) -> void;
    auto writeFunction(const std::string_view functionName, u_int numVars) -> void;
    auto writeCall(const std::string_view functionName, u_int numArgs) -> void;
    auto writeReturn() -> void;

    auto addCommandAsComment(std::string_view arg0) -> void;
    auto addCommandAsComment(vmCommand cmd, std::string_view arg1, std::string_view arg2) -> void;
};

#endif
