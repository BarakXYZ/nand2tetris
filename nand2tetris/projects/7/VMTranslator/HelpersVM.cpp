// Implemented by BarakXYZ 2024

#include "HelpersVM.h"

namespace HelpersVM {

auto openAndCheckFile(const std::string &fileName, std::ifstream &inFile) -> bool {
    inFile.open(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Error opening input file: " << fileName << std::endl;
        return false;
    }
    if (inFile.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "File is empty: " << fileName << std::endl;
        inFile.close();
        return false;
    }
    return true;
}

auto closeFile(std::ifstream &inFile) -> void {
    if (inFile.is_open()) 
        inFile.close();
    else
        std::cerr << "Couldn't close file.\n";
}

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
    size_t itr{0};

    // e.g. source/file.vm/
    if(str.back() == '/') {
        str.remove_suffix(1);
    }
    itr = str.rfind('/');

    // Clean '/' (e.g. source/file.vm == file.vm)
    if(itr != std::string_view::npos)
        str = str.substr(itr + 1);  // itr + 1 (pos of itr -> end  == file.vm)
                                    // not: itr, 1 (which will result in 'f')

    // Clean '.' (e.g. file.vm == file)
    itr = str.rfind('.');
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

auto extractDirName(std::string_view path) -> std::string {
    size_t itr{0};
    std::string dirName(path);

    // e.g. source/dir/  == source/dir
    if(dirName.back() == '/') 
        dirName.pop_back();

    itr = dirName.rfind('/');
    // Clean '/' (e.g. source/file.vm == file.vm)
    if(itr != std::string::npos) 
        dirName = dirName.substr(itr + 1);  // itr + 1 vs. itr, 1

    // Clean '.' (e.g. file.vm == file) | ".vm".length() == 3
    if((itr = dirName.rfind('.')) != std::string::npos)
        dirName = dirName.substr(0, dirName.length() - 3);

    dirName += ".asm";
    return dirName;
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
