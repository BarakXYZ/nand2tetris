// Implemented by BarakXYZ 2025

#include "SymbolTable.h"

std::unordered_map<std::string, FIdentifierDetails> FSymbolTable::ClassSymTable;
std::unordered_map<std::string, FIdentifierDetails> FSymbolTable::SubSymTable;

void FSymbolTable::Reset()
{
	ClassSymTable.clear();
	SubSymTable.clear();
}
void FSymbolTable::StartSubroutine()
{
}

void FSymbolTable::Define(std::string_view Name, std::string Type, EKind Kind)
{
}

int FSymbolTable::VarCount(EKind Kind)
{
	return 0;
}

EKind FSymbolTable::KindOf(std::string_view Name)
{
	return EKind::ARG;
}

std::string FSymbolTable::TypeOf(std::string_view Name)
{
	return "";
}

std::optional<FSymbolTable::SymbolMapConstIterator> FSymbolTable::FindEntry(std::string_view Name)
{
	const std::string NameStr = std::string(Name);
	auto			  ItSub = SubSymTable.find(NameStr);
	if (ItSub != SubSymTable.end())
	{
		return std::optional(ItSub);
	}

	auto ItrClass = ClassSymTable.find(NameStr);
	if (ItrClass != ClassSymTable.end())
	{
		return std::optional(ItrClass);
	}

	return std::nullopt; // "Not found"
}

int FSymbolTable::IndexOf(std::string_view Name)
{
	auto OptItr = FindEntry(Name);
	return OptItr ? OptItr.value()->second.Index : -1;
}
