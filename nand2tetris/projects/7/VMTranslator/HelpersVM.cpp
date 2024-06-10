// Implemented by BarakXYZ 2024

#include "HelpersVM.h"

namespace HelpersVM {

auto stringViewToInt(std::string_view str) -> int {
    int result = 0;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
    if (ec == std::errc()) {
        return result; // Conversion successful
    } else {
        throw std::invalid_argument("Invalid integer format");
    }
}
auto cleanProgramName(std::string_view str) -> std::string_view {
    auto itr{str.find_last_of('/')};

    // Clean '/' (e.g. source/file.vm == file.vm)
    if(itr != std::string_view::npos)
        str = str.substr(itr + 1);

    // Clean '.' (e.g. file.vm == file)
    itr = str.find_last_of('.');
    return str.substr(0, itr);
}

auto incrementNumOfCmds() -> void {
    ++numOfCmdsWritten;
}

auto getNumOfCmdsWritten() -> void {
    std::cout << "Overall Commands Written: " << numOfCmdsWritten << '\n';
}

auto printArgChars(std::string_view) -> void;

}
