#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include "utilities.h"
#include <fstream>
#include <memory>

class CodeWriter{

private:
    std::unique_ptr<std::ofstream> outFile;
    std::string_view programName{};
    unsigned long numOfCmdsWritten{0};

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
    auto getNumOfCmdsWritten() -> void;
};

#endif
