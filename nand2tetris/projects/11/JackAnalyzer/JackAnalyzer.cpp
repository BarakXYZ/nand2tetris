// Implemented by BarakXYZ 2025

#include <filesystem>
#include <iostream>
#include <memory>
#include "AnalyzerUtils.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include "JackAnalyzer.h"

// Exit codes
enum EExitCode
{
	SUCCESS = 0,
	INVALID_ARGUMENTS = 1,
	PATH_NOT_FOUND = 2,
	NOT_A_JACK_FILE = 3,
	FILE_PROCESSING_FAILED = 4,
	INVALID_FILE_TYPE = 5
};

int JackAnalyzer::Run(const std::string& InFileName, std::string& OutFileName)
{
	if (!std::filesystem::exists(InFileName))
	{
		std::cerr << "Error: Path does not exist: " << InFileName << '\n';
		return PATH_NOT_FOUND;
	}

	if (std::filesystem::is_directory(InFileName))
	{
		std::cout << "Processing directory: " << InFileName << '\n';
		for (const auto& File : std::filesystem::directory_iterator(InFileName))
		{
			if (File.path().extension() != ".jack") // Skip non-jack files
				continue;

			const std::string NewFile = File.path().string();
			std::cout << "Processing file: " << NewFile << '\n';

			if (!ProcessFile(NewFile, OutFileName))
			{
				std::cerr << "Error: Failed to process file: " << NewFile << '\n';
				return FILE_PROCESSING_FAILED;
			}
		}
	}
	else if (std::filesystem::is_regular_file(InFileName) && std::filesystem::path(InFileName).extension() == ".jack")
	{
		std::cout << "Processing single file: " << InFileName << '\n';
		if (!ProcessFile(InFileName, OutFileName))
		{
			std::cerr << "Error: Failed to process file: " << InFileName << '\n';
			return FILE_PROCESSING_FAILED;
		}
	}
	else
	{
		std::cerr << "Error: Invalid file type or missing .jack extension: " << InFileName << '\n';
		return INVALID_FILE_TYPE;
	}

	return SUCCESS;
}

bool JackAnalyzer::ProcessFile(const std::string& InFileName, std::string& OutFileName)
{
	OutFileName = AnalyzerUtils::ReplaceExtension(InFileName, ".xml");
	std::cout << "Output file: " << OutFileName << '\n';

	std::shared_ptr<FJackTokenizer> JackTokenizer =
		std::make_shared<FJackTokenizer>(InFileName);

	std::shared_ptr<FCompilationEngine> CompilationEngine =
		std::make_shared<FCompilationEngine>(
			InFileName, OutFileName, JackTokenizer);

	CompilationEngine->CompileClass();
	return true; // There's not a way for this function to return false, so why?
}

auto main(int argc, char* argv[]) -> int
{
	if (argc != 2 && argc != 3)
	{
		std::cerr << "\nUsage: <file_name.jack> [OutputPath] OR <directory_name> [OutputPath]\n";
		return INVALID_ARGUMENTS;
	}

	std::string InFileName{ argv[1] };
	std::string OutFileName = (argc == 3) ? argv[2] : argv[1];

	int result = JackAnalyzer::Run(InFileName, OutFileName);

	if (result != SUCCESS)
		std::cerr << "JackAnalyzer failed with exit code: " << result << '\n';
	else
		std::cout << "JackAnalyzer finished successfully!\n";

	return result;
}
