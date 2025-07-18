// Implemented by BarakXYZ 2025
//
#pragma once

#include "SymbolTable.h"
#include <string>
#include <fstream>
#include <unordered_map>

/**
 * Enumeration for the different Memory Segments.
 */
enum ESegment
{
	CONST,
	ARG,
	LOCAL,
	STATIC,
	THIS,
	THAT,
	POINTER,
	TEMP,
	NONE,
};

/**
 * Enumeration for the different Commands.
 */
enum ECommand
{
	ADD,
	SUB,
	NEG,
	EQ,
	GT,
	LT,
	AND,
	OR,
	NOT,
	INVALID,
};

class FVMWriter
{
public:
	/**
	 * Creates a new output.vm file and prepares it for writing.
	 */
	FVMWriter(const std::string& InFile);
	~FVMWriter();

	/**
	 * Writes a VM push command.
	 */
	void WritePush(ESegment Segment, int Index);

	/**
	 * Writes a VM pop command.
	 */
	void WritePop(ESegment Segment, int Index);

	/**
	 * Writes a VM arithmetic-logical command.
	 */
	void WriteArithmetic(ECommand Command);

	/**
	 * Writes a VM label command.
	 */
	void WriteLabel(std::string_view Label);

	/**
	 * Writes a VM goto command.
	 */
	void WriteGoto(std::string_view Label);

	/**
	 * Writes a VM if-goto command.
	 */
	void WriteIf(std::string_view Label);

	/**
	 * Writes a VM call command.
	 * Look into this to implement mult & div:
	 * https://github.com/woai3c/nand2tetris/blob/master/12/Math.jack
	 */
	void WriteCall(std::string_view Name, int NumArgs);

	/**
	 * Writes a VM function command.
	 */
	void WriteFunction(const std::string_view Name, int NumLocals);

	/**
	 * Writes a VM return command.
	 */
	void WriteReturn();

	/**
	 * Closes the output file.
	 */
	void Close();

	const std::string_view GetSegAsStr(ESegment InSegment);

	static ECommand GetCommandOpByChar(char InOp);
	static ESegment GetSegmentByKind(EKind InKind);

private:
	std::ofstream OutFileVM;

	std::unordered_map<ESegment, std::string_view> SegStrBySegEnum{
		{ ESegment::ARG, "argument" },
		{ ESegment::CONST, "constant" },
		{ ESegment::TEMP, "temp" },
		{ ESegment::LOCAL, "local" },
		{ ESegment::STATIC, "static" },
		{ ESegment::THAT, "that" },
		{ ESegment::THIS, "this" },
		{ ESegment::POINTER, "pointer" },
	};

	std::unordered_map<ECommand, std::string_view> CmdStrByCmdEnum{
		{ ECommand::ADD, "add" }
	};
};
