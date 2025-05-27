// Implemented by BarakXYZ 2025

#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <optional>
#include <unordered_map>
#include <utility>

/**
 * Implementation Notes:
 * The symbol table abstraction can be implemented using two separate hash tables:
 * One for the class scope, and a second one for the subroutine scope.
 *
 * When we start compiling a new subroutine, the latter hash table can be reset.
 *
 * When compiling an error-free Jack code, each symbol not found in the symbol
 * tables can be assumed to be either a subroutine name or a class name.
 */

/**
 * Enumeration for the different kinds of variables.
 */
enum EKind
{
	// Class kinds
	STATIC,
	FIELD,

	// Subroutine kinds
	ARG,
	VAR,
};

struct FIdentifierDetails
{
	std::string Name;
	std::string Type;
	EKind		Kind;
	int			Index;
};

class FSymbolTable
{
	using SymbolMap = std::unordered_map<std::string, FIdentifierDetails>;
	using SymbolMapConstIterator = SymbolMap::const_iterator;

	void Reset();

	/**
	 * Starts a new subroutine scope (i.e., resets the subroutine's symbol table)
	 */
	void StartSubroutine();

	/**
	 * Defines a new identifier of the given name, type, and kind, and assigns
	 * it a running index.
	 * STATIC and FIELD identifiers have a *class* scope, while ARG and VAR
	 * identifiers have a *subroutine* scope.
	 */
	void Define(std::string_view Name, std::string Type, EKind Kind);

	/*
	 * Returns the number of variables of the given kind already defined in the
	 * current scope.
	 * @Note This is useful for tracking the running indices of variables.
	 **/
	int VarCount(EKind Kind);

	/**
	 * Returns the kind of the named identifier in the current scope.
	 * If the identifier is unknown in the current scope, returns NONE.
	 */
	EKind KindOf(std::string_view Name);

	/**
	 * Returns the type of the named identifier in the current scope.
	 */
	std::string TypeOf(std::string_view Name);

	/**
	 * Returns the index assigned to the named identifier.
	 */
	int IndexOf(const std::string& Name);

	/**
	 * Finds an identifier by name.
	 * @param Name The name to find.
	 * @return An optional pair containing the iterator and a pointer to its owning map.
	 * std::nullopt if not found.
	 */
	std::optional<SymbolMapConstIterator> FindEntry(const std::string& Name) const;

	std::unordered_map<std::string, FIdentifierDetails> ClassSymTable;
	std::unordered_map<std::string, FIdentifierDetails> SubSymTable;
};
