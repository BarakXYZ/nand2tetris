// Implemented by BarakXYZ 2024

#include "CodeWriter.h"
#include "HelpersVM.h"

CodeWriter::CodeWriter(const std::string &outFileASM, const std::string_view entry) {
    initNewEntry(outFileASM);
    fileNameVM = entry;
};

CodeWriter::CodeWriter(const std::string &outFileASM) {
    initNewEntry(outFileASM);  // This seems to happen only once, so probably
                               // can be simplified and no need for function
}

CodeWriter::~CodeWriter() {
    if(outFile.is_open())
        outFile.close();
}

auto CodeWriter::setFileName(const std::string_view newFileVM) -> void {
    outFile
        << "// " << newFileVM << '\n';  // debug
    fileNameVM = newFileVM;
}

// Probably not needed (as the constructor takes care of this)
auto CodeWriter::initNewEntry(const std::string &outFileName) -> bool {
    outFile.open(outFileName, std::ios::trunc);
    if (!outFile) {
        std::cerr << "Error opening output file: " << outFileName << std::endl;
        return false;
    } return true;
}

// Also probably not needed (as this is achieved with the destructor)
auto CodeWriter::resetCurrentEntry() -> void {
    if (outFile.is_open()) 
        outFile.close();
    else
        std::cerr << "File is not opened, could not close file.\n";
}

// -------------------- Write Program Control Commands ----------------------------
using namespace ProgramControlCommands;

auto CodeWriter::writeInit() -> void {
    outFile 
        << "// Booting\n"  // debug
        << booting;
        writeCall("Sys.init", 0);
}

auto CodeWriter::writeLabel(const strView label) -> void {
    HelpersVM::incrementNumOfLabels();  // debug
    outFile 
        // << "test failing bla\n @50"
        << "// label " << label << '\n';  // debug
    if(currentDeclaredFunction.empty()) {
        outFile
            << '(' << label << ")\n";
    }
    else {
        outFile
            << '('
            << currentDeclaredFunction << '$' << label
            << ")\n";
    }
}

auto CodeWriter::writeGoto(const strView label) -> void {
    outFile
        // << "test failing bla\n @50"
        << "// goto " << label << '\n';  // debug
    
    if(currentDeclaredFunction.empty()) {
    outFile
        << '@' << label << '\n'
        << writeGotoCommand;
    }
    else {
        outFile
            << '@' 
            << currentDeclaredFunction << '$' << label << '\n'
            << writeGotoCommand;
    }
}

auto CodeWriter::writeIf(const strView label) -> void {
    outFile
        << "// if-goto " << label << '\n';  // debug

    if(currentDeclaredFunction.empty()) {
        outFile
            << writeIfGotoCommand
            << '@' << label << '\n'
            << "D;JNE\n";
    } else {
        outFile
            << writeIfGotoCommand
            << '@' 
            << currentDeclaredFunction << '$' << label << '\n'
            << "D;JNE\n";
    }
}

auto CodeWriter::writeFunction(const strView functionName, size_t numVars) -> void {
    HelpersVM::incrementNumOfLabels();  // debug
    // Update function name for goto and if-goto
    currentDeclaredFunction = functionName;

    outFile
        << "// function " << functionName << ' ' << numVars << '\n'  // debug
        << '(' << functionName << ")\n";  // e.g. (xxx.foo)

    // 2. repeat nVars times: // nVars = number of local variables
    // Loop nVar times and output to file:
    for(size_t i{0}; i < numVars; ++i) {
        outFile
            << writeFunctionCommand;  // Assembly
    }
}

auto CodeWriter::writeCall(const strView functionName, size_t numArgs) -> void {
    HelpersVM::incrementNumOfLabels();  // debug
    outFile
        << "// call " << functionName << ' ' << numArgs << '\n'  // debug
        << '@' << fileNameVM << "$ret." << ++countReturn << '\n' // Not sure here
        << writeCallPt1

    // 6. ARG = SP-5-nArgs  // Repositions ARG
        // D = i+5, D = SP-D, ARG = D
        << '@' << numArgs << '\n'
        << writeCallPt2

    // 8. goto functionName  // Transfers control to the called function
        << '@' << functionName << '\n'  // e.g. (xxx.foo)
        << "0;JMP\n"

    // 9. (@file$ret.x)  // Declares a label for the return to jump back to
        << '(' << fileNameVM << "$ret." << countReturn << ")\n";
}

auto CodeWriter::writeReturn() -> void {
    outFile
        << "// return (function " << currentDeclaredFunction << ")\n"  // debug
        << writeReturnCommand;
}

// -------------------- Write Push Pop Commands ------------------------------

using namespace PushPopCommands;

auto CodeWriter::writePushPop(vmCommand cmdType, std::string_view arg1, std::string_view arg2) -> void {
    // Add the VM command itself as a comment for debugging:
    addCommandAsComment(cmdType, arg1, arg2);
    int i{strViewToInt(arg2)};

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
        outFile << segmentTable[LCL];  // @LCL
    else if (seg == "argument")
        outFile << segmentTable[ARG];  // @ARG
    else if (seg == "this")
        outFile << segmentTable[THIS];  // @THIS
    else
        outFile << segmentTable[THAT];  // @THAT

    outFile
        << "D=M\n"
        << '@' << i << '\n';

    if(cmd == vmCommand::C_PUSH)
        outFile << PushPopCommands::pushSegment;
    else
        outFile << PushPopCommands::popSegment;
}

auto CodeWriter::writePushConstant(int i) -> void {
    outFile 
        << '@' << i << '\n'
        << PushPopCommands::pushConstant;
}

auto CodeWriter::writePushPopStatic(vmCommand cmd, int i) -> void {
    if(cmd == vmCommand::C_PUSH) {
        outFile
            << '@' << fileNameVM << '.' << i << '\n'
            << PushPopCommands::pushStatic;
    } else {
        outFile 
            << PushPopCommands::popStatic
            << '@' << fileNameVM << '.' << i << '\n'
            << "M=D\n";
    }
}
auto CodeWriter::writePushPopTemp(vmCommand cmd, int i) -> void {
    if(cmd == vmCommand::C_PUSH) {
        outFile 
            << '@' << i << '\n'
            << PushPopCommands::pushTemp;
    } else {
        outFile 
            << '@' << i << '\n'
            << PushPopCommands::popTemp;
    }
}
auto CodeWriter::writePushPopPointer(vmCommand cmd, int i) -> void {
    if(cmd == vmCommand::C_PUSH) {
        if(i == 0)
            outFile << segmentTable[THIS];  // 0 -> @THIS(3)
        else
            outFile << segmentTable[THAT];  // 1 -> @THAT(4)
        outFile << PushPopCommands::pushPointer;
    }
    else {
        outFile << PushPopCommands::popPointer;
        if(i == 0) {
            outFile 
                << segmentTable[THIS]
                << "M=D\n";
        }
        else {
            outFile 
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
                outFile << arithmeticAdd;
                break;
            case ArithmeticCommands::SUB:
                outFile << arithmeticSub;
                break;
            case ArithmeticCommands::NEG:
                outFile << arithmeticNeg;
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
                outFile << logicalAnd;
                break;
            case ArithmeticCommands::OR:
                outFile << logicalOr;
                break;
            case ArithmeticCommands::NOT:
                outFile << logicalNot;
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
    outFile 
        << logicalEqPt1 << i << logicalEqPt2 << i
        << logicalEqPt3 << i << logicalEqPt4 << i
        << logicalEqPt5;
}
using namespace ArithmeticLogicalCommands;

auto CodeWriter::writeLogicalGT(unsigned int i) -> void {
    outFile 
        << logicalGtPt1 << i << logicalGtPt2 << i
        << logicalGtPt3 << i << logicalGtPt4 << i
        << logicalGtPt5;
}

auto CodeWriter::writeLogicalLT(unsigned int i) -> void {
    outFile 
        << logicalLtPt1 << i << logicalLtPt2 << i
        << logicalLtPt3 << i << logicalLtPt4 << i
        << logicalLtPt5;
}

// Debug Push-Pop Commands
auto CodeWriter::addCommandAsComment(vmCommand cmd, std::string_view arg1, std::string_view arg2) -> void {
    if (cmd == vmCommand::C_PUSH)
        outFile << "// push " << arg1 << ' ' << arg2 << '\n'; 
    else if (cmd == vmCommand::C_POP)
        outFile << "// pop " << arg1 << ' ' << arg2 << '\n'; 
}

// Debug Arithmetic Command
auto CodeWriter::addCommandAsComment(std::string_view arg0) -> void {
        outFile << "// " << arg0 << '\n'; 
}
