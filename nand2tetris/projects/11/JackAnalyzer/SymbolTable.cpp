// Implemented by BarakXYZ 2025

#include "SymbolTable.h"

void FSymbolTable::Reset(ESymbolTableType SymTableType)
{
	SymTableMap.clear();
}

void FSymbolTable::StartSubroutine()
{
}

void FSymbolTable::Define(std::string Name, std::string Type, EKind Kind)
{
	VarTrackers.IncVarCount(Kind); // Tracked as 1 index, printed as 0 index
	SymTableMap.emplace(std::pair(Name,
		FIdentifierDetails(Name, Type, Kind, VarTrackers.GetVarCount(Kind) - 1)));
}

// We don't really need this if we're using the FVarCounters Struct... -_-
int FSymbolTable::VarCount(EKind Kind)
{
	return -1;
}

EKind FSymbolTable::KindOf(std::string_view Name)
{
	return EKind::ARG;
}

std::string FSymbolTable::TypeOf(std::string_view Name)
{
	return "";
}

int FSymbolTable::IndexOf(std::string_view Name)
{
	auto OptItr = FindEntry(Name);
	return OptItr ? OptItr.value()->second.Index : -1;
}

std::optional<FSymbolTable::SymbolMapConstIterator> FSymbolTable::FindEntry(std::string_view Name)
{
	const std::string NameStr = std::string(Name);
	auto			  ItSub = SymTableMap.find(NameStr);
	return ItSub != SymTableMap.end() ? std::optional(ItSub) : std::nullopt;
}

void FVarCounters::IncVarCount(EKind Kind)
{
	switch (Kind)
	{
		case EKind::ARG:
		{
			++ArgCount;
			break;
		}
		case EKind::FIELD:
		{
			++FieldCount;
			break;
		}
		case EKind::STATIC:
		{
			++StaticCount;
			break;
		}
		case EKind::VAR:
		{
			++VarCount;
			break;
		}
	}
}
int FVarCounters::GetVarCount(EKind Kind)
{
	switch (Kind)
	{
		case EKind::ARG:
			return ArgCount;

		case EKind::FIELD:
			return FieldCount;

		case EKind::STATIC:
			return StaticCount;

		case EKind::VAR:
			return VarCount;
	}
	return -1;
}

void FVarCounters::Reset()
{
	ArgCount = 0;
	FieldCount = 0;
	StaticCount = 0;
	VarCount = 0;
}
