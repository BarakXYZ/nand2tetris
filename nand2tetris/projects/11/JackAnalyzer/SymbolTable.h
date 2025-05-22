// Implemented by BarakXYZ 2025

#pragma once

#include <string>
// TODO: Convert input strings to string_view if possible

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

class FSymbolTable
{
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
	void Define(std::string Name, std::string Type, EKind Kind);

	/*
	 * Returns he number of variables of the given kind already defined in the
	 * current scope.
	 * @Note This is useful for tracking the running indices of variables.
	 **/
	int VarCount(EKind Kind);

	/**
	 * Returns the kind of the named identifiers in the current scope.
	 * If the identifier is unknown in the current scope, returns NONE.
	 */
	EKind KindOf(std::string Name);

	/**
	 * Returns the type of the named identifier in the current scope.
	 */
	std::string TypeOf(std::string Name);

	/**
	 * Returns the index assigned to the named identifier.
	 */
	int IndexOf(std::string Name);
};
