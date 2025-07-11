// Implemented by BarakXYZ 2025

#include "JackTokenizer.h"
#include <algorithm>
#include <cctype>
#include <iostream>

FJackTokenizer::FJackTokenizer(const std::string& fileName)
{
	if (!InitFile(fileName))
		std::exit(EXIT_FAILURE);
}

FJackTokenizer::~FJackTokenizer()
{
	if (InFile.is_open())
		InFile.close();
}

bool FJackTokenizer::InitFile(const std::string& fileName)
{
	InFile.open(fileName);
	if (!InFile.is_open())
	{
		std::cerr << "Error opening input file: " << fileName << std::endl;
		return false;
	}
	if (InFile.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << "File is empty: " << fileName << std::endl;
		InFile.close();
		return false;
	}
	return true;
}

bool FJackTokenizer::HasMoreTokens()
{
	return !CurrentlyParsedTokens.empty()
		|| InFile.peek() != std::ifstream::traits_type::eof();
}

void FJackTokenizer::Advance()
{
	// If we have no tokens left from the previous line fetched, get the next
	// line and process its tokens.
	if (CurrentlyParsedTokens.empty())
	{
		std::string CursorLine;

		// Keep reading lines until we find a non-comment line
		while (std::getline(InFile >> std::ws, CursorLine))
		{
			// Handle multi-line comments
			if (InMultiLineComment)
			{
				// Look for the end of multi-line comment
				size_t endPos = CursorLine.find("*/");
				if (endPos != std::string::npos)
				{
					InMultiLineComment = false;
					// Continue processing the rest of the line after */
					CursorLine = CursorLine.substr(endPos + 2);
					// If the rest of the line is empty or just whitespace, continue to next line
					if (CursorLine.find_first_not_of(" \t\r\n") == std::string::npos)
						continue;
				}
				else
					// Still inside multi-line comment, skip this entire line
					continue;
			}

			// Check for start of multi-line comment
			size_t multiLineStart = CursorLine.find("/*");
			if (multiLineStart != std::string::npos)
			{
				// Check if the comment ends on the same line
				size_t multiLineEnd = CursorLine.find("*/", multiLineStart + 2);
				if (multiLineEnd != std::string::npos)
				{
					// Single-line /* ... */ comment
					// Remove the comment portion and continue with the rest
					std::string beforeComment = CursorLine.substr(0, multiLineStart);
					std::string afterComment = CursorLine.substr(multiLineEnd + 2);
					CursorLine = beforeComment + afterComment;
					// If nothing left, continue to next line
					if (CursorLine.find_first_not_of(" \t\r\n") == std::string::npos)
					{
						continue;
					}
				}
				else
				{
					// Multi-line comment starts but doesn't end on this line
					InMultiLineComment = true;
					// Process any content before the comment start
					CursorLine = CursorLine.substr(0, multiLineStart);
					if (CursorLine.find_first_not_of(" \t\r\n") == std::string::npos)
						continue;
				}
			}

			// Skip single-line comments that start at the beginning
			if (CursorLine.size() >= 2 && CursorLine[0] == '/' && CursorLine[1] == '/')
			{
				std::cout << "Comment-Line found: Skipping...\n";
				continue;
			}

			// We have a valid line to process
			break;
		}

		// If we couldn't get a valid line
		if (CursorLine.empty() && !InFile)
		{
			std::cout << "No more valid lines to process\n";
			return;
		}

		// Check for String Constant
		if (IsValidStringConstant(CursorLine))
		{
			CurrentTokenType = ETokenType::STRING_CONST;
			CurrentStringVal = std::move(CursorLine);
			std::cout << "String Constant Found: " << CurrentStringVal << '\n';
			return;
		}

		// Split the line into tokens
		SplitLineToTokens(CursorLine);
	}

	// Process tokens from the queue (rest of the method remains the same)
	std::string_view CurrentToken = CurrentlyParsedTokens.front();

	if (CurrentToken.size() >= 2 && CurrentToken.front() == '"' && CurrentToken.back() == '"')
	{
		CurrentTokenType = ETokenType::STRING_CONST;
		CurrentStringVal = CurrentToken.substr(1, CurrentToken.size() - 2);
		std::cout << "String Constant Found: " << CurrentStringVal << '\n';
	}
	else if (KeywordSet.contains(CurrentToken))
	{
		CurrentTokenType = ETokenType::KEYWORD;
		CurrentKeyword = CurrentToken;
		std::cout << "Found Keyword: " << CurrentToken << '\n';
	}
	else if (CurrentToken.length() == 1 && SymbolSet.contains(CurrentToken[0]))
	{
		CurrentTokenType = ETokenType::SYMBOL;
		CurrentSymbol = CurrentToken[0];
		std::cout << "Found Symbol: " << CurrentToken << '\n';
	}
	else if (TryGetIntegerConstant(CurrentToken, CurrentIntVal))
	{
		std::cout << "Integer Constant Found: " << CurrentToken << '\n';
		CurrentTokenType = ETokenType::INT_CONST;
	}
	else if (IsValidIdentifier(CurrentToken))
	{
		CurrentIdentifier = CurrentToken;
		CurrentTokenType = ETokenType::IDENTIFIER;
		std::cout << "Identifier Found: " << CurrentToken << '\n';
	}
	else
	{
		std::cerr << "Error: Could NOT Classify Token: " << CurrentToken << '\n';
		std::exit(EXIT_FAILURE);
	}

	CurrentlyParsedTokens.pop_front();
}

// DEPRECATED:
// This method is now less useful since it doesn't handle multi-line comments
// Only check for single-line comments starting with //
bool FJackTokenizer::IsCommentLine(const std::string_view InLine)
{
	return InLine.size() >= 2 && InLine[0] == '/' && InLine[1] == '/';
}

ETokenType FJackTokenizer::TokenType() const
{
	return CurrentTokenType;
}

std::string_view FJackTokenizer::Keyword() const
{
	return CurrentKeyword;
}

char FJackTokenizer::Symbol() const
{
	return CurrentSymbol;
}

std::string_view FJackTokenizer::Identifier() const
{
	return CurrentIdentifier;
}

unsigned int FJackTokenizer::IntVal() const
{
	return CurrentIntVal;
}

std::string_view FJackTokenizer::StringVal() const
{
	return CurrentStringVal;
}

void FJackTokenizer::SplitLineToTokens(std::string_view InLine)
{
	// Strip single-line comment
	size_t commentPos = InLine.find("//");
	if (commentPos != std::string::npos)
	{
		InLine = InLine.substr(0, commentPos);
	}

	CurrentlyParsedTokens.clear();
	std::string token;

	// Iterate over characters in InLine
	for (size_t i = 0; i < InLine.size(); ++i)
	{
		char c = InLine[i];

		// Skip whitespace; if we already built up a token, push it first
		if (std::isspace(static_cast<unsigned char>(c)))
		{
			if (!token.empty())
			{
				CurrentlyParsedTokens.push_back(std::move(token));
				token.clear();
			}
			continue;
		}

		// If we hit a double quote, read everything up to the next quote
		if (c == '"')
		{
			// First, if we already have a token building up, push it
			if (!token.empty())
			{
				CurrentlyParsedTokens.push_back(std::move(token));
				token.clear();
			}

			// We'll store the entire quoted token, including the outer quotes
			std::string stringToken;
			stringToken.push_back('"');

			// Move past the first quote and read until the next quote (or end)
			++i;
			while (i < InLine.size() && InLine[i] != '"')
			{
				stringToken.push_back(InLine[i]);
				++i;
			}
			// If we stopped because of a quote, include it
			if (i < InLine.size() && InLine[i] == '"')
			{
				stringToken.push_back('"');
			}
			else
			{
				// Invalid string.
			}

			// We should have a full quoted string as one token
			CurrentlyParsedTokens.push_back(std::move(stringToken));
			continue;
		}

		// If c is one of our recognized single-character symbols
		if (SymbolSet.contains(c))
		{
			// First push any token we were building
			if (!token.empty())
			{
				CurrentlyParsedTokens.push_back(std::move(token));
				token.clear();
			}
			// Then push this single-character symbol
			CurrentlyParsedTokens.emplace_back(1, c);
		}
		else
		{
			// Part of an identifier/number/etc., so just accumulate
			token.push_back(c);
		}
	}

	// Push last token if any leftover
	if (!token.empty())
	{
		CurrentlyParsedTokens.push_back(std::move(token));
		token.clear();
	}
}

bool FJackTokenizer::TryGetIntegerConstant(const std::string_view Token, unsigned int& OutIntVal)
{
	if (Token.empty())
		return false; // Empty string is invalid

	// Ensure all characters are digits
	if (!std::all_of(Token.begin(), Token.end(), ::isdigit))
		return false;

	try
	{
		size_t Pos = 0;
		int	   Num = std::stoi(std::string(Token), &Pos); // string needed

		// Ensure the entire string was parsed (prevents "123abc" cases)
		if (Pos != Token.size())
			return false;

		if (Num >= 0 && Num <= 32767)
		{
			OutIntVal = Num;
			return true;
		}
		return false;
	}
	catch (const std::exception&)
	{
		return false; // Handles out-of-range and invalid conversions
	}
}

bool FJackTokenizer::IsValidStringConstant(const std::string_view InLine)
{
	// Has at least 2 characters.
	if (InLine.length() < 2)
		return false;

	// Should start & end with '"' (e.g. "My Valid String")
	else if (!InLine.starts_with('"') || !InLine.ends_with('"'))
		return false;

	// Should not contain new lines or double quotes (e.g. "My, "Invalid" Str\n")
	return std::none_of(InLine.begin(), InLine.end(), [](char C) {
		return C == '"' || C == '\n';
	});
}

bool FJackTokenizer::IsValidIdentifier(const std::string_view Token)
{ // At this point guaranteed to not be empty
	// Ensure doesn't start with a digit
	if (std::isdigit(Token[0]))
		return false;

	// Ensure all characters are letters, digits, or underscores
	return std::all_of(Token.begin(), Token.end(), [](char C) {
		return std::isalnum(C) || C == '_';
	});
}

const std::string_view FJackTokenizer::GetTokenTypeAsString()
{
	static const std::unordered_map<ETokenType, std::string_view> TokenTypeAsStringMap = {
		{ ETokenType::SYMBOL, "Symbol" },
		{ ETokenType::KEYWORD, "Keyword" },
		{ ETokenType::IDENTIFIER, "Identifier" },
		{ ETokenType::INT_CONST, "Integer Constant" },
		{ ETokenType::STRING_CONST, "String Constant" }
	};

	if (auto TokenType = TokenTypeAsStringMap.find(CurrentTokenType);
		TokenType != TokenTypeAsStringMap.end())
		return TokenType->second;

	return "Unknown";
}
