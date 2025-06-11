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
	// else, continue until all tokens are removed from the tokens deque
	if (CurrentlyParsedTokens.empty())
	{
		std::string CursorLine;
		if (!std::getline(InFile >> std::ws, CursorLine)) // Fetch next line
		{
			// CurrentTokenType = ETokenType::NONE;
			std::cout << "Invalid-Line found; Skipping...\n";
			if (HasMoreTokens())
				Advance();
			return;
		}

		if (IsCommentLine(CursorLine)) // Skip comment lines
		{
			// CurrentTokenType = ETokenType::COMMENT;
			std::cout << "Comment-Line found: Skipping...\n";
			if (HasMoreTokens())
				Advance();
			return;
		}

		// We firstly want to check for String Constant, i.e. ("My Str Constant")
		if (IsValidStringConstant(CursorLine))
		{
			CurrentTokenType = ETokenType::STRING_CONST;
			CurrentStringVal = std::move(CursorLine);
			std::cout << "String Constant Found: " << CurrentStringVal << '\n';
			return; // early return
		}

		// For complex inputs like "(~(key = 0))", etc. we need to pre-process;
		// i.e. split the line to valid separated tokens:
		// Convert: (~(key = 0)) To: '(', '~', '(', 'key', '=', '0', ')', ')'
		SplitLineToTokens(CursorLine); // Populates the CurrentlyParsedTokens
	}

	std::string_view CurrentToken = CurrentlyParsedTokens.front();

	if (CurrentToken.size() >= 2 && CurrentToken.front() == '"' && CurrentToken.back() == '"')
	{
		CurrentTokenType = ETokenType::STRING_CONST;
		// Store without quotes
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
		// If could not process any type of token, exit:
		std::cerr << "Error: Could NOT Classify Token: " << CurrentToken << '\n';
		std::exit(EXIT_FAILURE);
	}

	CurrentlyParsedTokens.pop_front();
}

bool FJackTokenizer::IsCommentLine()
{
	const char FirstChar = CurrentLine.front();
	return (FirstChar == '/' || FirstChar == '*');
}
bool FJackTokenizer::IsCommentLine(const std::string_view InLine)
{
	const char FirstChar = InLine.front();
	return (FirstChar == '/' || FirstChar == '*');
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
