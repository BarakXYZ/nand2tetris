// Implemented by BarakXYZ 2025

#include "JackTokenizer.h"
#include "CompilationEngine.h"

namespace JackAnalyzer
{
	int	 Run(const std::string& InFileName, std::string& OutFileName);
	bool ProcessFile(const std::string& InFileName, std::string& OutFileName);
	bool ProcessFileContent(
		std::shared_ptr<FJackTokenizer>		Tokenizer,
		std::shared_ptr<FCompilationEngine> CompilationEngine);
}; // namespace JackAnalyzer
