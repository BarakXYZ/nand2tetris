#include "code_writer.h"

CodeWriter::CodeWriter(std::unique_ptr<std::ofstream> outFile) 
: outFile(std::move(outFile)){};

auto CodeWriter::writePushConstant(std::string_view segment, int i) -> void {
    *outFile << "// addr = segmentPointer + i\n";
    *outFile << "@" << i << '\n' << vmCodeToAssembly::pushConstant;
}

auto CodeWriter::writePushSegment(std::string_view segment, int i) -> void {
    if (segment == "local")
        *outFile << "@LCL\n";
    else if (segment == "argument")
        *outFile << "@ARG\n";
    else if (segment == "this")
        *outFile << "@THIS\n";
    else
        *outFile << "@THAT\n";
    *outFile << "D=A\n" << "@" << i << vmCodeToAssembly::pushSegment;
}

auto CodeWriter::writePush(std::string_view arg1, std::string_view arg2) -> void {
    // Add comment line for debugging:
    *outFile << "// push " << arg1 << ' ' << arg2 << '\n'; 
    int i{stringViewToInt(arg2)};

    // Switch on Memory Segment
    if (arg1 == "constant") {
        writePushConstant(arg1, i);
    }
    else if (arg1 == "static") {

    }
    else if (arg1 == "pointer") {

    }
    else if (arg1 == "temp") {

    }
    else {  // local, argument, this or that.
        writePushSegment(arg1, i);
    }
}

auto CodeWriter::writePop(std::string_view arg1, std::string_view arg2) -> void {
    *outFile << "// pop " << arg1 << ' ' << arg2 << '\n'; 
    // No pop constant
    // local / argument / this / that
    // pop == addr = segmetPointer + i, SP--, *addr = *SP
}

auto CodeWriter::writeArithmetic(std::string_view arg0) -> void {
    *outFile << "// " << arg0 << '\n'; 

}

