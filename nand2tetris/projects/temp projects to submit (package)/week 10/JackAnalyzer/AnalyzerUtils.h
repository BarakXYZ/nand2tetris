// Implemented by BarakXYZ 2025

#pragma once

#include <fstream>
#include <string>
#include <iostream>

namespace AnalyzerUtils
{
	std::string ReplaceExtension(
		std::string_view FileName,
		std::string_view NewExtension);

} // namespace AnalyzerUtils
