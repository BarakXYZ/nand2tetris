#include "code_writer.h"
#include "utilities.h"

CodeWriter::CodeWriter(std::unique_ptr<std::ofstream> outFile, std::string_view programName) 
: outFile(std::move(outFile)), programName{programName} {};

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
        << "D=M\n"
        << '@' << i << '\n';

    if(cmd == vmCommand::C_PUSH)
        *outFile << vmCodeToAssembly::pushSegment;
    else
        *outFile << vmCodeToAssembly::popSegment;
}

auto CodeWriter::writePushConstant(int i) -> void {
    *outFile 
        << '@' << i << '\n'
        << vmCodeToAssembly::pushConstant;
}

auto CodeWriter::writePushPopStatic(vmCommand cmd, int i) -> void {
    if(cmd == vmCommand::C_PUSH) {
        *outFile
            << '@' << programName << '.' << i << '\n'
            << vmCodeToAssembly::pushStatic;
    } else {
        *outFile 
            << vmCodeToAssembly::popStatic
            << '@' << programName << '.' << i << '\n'
            // << "A=M\n"
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
    if(cmd == vmCommand::C_PUSH) {
        if(i == 0)
            *outFile << "@THIS\n";  // 0 -> THIS(3)
        else
            *outFile << "@THAT\n";  // 1 -> THAT(4)
        *outFile << vmCodeToAssembly::pushPointer;
    }
    else {
        *outFile << vmCodeToAssembly::popPointer;
        if(i == 0) {
            *outFile 
                << "@THIS\n"
                << "M=D\n";
        }
        else {
            *outFile 
                << "@THAT\n"
                << "M=D\n";
        }
    }
}


auto CodeWriter::writePushPop(vmCommand commandType, std::string_view arg1, std::string_view arg2) -> void {
    // Add the command itself as a comment for debugging:
    if (commandType == vmCommand::C_PUSH)
        *outFile << "// push " << arg1 << ' ' << arg2 << '\n'; 
    else
        *outFile << "// pop " << arg1 << ' ' << arg2 << '\n'; 
    ++numOfCmdsWritten;

    int i{stringViewToInt(arg2)};

    // Switch on Memory Segment and convert to assembly
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

auto CodeWriter::writeArithmetic(std::string_view arg0) -> void {
    *outFile << "// " << arg0 << '\n'; 
    ++numOfCmdsWritten;

    if (arg0 == "add") 
        *outFile << vmCodeToAssembly::arithmeticAdd;
    else if (arg0 == "sub") 
        *outFile << vmCodeToAssembly::arithmeticSub;
    else if (arg0 == "neg")
        *outFile << vmCodeToAssembly::arithmeticNeg;
    else if (arg0 == "eq")
        *outFile << vmCodeToAssembly::logicalEq;
    else if (arg0 == "gt")
        *outFile << vmCodeToAssembly::logicalGt;
    else if (arg0 == "lt")
        *outFile << vmCodeToAssembly::logicalLt;
    else if (arg0 == "and")
        *outFile << vmCodeToAssembly::logicalAnd;
    else if (arg0 == "or")
        *outFile << vmCodeToAssembly::logicalOr;
    else if (arg0 == "not")
        *outFile << vmCodeToAssembly::logicalNot;
    else {  // Debug
        std::cout << "NO WRITE!\n";
        size_t i{0};
        for(auto c : arg0) {
            std::cout << "Char " << i << ": " << c << '\n';
            ++i;
        }
    }
}

// Debug number of written commands
auto CodeWriter::getNumOfCmdsWritten() -> void {
    std::cout << "Overall Commands Written: " << numOfCmdsWritten << '\n';
}

