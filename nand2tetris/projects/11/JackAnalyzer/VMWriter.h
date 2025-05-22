#pragma once

#include <string>

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
	TEMP
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
	NOT
};

class FVMWriter
{
	/**
	 * Creates a new output.vm file and prepares it for writing.
	 */
	FVMWriter(std::string InFile);

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
	void WriteLabel(std::string Label);

	/**
	 * Writes a VM goto command.
	 */
	void WriteGoto(std::string Label);

	/**
	 * Writes a VM if-goto command.
	 */
	void WriteIf(std::string Label);

	/**
	 * Writes a VM call command.
	 */
	void WriteCall(std::string Name, int NumArgs);

	/**
	 * Writes a VM function command.
	 */
	void WriteFunction(std::string Name, int NumLocals);

	/**
	 * Writes a VM return command.
	 */
	void WriteReturn();

	/**
	 * Closes the output file.
	 */
	void Close();
};
