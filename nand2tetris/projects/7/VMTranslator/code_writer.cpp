#include "code_writer.h"
#include "utilities.h"

CodeWriter::CodeWriter(std::unique_ptr<std::ofstream> outFile, std::string_view programName) 
: outFile(std::move(outFile)), programName{programName} {};

auto CodeWriter::writePushConstant(int i) -> void {
    *outFile << "// addr = segmentPointer + i\n";
    *outFile << "@" << i << '\n';
    *outFile << vmCodeToAssembly::pushConstant;
}

auto CodeWriter::writePushPopSegment(vmCommand cmd, std::string_view segment, int i) -> void {
    if (segment == "local")
        *outFile << "@LCL\n";
    else if (segment == "argument")
        *outFile << "@ARG\n";
    else if (segment == "this")
        *outFile << "@THIS\n";
    else
        *outFile << "@THAT\n";

    *outFile
        << "D=A\n"
        << '@' << i << '\n';

    if(cmd == vmCommand::C_PUSH)
        *outFile << vmCodeToAssembly::pushSegment;
    else
        *outFile << vmCodeToAssembly::popSegment;
}

auto CodeWriter::writePushPopStatic(vmCommand cmd, int i) -> void {
    if(cmd == vmCommand::C_PUSH) {
        *outFile
            << '@' << programName << i << '\n'
            << vmCodeToAssembly::pushStatic;
    } else {
        *outFile 
            << vmCodeToAssembly::popStatic
            << "// *fileName.i = *SP\n"  // Debug
            << '@' << programName << i << '\n'
            << "A=M\n"
            << "M=D\n";
    }
}
auto CodeWriter::writePushPopTemp(vmCommand cmd, int i) -> void {
    if(cmd == vmCommand::C_PUSH) {
        *outFile 
            << "// addr = 5 + i\n" // Debug
            << '@' << i << '\n'
            << vmCodeToAssembly::pushTemp;
    } else {
        *outFile 
            << "// addr = 5 + i\n" // Debug
            << '@' << i << '\n'
            << vmCodeToAssembly::popTemp;
    }
}
auto CodeWriter::writePushPopPointer(vmCommand cmd, int i) -> void {
    *outFile << "// *SP = THIS/THAT\n"; // Debug
    if(!i)
        *outFile << "@3\n";  // 0 -> THIS
    else
        *outFile << "@4\n";  // 1 -> THAT
    *outFile << vmCodeToAssembly::pushPointer;
}

auto CodeWriter::writePushPop(vmCommand commandType, std::string_view arg1, std::string_view arg2) -> void {
    // Add comment line for debugging:
    *outFile << "// push " << arg1 << ' ' << arg2 << '\n'; 
    int i{stringViewToInt(arg2)};

    // Switch on Memory Segment
    if (arg1 == "constant") 
        writePushConstant(i);
    else if (arg1 == "static") 
        writePushPopStatic(commandType, i);
    else if (arg1 == "pointer") 
        writePushPopPointer(commandType, i);
    else if (arg1 == "temp") 
        writePushPopTemp(commandType, i);
    else   // local, argument, this or that.
        writePushPopSegment(commandType, arg1, i);
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

