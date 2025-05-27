// Implemented by BarakXYZ 2025

#include "SymbolTable.h"

void FSymbolTable::Reset()
{
	ClassSymTable.clear();
	SubSymTable.clear();
}

std::optional<FSymbolTable::SymbolMapConstIterator> FSymbolTable::FindEntry(const std::string& Name) const
{
	auto ItSub = SubSymTable.find(Name);
	if (ItSub != SubSymTable.end())
	{
		return std::optional(ItSub);
	}

	auto ItrClass = ClassSymTable.find(Name);
	if (ItrClass != ClassSymTable.end())
	{
		return std::optional(ItrClass);
	}

	return std::nullopt; // "Not found"
}

int FSymbolTable::IndexOf(const std::string& Name)
{
	auto OptItr = FindEntry(Name);
	return OptItr ? OptItr.value()->second.Index : -1;
}
