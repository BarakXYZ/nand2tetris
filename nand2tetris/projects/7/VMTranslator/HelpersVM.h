// Implemented by BarakXYZ 2024

#ifndef HELPERS_VM
#define HELPERS_VM

#include <string>
#include <charconv>
#include <iostream>
#include <array>

namespace HelpersVM {

enum vmCommand {
    C_ARITHMETIC,
    C_PUSH, C_POP,
    C_LABEL, C_GOTO,
    C_IF, C_FUNCTION,
    C_RETURN, C_CALL,
};

enum SegTags {
    SP, LCL, ARG, THIS, THAT, CONSTANT, STATIC, TEMP, POINTER,
    Count
};

using tbl = std::array<std::string_view, static_cast<size_t>(SegTags::Count)>;
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

static unsigned int numOfCmdsWritten{0};

auto stringViewToInt(std::string_view str) -> int;
auto cleanProgramName(std::string_view str) -> std::string_view;
auto printArgChars(std::string_view) -> void;
auto incrementNumOfCmds() -> void;
auto getNumOfCmdsWritten() -> void;
}


#endif
