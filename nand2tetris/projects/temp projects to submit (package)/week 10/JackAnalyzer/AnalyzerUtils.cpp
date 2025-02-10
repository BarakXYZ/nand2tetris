// Implemented by BarakXYZ 2025

#include "AnalyzerUtils.h"

namespace AnalyzerUtils
{
	std::string ReplaceExtension(
		std::string_view FileName,
		std::string_view NewExtension)
	{
		std::string Result{ FileName };
		auto		Pos = Result.rfind('.');
		if (Pos != std::string::npos)
		{
			Result.replace(Pos, std::string::npos, NewExtension);
		}
		else
		{
			Result += NewExtension;
		}
		return Result;
	}
} // namespace AnalyzerUtils
