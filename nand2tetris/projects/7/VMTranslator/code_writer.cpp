#include "code_writer.h"

CodeWriter::CodeWriter(std::unique_ptr<std::ofstream> outFile) : outFile(std::move(outFile)){};
