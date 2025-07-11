// Implemented by BarakXYZ 2025

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "SymbolTable.h"

/**
 * @brief Converts an EKind enum to its string representation.
 * @param Kind The enum value to convert.
 * @return A string like "static", "field", "argument", or "var".
 */
inline std::string KindToString(EKind Kind)
{
	switch (Kind)
	{
		case EKind::STATIC:
			return "static";
		case EKind::FIELD:
			return "field";
		case EKind::ARG:
			return "argument";
		case EKind::VAR:
			return "local";
		case EKind::NONE:
		default:
			return "unknown";
	}
}

/**
 * @brief Prints a formatted, ordered representation of a symbol table to std::cout.
 * @param table The FSymbolTable instance to visualize.
 * @param title A title to print above the table (e.g., "Class Table: Main").
 */
inline void PrintSymbolTable(const FSymbolTable& table, const std::string& title)
{
	// --- Step 1: Collect all entries from the map ---
	std::vector<FIdentifierDetails> sortedEntries;
	for (const auto& pair : table.SymTableMap)
	{
		sortedEntries.push_back(pair.second);
	}

	// --- Step 2: Sort entries by kind and then by index for a meaningful order ---
	// Helper lambda to define the sorting priority of each kind.
	auto getKindPriority = [](EKind kind) {
		switch (kind)
		{
			case EKind::FIELD:
				return 0; // Fields first for class scope
			case EKind::STATIC:
				return 1; // Then statics
			case EKind::ARG:
				return 2; // Arguments first for subroutine scope
			case EKind::VAR:
				return 3; // Then locals
			case EKind::NONE:
			default:
				return 4; // Should not happen in valid tables
		}
	};

	std::sort(sortedEntries.begin(), sortedEntries.end(),
		[&](const FIdentifierDetails& a, const FIdentifierDetails& b) {
			int priorityA = getKindPriority(a.Kind);
			int priorityB = getKindPriority(b.Kind);

			// If kinds have different priorities, sort based on that priority.
			if (priorityA != priorityB)
			{
				return priorityA < priorityB;
			}

			// If kinds are the same, sort by their declaration index.
			return a.Index < b.Index;
		});

	// --- Step 3: Determine column widths for nice formatting ---
	size_t maxNameWidth = 4; // Min width for "Name"
	size_t maxTypeWidth = 4; // Min width for "Type"
	for (const auto& entry : sortedEntries)
	{
		if (entry.Name.length() > maxNameWidth)
			maxNameWidth = entry.Name.length();
		if (entry.Type.length() > maxTypeWidth)
			maxTypeWidth = entry.Type.length();
	}
	const size_t kindWidth = 8;	 // "argument" is the longest
	const size_t indexWidth = 5; // "Index"

	// --- Step 4: Print the formatted table ---
	// Helper lambda to print a separator line
	auto printSeparator = [&]() {
		std::cout << "+-" << std::string(maxNameWidth, '-') << "-+-" << std::string(maxTypeWidth, '-') << "-+-" << std::string(kindWidth, '-') << "-+-" << std::string(indexWidth, '-') << "-+\n";
	};

	std::cout << "\n";
	printSeparator();
	// Print the main title spanning across the top
	std::string centeredTitle = "| " + title;
	std::cout << std::left << std::setw(maxNameWidth + maxTypeWidth + kindWidth + indexWidth + 10) << centeredTitle << "|\n";
	printSeparator();

	if (sortedEntries.empty())
	{
		std::cout << "| " << std::left << std::setw(maxNameWidth + maxTypeWidth + kindWidth + indexWidth + 7) << "[Table is Empty]" << "|\n";
	}
	else
	{
		// Print header
		std::cout << "| " << std::left << std::setw(maxNameWidth) << "Name" << " | "
				  << std::left << std::setw(maxTypeWidth) << "Type" << " | "
				  << std::left << std::setw(kindWidth) << "Kind" << " | "
				  << std::left << std::setw(indexWidth) << "Index" << " |\n";
		printSeparator();

		// Print rows
		for (const auto& entry : sortedEntries)
		{
			std::cout << "| " << std::left << std::setw(maxNameWidth) << entry.Name << " | "
					  << std::left << std::setw(maxTypeWidth) << entry.Type << " | "
					  << std::left << std::setw(kindWidth) << KindToString(entry.Kind) << " | "
					  << std::right << std::setw(indexWidth) << entry.Index << " |\n";
		}
	}

	printSeparator();
	std::cout << "\n";
}
