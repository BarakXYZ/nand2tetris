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

void FVMWriter::WritePop(ESegment Segment, int Index)
{
	static constexpr std::string_view PopKeyword = "pop";
	OutFileVM << PopKeyword << ' ' << GetSegAsStr(Segment) << ' ' << Index << '\n';
}

void FVMWriter::WriteCall(std::string Name, int NumArgs)
{
	static constexpr std::string_view CallKeyword = "call";
	OutFileVM << CallKeyword << ' ' << Name << ' ' << NumArgs << '\n';
}

void FVMWriter::WriteArithmetic(ECommand Command)
{
	std::string OpStr;
	switch (Command)
	{
		case ECommand::ADD:
		{
			OpStr = "add";
			break;
		}
		case ECommand::SUB:
		{
			OpStr = "sub";
			break;
		}
		case ECommand::NEG:
		{
			OpStr = "neg";
			break;
		}
		case ECommand::NOT:
		{
			OpStr = "not";
			break;
		}
		default:
			OpStr = "invalid_op!";
	}
	OutFileVM << OpStr << '\n';
}

void FVMWriter::WriteReturn()
{
	static constexpr std::string_view RetKeyword = "return";
	OutFileVM << RetKeyword << '\n';
}

const std::string_view FVMWriter::GetSegAsStr(ESegment InSegment)
{
	return SegStrBySegEnum.find(InSegment)->second;
}

ECommand FVMWriter::GetCommandOpByChar(char InOp)
{
	switch (InOp)
	{
		case ('+'):
			return ECommand::ADD;
		case ('-'):
			return ECommand::SUB;
		case ('&'):
			return ECommand::AND;
		case ('|'):
			return ECommand::OR;
		default:
			return ECommand::INVALID;
	}
}

ESegment FVMWriter::GetSegmentByKind(EKind InKind)
{
	switch (InKind)
	{
		case (EKind::FIELD):
			return ESegment::THIS;
		case (EKind::STATIC):
			return ESegment::STATIC;
		case (EKind::ARG):
			return ESegment::ARG;
		case (EKind::VAR):
			return ESegment::LOCAL;

		case (EKind::NONE):
		default:
			return ESegment::NONE;
	}
}
