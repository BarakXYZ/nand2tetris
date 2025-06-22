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
	static constexpr std::string_view FuncKeyword = "function";
	OutFileVM << FuncKeyword << ' ' << Name << ' ' << NumLocals << '\n';
}

void FVMWriter::WritePush(ESegment Segment, int Index)
{
	static constexpr std::string_view PushKeyword = "push";
	OutFileVM << PushKeyword << ' ' << GetSegAsStr(Segment) << ' ' << Index << '\n';
}

void FVMWriter::WriteCall(std::string Name, int NumArgs)
{
	static constexpr std::string_view CallKeyword = "call";
	OutFileVM << CallKeyword << ' ' << Name << ' ' << NumArgs << '\n';
}

const std::string_view FVMWriter::GetSegAsStr(ESegment InSegment)
{
	return SegStrBySegEnum.find(InSegment)->second;
}
