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

auto replaceExtension(std::string_view filename, std::string_view new_extension) -> std::string {
    std::string result{filename};
    auto pos = result.rfind('.');
    if (pos != std::string::npos) {
        result.replace(pos, std::string::npos, new_extension);
    } else {
        result += new_extension;
    }
    return result;
}

auto incrementNumOfCmds() -> void {
    ++numOfCmdsWritten;
}

auto getNumOfCmdsWritten() -> void {
    std::cout << "Overall Commands Written: " << numOfCmdsWritten << '\n';
}

auto debugArgChars(std::string_view arg) -> void {
    std::cout << "NO WRITE!\n";
    size_t i{0};
    for(auto c : arg) {
        std::cout << "Char " << i << ": " << c << '\n';
        ++i;
    }
}

}
