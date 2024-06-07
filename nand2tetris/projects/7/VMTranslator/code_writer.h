#ifndef CODE_WRITER_H
#define CODE_WRITER_H
#include <fstream>
#include <memory>

class CodeWriter{

public:
    std::unique_ptr<std::ofstream> outFile;
    CodeWriter(std::unique_ptr<std::ofstream> outFile);
};

#endif
