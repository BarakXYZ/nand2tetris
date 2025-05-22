// Implemented by BarakXYZ 2025

#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <deque>

/**
 * @brief Enumeration for the different types of tokens.
 */
enum ETokenType
{
	NONE,		 ///< No token.
	COMMENT,	 ///< A comment line.
	KEYWORD,	 ///< A language keyword.
	SYMBOL,		 ///< A symbol character.
	IDENTIFIER,	 ///< An identifier (variable, function name, etc.).
	INT_CONST,	 ///< An integer constant.
	STRING_CONST ///< A string constant.
};

/**
 * @brief A tokenizer for the Jack programming language.
 *
 * This class reads an input file and tokenizes its content according to the Jack language specification.
 */
class FJackTokenizer
{

private:
	std::ifstream InFile;		 ///< Input file stream.
	std::string	  CurrentLine{}; ///< Stores the current line being processed.

public:
	/**
	 * @brief Constructs a new FJackTokenizer object and opens the specified file.
	 *
	 * @param fileName The name of the file to tokenize.
	 */
	FJackTokenizer(const std::string& fileName);

	/**
	 * @brief Destroys the FJackTokenizer object.
	 *
	 * Closes the input file stream if it is open.
	 */
	~FJackTokenizer();

	/**
	 * @brief Initializes the file stream with the specified file.
	 *
	 * Opens the file and verifies that it is accessible and not empty.
	 *
	 * @param fileName The name of the file to open.
	 * @return true if the file was successfully opened and is not empty; false otherwise.
	 */
	bool InitFile(const std::string& fileName);

	/**
	 * @brief Determines if the current line is a comment.
	 *
	 * Checks the first character of the currently stored line.
	 *
	 * @return true if the line is a comment; false otherwise.
	 */
	bool IsCommentLine();

	/**
	 * @brief Determines if a given line is a comment.
	 *
	 * Checks the first character of the provided line.
	 *
	 * @param InLine The line to check.
	 * @return true if the line is a comment; false otherwise.
	 */
	bool IsCommentLine(const std::string_view InLine);

	/**
	 * @brief Advances the tokenizer to the next token.
	 *
	 * Reads the next line from the file (if needed), skips comment lines, and processes the line to extract tokens.
	 * It classifies the token as a keyword, symbol, integer constant, identifier, or string constant.
	 */
	void Advance();

	/**
	 * @brief Retrieves the type of the current token.
	 *
	 * @return The current token's type as an ETokenType.
	 */
	ETokenType TokenType() const;

	/**
	 * @brief Retrieves the current keyword token.
	 *
	 * @return The current keyword as a string view.
	 */
	std::string_view Keyword() const;

	/**
	 * @brief Retrieves the current symbol token.
	 *
	 * @return The current symbol as a character.
	 */
	char Symbol() const;

	/**
	 * @brief Retrieves the current identifier token.
	 *
	 * @return The current identifier as a string view.
	 */
	std::string_view Identifier() const;

	/**
	 * @brief Retrieves the integer constant value of the current token.
	 *
	 * @return The integer value.
	 */
	unsigned int IntVal() const;

	/**
	 * @brief Retrieves the string constant of the current token.
	 *
	 * @return The string constant as a string view.
	 */
	std::string_view StringVal() const;

	/**
	 * @brief Checks whether there are more tokens to process.
	 *
	 * Determines if there are tokens remaining in the current line or if the input file has more content.
	 *
	 * @return true if more tokens are available; false otherwise.
	 */
	bool HasMoreTokens();

	/**
	 * @brief Attempts to parse a token as an integer constant.
	 *
	 * Checks if the token consists entirely of digits and is within the valid range (0 to 32767).
	 *
	 * @param Token The token to parse.
	 * @param OutIntVal Output parameter that will hold the parsed integer value if successful.
	 * @return true if the token is a valid integer constant; false otherwise.
	 */
	bool TryGetIntegerConstant(const std::string_view Token, unsigned int& OutIntVal);

	/**
	 * @brief Checks if the given line is a valid string constant.
	 *
	 * A valid string constant must be enclosed in double quotes and not contain any newlines or extra quotes.
	 *
	 * @param InLine The line to check.
	 * @return true if the line is a valid string constant; false otherwise.
	 */
	bool IsValidStringConstant(const std::string_view InLine);

	/**
	 * @brief Checks if the given token is a valid identifier.
	 *
	 * An identifier must not start with a digit and must consist only of alphanumeric characters or underscores.
	 *
	 * @param Token The token to check.
	 * @return true if the token is a valid identifier; false otherwise.
	 */
	bool IsValidIdentifier(const std::string_view Token);

	/**
	 * @brief Splits a line into individual tokens.
	 *
	 * Processes the input line by stripping single-line comments, splitting by whitespace,
	 * handling string constants, and recognizing symbols. The resulting tokens are stored in a deque.
	 *
	 * @param Token The line to be tokenized.
	 */
	void SplitLineToTokens(const std::string_view Token);

	/**
	 * @brief Returns a human-readable string representation of the current token type.
	 *
	 * @return A string view describing the current token type.
	 */
	const std::string_view GetTokenTypeAsString();

	// Data members for the current token information.
	ETokenType				CurrentTokenType = ETokenType::NONE; ///< The type of the current token.
	std::string				CurrentKeyword;						 ///< The current keyword (if applicable).
	char					CurrentSymbol;						 ///< The current symbol (if applicable).
	std::string				CurrentIdentifier;					 ///< The current identifier (if applicable).
	unsigned int			CurrentIntVal;						 ///< The current integer constant value.
	std::string				CurrentStringVal;					 ///< The current string constant.
	std::deque<std::string> CurrentlyParsedTokens;				 ///< Queue of tokens parsed from the current line.

	// Mapping of specific keywords to their descriptions.
	std::unordered_map<std::string_view, std::string_view> KeywordFuncByKeyword{
		{ "class", "keyword" }, { "constructor", "keyword" }
	};

	// Set of valid keywords in the Jack language.
	const std::unordered_set<std::string_view> KeywordSet{
		"class", "constructor", "function", "method", "field", "static", "var",
		"int", "char", "boolean", "void", "true", "false", "null", "this", "let",
		"do", "if", "else", "while", "return"
	};

	// Set of valid symbol characters in the Jack language.
	const std::unordered_set<char> SymbolSet{
		'{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&',
		'|', '<', '>', '=', '~'
	};
};
