// Implemented by BarakXYZ 2024

#include "CodeWriter.h"
#include "HelpersVM.h"

CodeWriter::CodeWriter(std::unique_ptr<std::ofstream> outFile, std::string_view programName) 
: outFile(std::move(outFile)), programName{programName} {};

using namespace PushPopCommands;

auto CodeWriter::writePushPopSegment(vmCommand cmd, std::string_view seg, int i) -> void {
    if (seg == "local")
        *outFile << segmentTable[LCL];  // @LCL
    else if (seg == "argument")
        *outFile << segmentTable[ARG];  // @ARG
    else if (seg == "this")
        *outFile << segmentTable[THIS];  // @THIS
    else
        *outFile << segmentTable[THAT];  // @THAT

    *outFile
        << "D=M\n"
        << '@' << i << '\n';

    if(cmd == vmCommand::C_PUSH)
        *outFile << PushPopCommands::pushSegment;
    else
        *outFile << PushPopCommands::popSegment;
}

auto CodeWriter::writePushConstant(int i) -> void {
    *outFile 
        << '@' << i << '\n'
        << PushPopCommands::pushConstant;
}

auto CodeWriter::writePushPopStatic(vmCommand cmd, int i) -> void {
    if(cmd == vmCommand::C_PUSH) {
        *outFile
            << '@' << programName << '.' << i << '\n'
            << PushPopCommands::pushStatic;
    } else {
        *outFile 
            << PushPopCommands::popStatic
            << '@' << programName << '.' << i << '\n'
            << "M=D\n";
    }
}
auto CodeWriter::writePushPopTemp(vmCommand cmd, int i) -> void {
    if(cmd == vmCommand::C_PUSH) {
        *outFile 
            << '@' << i << '\n'
            << PushPopCommands::pushTemp;
    } else {
        *outFile 
            << '@' << i << '\n'
            << PushPopCommands::popTemp;
    }
}
auto CodeWriter::writePushPopPointer(vmCommand cmd, int i) -> void {
    if(cmd == vmCommand::C_PUSH) {
        if(i == 0)
            *outFile << segmentTable[THIS];  // 0 -> @THIS(3)
        else
            *outFile << segmentTable[THAT];  // 1 -> @THAT(4)
        *outFile << PushPopCommands::pushPointer;
    }
    else {
        *outFile << PushPopCommands::popPointer;
        if(i == 0) {
            *outFile 
                << segmentTable[THIS]
                << "M=D\n";
        }
        else {
            *outFile 
                << segmentTable[THAT]
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

using namespace ArithmeticLogicalCommands;
auto CodeWriter::writeLogicalEQ(unsigned int i) -> void {
    *outFile 
        << logicalEqPt1 << i << logicalEqPt2 << i
        << logicalEqPt3 << i << logicalEqPt4 << i
        << logicalEqPt5;
}
using namespace ArithmeticLogicalCommands;

auto CodeWriter::writeLogicalGT(unsigned int i) -> void {
    *outFile 
        << logicalGtPt1 << i << logicalGtPt2 << i
        << logicalGtPt3 << i << logicalGtPt4 << i
        << logicalGtPt5;
}

auto CodeWriter::writeLogicalLT(unsigned int i) -> void {
    *outFile 
        << logicalLtPt1 << i << logicalLtPt2 << i
        << logicalLtPt3 << i << logicalLtPt4 << i
        << logicalLtPt5;
}

auto CodeWriter::writeArithmetic(std::string_view arg0) -> void {
    *outFile << "// " << arg0 << '\n'; 

    if (arg0 == "add") 
        *outFile << arithmeticAdd;
    else if (arg0 == "sub") 
        *outFile << arithmeticSub;
    else if (arg0 == "neg")
        *outFile << arithmeticNeg;
    else if (arg0 == "eq")
        writeLogicalEQ(countLogicalEQ++);
    else if (arg0 == "gt")
        writeLogicalGT(countLogicalGT++);
    else if (arg0 == "lt")
        writeLogicalLT(countLogicalLT++);
    else if (arg0 == "and")
        *outFile << logicalAnd;
    else if (arg0 == "or")
        *outFile << logicalOr;
    else if (arg0 == "not")
        *outFile << logicalNot;
    else {  // Debug
        std::cout << "NO WRITE!\n";
        size_t i{0};
        for(auto c : arg0) {
            std::cout << "Char " << i << ": " << c << '\n';
            ++i;
        }
    }
}
