// Implemented by BarakXYZ 2024

#ifndef HELPERS_VM
#define HELPERS_VM

#include <fstream>
#include <string>
#include <charconv>
#include <iostream>
#include <array>
#include <unordered_map>

namespace HelpersVM {

using strView = std::string_view;

enum vmCommand {
    C_ARITHMETIC,
    C_PUSH, C_POP,
    C_LABEL, C_GOTO,
    C_IF, C_FUNCTION,
    C_RETURN, C_CALL,
};

const std::unordered_map<std::string_view, vmCommand> commandMap = {
    {"push", C_PUSH},
    {"pop", C_POP},
    {"label", C_LABEL},
    {"goto", C_GOTO},
    {"if-goto", C_IF},
    {"function", C_FUNCTION},
    {"return", C_RETURN},
    {"call", C_CALL},
    // else artihmetic (i.e. add, sub, neg, etc.)
};

enum SegTags {
    SP, LCL, ARG, THIS, THAT, CONSTANT, STATIC, TEMP, POINTER,
    CountSeg
};

using tbl = std::array<std::string_view, static_cast<size_t>(SegTags::CountSeg)>;
constexpr tbl segmentTable = {{
    "@SP\n",
    "@LCL\n",
    "@ARG\n",
    "@THIS\n",
    "@THAT\n",
    "@CONSTANT\n",  // This isn't needed, added for correct enumeration
    "@STATIC\n",
    "@TEMP\n",
    "@POINTER\n",
}};

const std::unordered_map<std::string_view, SegTags> segmentMap = {
    {"sp", SegTags::SP},
    {"local", SegTags::LCL},
    {"argument", SegTags::ARG},
    {"this", SegTags::THIS},
    {"that", SegTags::THAT},
    {"constant", SegTags::CONSTANT},
    {"static", SegTags::STATIC},
    {"temp", SegTags::TEMP},
    {"pointer", SegTags::POINTER}
};

enum ArithmeticCommands {
    ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT,
    CountArithmetic
};

const std::unordered_map<std::string_view, ArithmeticCommands> arithmeticMap = {
    {"add", ArithmeticCommands::ADD},
    {"sub", ArithmeticCommands::SUB},
    {"neg", ArithmeticCommands::NEG},
    {"eq", ArithmeticCommands::EQ},
    {"gt", ArithmeticCommands::GT},
    {"lt", ArithmeticCommands::LT},
    {"and", ArithmeticCommands::AND},
    {"or", ArithmeticCommands::OR},
    {"not", ArithmeticCommands::NOT}
};

static unsigned int numOfCmdsWritten{0};
static unsigned int numOfLabelsWritten{0};

auto strViewToInt(std::string_view str) -> int;
auto cleanProgramName(std::string_view str) -> std::string_view;
auto replaceExtension(std::string_view filename, std::string_view new_extension) -> std::string;
auto makeDirOutFileName(std::string_view dirName) -> std::string;
auto debugArgChars(std::string_view) -> void;
auto incrementNumOfCmds() -> void;
auto getNumOfCmdsWritten() -> void;
auto incrementNumOfLabels() -> void;
auto getNumOfLabelsWritten() -> void;

auto isFileOrDir(const std::string_view);
auto extractDirName(std::string_view) -> std::string;

inline constexpr std::string_view usage{" <input_file.vm> [output_file.asm]\n"};

}
#endif
