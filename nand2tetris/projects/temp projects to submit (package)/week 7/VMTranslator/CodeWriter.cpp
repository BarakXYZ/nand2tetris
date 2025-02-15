// Implemented by BarakXYZ 2024

#include "CodeWriter.h"
#include "HelpersVM.h"

CodeWriter::CodeWriter(std::unique_ptr<std::ofstream> outFile, std::string_view programName) 
: outFile(std::move(outFile)), programName{programName} {};

using namespace PushPopCommands;

auto CodeWriter::writePushPop(vmCommand cmdType, std::string_view arg1, std::string_view arg2) -> void {
    // Add the VM command itself as a comment for debugging:
    addCommandAsComment(cmdType, arg1, arg2);
    int i{stringViewToInt(arg2)};

    // Switch on Memory Segment and convert to assembly
    auto it = segmentMap.find(arg1);
    if (it != segmentMap.end()) {
        switch (it->second) {
            case SegTags::CONSTANT:
                writePushConstant(i);
                break;
            case SegTags::STATIC:
                writePushPopStatic(cmdType, i);
                break;
            case SegTags::POINTER:
                writePushPopPointer(cmdType, i);
                break;
            case SegTags::TEMP:
                writePushPopTemp(cmdType, i);
                break;
            default:  // Implicit: LCL, ARG, THIS, THAT
                writePushPopSegment(cmdType, arg1, i);
                break;
        }
    } else {
        std::cerr 
        << "Invalid Push-Pop Command: " 
        << cmdType << ' ' << arg1 << '\n';
    }
}

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

// -------------------- Write Arithmetic Commands ------------------------------

using namespace ArithmeticLogicalCommands;

auto CodeWriter::writeArithmetic(std::string_view arg0) -> void {
    // Add the VM command itself as a comment for debugging:
    addCommandAsComment(arg0);

    auto it = arithmeticMap.find(arg0);
    if (it != arithmeticMap.end()) {
        switch (it->second) {
            case ArithmeticCommands::ADD:
                *outFile << arithmeticAdd;
                break;
            case ArithmeticCommands::SUB:
                *outFile << arithmeticSub;
                break;
            case ArithmeticCommands::NEG:
                *outFile << arithmeticNeg;
                break;
            case ArithmeticCommands::EQ:
                writeLogicalEQ(countLogicalEQ++);
                break;
            case ArithmeticCommands::GT:
                writeLogicalGT(countLogicalGT++);
                break;
            case ArithmeticCommands::LT:
                writeLogicalLT(countLogicalLT++);
                break;
            case ArithmeticCommands::AND:
                *outFile << logicalAnd;
                break;
            case ArithmeticCommands::OR:
                *outFile << logicalOr;
                break;
            case ArithmeticCommands::NOT:
                *outFile << logicalNot;
                break;
            default:
                // Debug Invalid VM Command
                debugArgChars(arg0);
                break;
        }
    } else {
        // Debug Invalid VM Command
        debugArgChars(arg0);
    }
}

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

// Debug Push-Pop Commands
auto CodeWriter::addCommandAsComment(vmCommand cmd, std::string_view arg1, std::string_view arg2) -> void {
    if (cmd == vmCommand::C_PUSH)
        *outFile << "// push " << arg1 << ' ' << arg2 << '\n'; 
    else if (cmd == vmCommand::C_POP)
        *outFile << "// pop " << arg1 << ' ' << arg2 << '\n'; 
}

// Debug Arithmetic Command
auto CodeWriter::addCommandAsComment(std::string_view arg0) -> void {
        *outFile << "// " << arg0 << '\n'; 
}
