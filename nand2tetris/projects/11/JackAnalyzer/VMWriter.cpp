#include "VMWriter.h"
#include "AnalyzerUtils.h"
#include <iostream>

FVMWriter::FVMWriter(const std::string& InFile)
{
	const std::string OutFileNameVM = AnalyzerUtils::ReplaceExtension(InFile, ".vm");
	std::cout << "Output VM file: " << OutFileNameVM << '\n';
	OutFileVM.open(OutFileNameVM, std::ios::trunc);
	if (!OutFileVM)
	{
		std::cerr << "Error opening VM output file: " << OutFileNameVM << std::endl;
		std::exit(99);
	}
}

FVMWriter::~FVMWriter()
{
	if (OutFileVM.is_open())
		OutFileVM.close();
}

void FVMWriter::WriteFunction(const std::string& Name, int NumLocals)
{
}
