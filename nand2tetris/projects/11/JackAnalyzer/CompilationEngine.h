// Implemented by BarakXYZ 2025

#pragma once

#include <fstream>
#include <memory>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <string_view>
#include "JackTokenizer.h"

class FCompilationEngine
{
public:
	/**
	 * @brief Constructs a new FCompilationEngine object.
	 *
	 * Initializes the compilation engine with the specified input file, output file,
	 * and a shared pointer to a FJackTokenizer.
	 * The next routine called must be CompileClass().
	 *
	 * @param InFileName The input file name for the Jack source.
	 * @param OutFileName The output file name for the generated output.
	 * @param JackTokenizer Shared pointer to a FJackTokenizer instance.
	 */
	FCompilationEngine(
		const std::string_view InFileName, const std::string& OutFileName,
		std::shared_ptr<FJackTokenizer> JackTokenizer);

	/**
	 * @brief Destructor for FCompilationEngine.
	 *
	 * Closes the output file if it is open.
	 */
	~FCompilationEngine();

	/**
	 * @brief Opens a new output file entry for writing.
	 *
	 * Opens (and truncates) the specified output file for writing.
	 *
	 * @param fileNameVM The name of the output file.
	 * @return true if the file was successfully opened, false otherwise.
	 */
	auto InitNewEntry(const std::string& fileNameVM) -> bool;

	////////////////////////////////////////////////////////////
	// Compilation routines

	/**
	 * @brief Compiles a complete class.
	 *
	 * Expects the token stream to follow the structure:
	 * 'class' className '{' classVarDec* subroutineDec* '}'.
	 */
	void CompileClass();

	/**
	 * @brief Compiles a static variable declaration or a field declaration.
	 */
	void CompileClassVarDec();

	/**
	 * @brief Compiles a complete method, function, or constructor.
	 *
	 * Expects the subroutine declaration to follow the appropriate syntax.
	 */
	void CompileSubroutineDec();

	/**
	 * @brief Compiles a (possibly empty) parameter list.
	 *
	 * Does not handle the enclosing parentheses "()", only the inner parameter list.
	 */
	void CompileParameterList();

	/**
	 * @brief Compiles a subroutine's body.
	 *
	 * Processes the '{' varDec* statements '}' block.
	 */
	void CompileSubroutineBody();

	/**
	 * @brief Compiles a local variable declaration.
	 *
	 * Processes a 'var' declaration with one or more variable names.
	 */
	void CompileVarDec();

	/**
	 * @brief Compiles a sequence of statements.
	 *
	 * Processes zero or more statements. The enclosing "{" and "}" are not handled here.
	 */
	void CompileStatements();

	////////////////////////////////////////////////////////////
	// Statement compilation routines

	/**
	 * @brief Compiles a let statement.
	 *
	 * Processes a let statement with optional array indexing and an expression assignment.
	 */
	void CompileLet();

	/**
	 * @brief Compiles an if statement.
	 *
	 * Processes an if statement with an optional trailing else clause.
	 */
	void CompileIf();

	/**
	 * @brief Compiles a while statement.
	 */
	void CompileWhile();

	/**
	 * @brief Compiles a do statement.
	 *
	 * Processes a subroutine call statement.
	 */
	void CompileDo();

	/**
	 * @brief Compiles a return statement.
	 *
	 * Processes a return statement with an optional expression.
	 */
	void CompileReturn();

	////////////////////////////////////////////////////////////
	// Expression compilation routines

	/**
	 * @brief Compiles an expression.
	 *
	 * Processes an expression consisting of one term possibly followed by
	 * one or more operator-term pairs.
	 */
	void CompileExpression();

	/**
	 * @brief Compiles a term.
	 *
	 * Processes a term, which may be an integer constant, string constant, keyword,
	 * variable, array entry, subroutine call, parenthesized expression, or a unary operation.
	 * If the current token is an identifier, a look-ahead token is used to distinguish
	 * between a variable, an array entry, or a subroutine call.
	 */
	void CompileTerm();

	/**
	 * @brief Compiles a comma-separated list of expressions.
	 *
	 * Processes a (possibly empty) list of expressions separated by commas.
	 */
	void CompileExpressionList();

	////////////////////////////////////////////////////////////
	// Helper functions

	/**
	 * @brief Advances the tokenizer to the next token.
	 *
	 * If additional tokens are available, advances the tokenizer and increments
	 * the internal advance counter. Otherwise, logs an error and exits the program.
	 */
	void TryAdvanceTokenizer();

	/**
	 * @brief Outputs a symbol token to the output file.
	 *
	 * Checks that the current token is a symbol and matches the expected symbol.
	 *
	 * @param Symbol The expected symbol character.
	 */
	void OutputSymbol(char Symbol, bool bShouldAdvanceFile = true);

	/**
	 * @brief Outputs a symbol token (as a string) to the output file.
	 *
	 * Formats and writes the provided symbol string.
	 *
	 * @param Symbol The symbol string to output.
	 */
	void OutputSymbol(const std::string_view Symbol);

	/**
	 * @brief Outputs the current identifier token to the output file.
	 *
	 * Expects the current token to be an identifier and writes it with proper formatting.
	 */
	void CompileIdentifier(const std::string_view IdentifierCategory = "None", bool bIsDeclared = false);

	/**
	 * @brief Outputs the provided identifier as a token.
	 *
	 * This override is used when a saved identifier is needed (and the current token
	 * is already the active one).
	 *
	 * @param Identifier The identifier string to output.
	 */
	void CompileIdentifier(const std::string_view Identifier, const std::string_view IdentifierCategory);

	/**
	 * @brief Outputs a keyword token to the output file.
	 *
	 * Formats and writes the keyword with the appropriate XML tags.
	 *
	 * @param Keyword The keyword to output.
	 */
	void OutputKeyword(const std::string_view Keyword);

	/**
	 * @brief Adjusts the current indentation level.
	 *
	 * Increases the indentation if @a bIndentDeeper is true; otherwise, decreases it.
	 *
	 * @param bIndentDeeper If true, increases the indentation level; if false, decreases it.
	 */
	void AdjustIndentation(bool bIndentDeeper);

	/**
	 * @brief Outputs the current indentation to the output file.
	 *
	 * Writes a number of indentation strings based on the current indentation level.
	 */
	void OutputIndentation()
	{
		for (int i = 0; i < IndentLevel; ++i)
		{
			OutFile << IndentInst;
		}
	}

	/**
	 * @brief Increases the indentation level by one.
	 */
	void IncIndent() { ++IndentLevel; }

	/**
	 * @brief Decreases the indentation level by one.
	 *
	 * Ensures that the indentation level does not go negative.
	 */
	void DecIndent()
	{
		if (IndentLevel > 0)
			--IndentLevel;
	}

	/**
	 * @brief Increments the internal advance counter.
	 */
	void IncAdvance() { ++AdvanceCounter; }

	/**
	 * @brief Outputs a type token to the output file.
	 *
	 * Processes a type which can be a keyword ('int', 'char', or 'boolean') or a class name (identifier).
	 *
	 * @return true if a valid type token was output; false otherwise.
	 */
	bool OutputType();

	/**
	 * @brief Outputs a comma-separated list of variable names.
	 *
	 * Processes and outputs each variable name preceded by a comma.
	 */
	void OutputCommaSeparatedVarNames();

	/**
	 * @brief Compiles a subroutine call.
	 *
	 * Processes a subroutine call which may be in one of the two forms:
	 * - subroutineName '(' expressionList ')'
	 * - (className | varName) '.' subroutineName '(' expressionList ')'
	 *
	 * @param Identifier The initial identifier in the subroutine call.
	 */
	void CompileSubroutineCall(const std::string_view Identifier);

private:
	/// Output file stream used for writing the compiled output.
	std::ofstream OutFile;
	/// Shared pointer to the Jack tokenizer.
	std::shared_ptr<FJackTokenizer> Tokenizer;
	/// A constant string used for one level of indentation.
	static constexpr std::string_view IndentInst{ "  " };
	/// Current indentation level.
	unsigned int IndentLevel{ 0 };
	/// Counter for the number of times the tokenizer was advanced.
	unsigned long long AdvanceCounter{ 0 };

	/// Set of valid keywords for type declarations.
	static const std::unordered_set<std::string_view> ValidTypeKeywords;

	/// Beginning tag for an identifier.
	static constexpr std::string_view IdBegin = "<identifier> ";
	/// Ending tag for an identifier.
	static constexpr std::string_view IdEnd = " </identifier>\n";

	static constexpr std::string_view SymBegin = "<symbol> ";
	static constexpr std::string_view SymEnd = " </symbol>\n";

	// Identifier Categories
	static constexpr std::string_view CatVar = "var";
	static constexpr std::string_view CatArg = "argument";
	static constexpr std::string_view CatStatic = "static";
	static constexpr std::string_view CatField = "field";
	static constexpr std::string_view CatClass = "class";
	static constexpr std::string_view CatSub = "subroutine";
};
