// Implemented by BarakXYZ 2025

#include "CompilationEngine.h"
#include <iostream>
#include "SymbolTable.h"

const std::unordered_set<std::string_view> FCompilationEngine::ValidTypeKeywords = {
	"int", "char", "boolean"
};

FCompilationEngine::FCompilationEngine(
	const std::string_view InFileName, const std::string& OutFileName,
	std::shared_ptr<FJackTokenizer> JackTokenizer)
{
	InitNewEntry(OutFileName);
	Tokenizer = JackTokenizer;
};

FCompilationEngine::~FCompilationEngine()
{
	if (OutFile.is_open())
		OutFile.close();
}

bool FCompilationEngine::InitNewEntry(const std::string& outFileName)
{
	// Create (open) the file for writing
	OutFile.open(outFileName, std::ios::trunc);
	if (!OutFile)
	{
		std::cerr << "Error opening output file: " << outFileName << std::endl;
		return false;
	}
	return true;
}

void FCompilationEngine::CompileClass()
{
	static constexpr std::string_view ClassBegin{ "<class>\n" };
	static constexpr std::string_view ClassEnd{ "</class>" };

	/** 'class' className '{' classVarDec* subroutineDec* '}' */
	TryAdvanceTokenizer();

	// Expect: 'class'
	if (Tokenizer->TokenType() == KEYWORD && Tokenizer->Keyword() == "class")
	{
		OutFile << ClassBegin;
		IncIndent();
		OutputKeyword("class");
	}
	else
	{
		std::cerr << "Expected: 'class'\n";
		exit(1);
	}

	// TODO: Add class category
	// Expect: 'className'
	CompileIdentifier("class", true);

	// Expect: '{'
	if (Tokenizer->TokenType() == SYMBOL && Tokenizer->Symbol() == '{')
		OutputSymbol('{');
	else
		return;

	/** Expect: 0 or more Var Declarations */
	static const std::unordered_set<std::string_view> ValidVarKeywordSet = {
		"static", "field"
	};

	while (Tokenizer->TokenType() == KEYWORD
		&& ValidVarKeywordSet.contains(Tokenizer->Keyword()))
	{
		CompileClassVarDec();
	}

	/** Expect: 0 or more Subroutine Declarations */
	static const std::unordered_set<std::string_view> ValidRoutineKeywordSet = {
		"constructor", "function", "method"
	};

	while (Tokenizer->TokenType() == KEYWORD
		&& ValidRoutineKeywordSet.contains(Tokenizer->Keyword()))
	{
		CompileSubroutineDec();
	}

	// Expect: '}'
	if (Tokenizer->TokenType() == SYMBOL && Tokenizer->Symbol() == '}')
		OutputSymbol('}', false /** Don't advance as we may be at the EOF */);

	DecIndent();
	OutFile << ClassEnd;
}

void FCompilationEngine::CompileClassVarDec()
{
	/**
	 * ('static') | ('field') type varName (',' varName)* ';'
	 */
	static constexpr std::string_view VarBegin = "<classVarDec>\n";
	static constexpr std::string_view VarEnd = "</classVarDec>\n";

	OutputIndentation();
	OutFile << VarBegin;
	IncIndent();

	// Expect:  ('static') | ('field') Keywords (checked by CompileClass())
	OutputKeyword(Tokenizer->Keyword());

	// Expect: type (keyword) || className (identifier)
	OutputType();

	// TODO: Add static / field category
	// Expect: varName (identifier)
	CompileIdentifier();

	// Expect: 0 or more comma separated varNames
	OutputCommaSeparatedVarNames();

	// Expect: ';' (Symbol)
	OutputSymbol(';');

	DecIndent();
	OutputIndentation();
	OutFile << VarEnd;
}

void FCompilationEngine::CompileSubroutineDec()
{
	/**
	 * ('static' | 'function' | 'method') ('void' | type) subroutineName
	 * '(' parameterList ')' subroutineBody
	 */
	static constexpr std::string_view SubDecBegin = "<subroutineDec>\n";
	static constexpr std::string_view SubDecEnd = "</subroutineDec>\n";

	OutputIndentation();
	OutFile << SubDecBegin;
	IncIndent();

	// Expect: ('static'|'function'|'method') Keywords (checked by CompileClass())
	OutputKeyword(Tokenizer->Keyword());

	// Expect: ('void' | type)
	static constexpr std::string_view Void = "void";
	if (Tokenizer->Keyword() == Void)
		OutputKeyword(Void);
	else
		OutputType();

	// TODO: Add subroutine category
	// Expect: subroutineName (identifier)
	CompileIdentifier();

	// Expect: '(' (symbol))
	OutputSymbol('(');

	// Expect: parameterList (can be empty)
	CompileParameterList();

	// Expect: ')' (symbol))
	OutputSymbol(')');

	// Expect: subroutineBody
	CompileSubroutineBody();

	DecIndent();
	OutputIndentation();
	OutFile << SubDecEnd;
}

void FCompilationEngine::CompileParameterList()
{
	/** ((type varName)(',' type varName)*)? */
	static constexpr std::string_view ParamBegin = "<parameterList>\n";
	static constexpr std::string_view ParamEnd = "</parameterList>\n";

	OutputIndentation();
	OutFile << ParamBegin;
	IncIndent();

	if (OutputType()) // Check if empty parameterList
	{
		// TODO: Add category "argument"
		// Expect: varName
		CompileIdentifier();

		// Expect: (',' type varName)*
		while (Tokenizer->TokenType() == SYMBOL && Tokenizer->Symbol() == ',')
		{
			OutputSymbol(',');

			OutputType();

			// TODO: Add category "argument"
			CompileIdentifier();
		}
	}

	DecIndent();
	OutputIndentation();
	OutFile << ParamEnd;
}

void FCompilationEngine::CompileSubroutineBody()
{
	/** '{' varDec* statements '}' */

	static constexpr std::string_view SubBodyBegin = "<subroutineBody>\n";
	static constexpr std::string_view SubBodyEnd = "</subroutineBody>\n";

	OutputIndentation();
	OutFile << SubBodyBegin;
	IncIndent();

	// Expect: '{' (symbol))
	OutputSymbol('{');

	// Expect: varDec*
	while (Tokenizer->Keyword() == "var")
		CompileVarDec();

	// Expect: statements
	CompileStatements();

	// Expect: '}' (symbol))
	OutputSymbol('}');

	DecIndent();
	OutputIndentation();
	OutFile << SubBodyEnd;
}

void FCompilationEngine::CompileVarDec()
{
	/** 'var' type varName (','varName)* ';' */

	static constexpr std::string_view VarDecBegin = "<varDec>\n";
	static constexpr std::string_view VarDecEnd = "</varDec>\n";

	OutputIndentation();
	OutFile << VarDecBegin;
	IncIndent();

	// Expect: 'var' (checked by subroutineBody in a while loop)
	OutputKeyword("var");

	// Expect: type
	OutputType();

	// TODO: Add category (var / local)

	// Expect: varName (identifier)
	CompileIdentifier();

	// Expect: (',' varName)*
	OutputCommaSeparatedVarNames();

	// Expect: ';' (symbol)
	OutputSymbol(';');

	DecIndent();
	OutputIndentation();
	OutFile << VarDecEnd;
}

void FCompilationEngine::CompileStatements()
{
	/** statement* */
	// Not sure if they expect to output empty begin & end if no statements

	static constexpr std::string_view StatementsBegin = "<statements>\n";
	static constexpr std::string_view StatementsEnd = "</statements>\n";

	static const std::unordered_map<std::string_view, void (FCompilationEngine::*)()> CompileStatementByKeyword = {
		{ "let", &FCompilationEngine::CompileLet },
		{ "if", &FCompilationEngine::CompileIf },
		{ "while", &FCompilationEngine::CompileWhile },
		{ "do", &FCompilationEngine::CompileDo },
		{ "return", &FCompilationEngine::CompileReturn }
	};

	OutputIndentation();
	OutFile << StatementsBegin;
	IncIndent();

	auto it = (Tokenizer->TokenType() == KEYWORD)
		? CompileStatementByKeyword.find(Tokenizer->Keyword())
		: CompileStatementByKeyword.end();
	while (it != CompileStatementByKeyword.end())
	{
		(this->*(it->second))();
		it = (Tokenizer->TokenType() == KEYWORD)
			? CompileStatementByKeyword.find(Tokenizer->Keyword())
			: CompileStatementByKeyword.end();
	}

	DecIndent();
	OutputIndentation();
	OutFile << StatementsEnd;
}

void FCompilationEngine::CompileLet()
{
	/** 'let' varName ('['expression']')? '=' expression';' */
	static constexpr std::string_view LetBegin = "<letStatement>\n";
	static constexpr std::string_view LetEnd = "</letStatement>\n";

	OutputIndentation();
	OutFile << LetBegin;
	IncIndent();

	// Expect: 'let' (keyword)
	OutputKeyword("let"); // Checked by CompileStatements

	// Expect: varName (identifier)
	CompileIdentifier();

	// Expect: ('['expression']')? -> (i.e. 0 or 1)
	if (Tokenizer->TokenType() == SYMBOL && Tokenizer->Symbol() == '[')
	{
		OutputSymbol('[');

		CompileExpression();

		OutputSymbol(']');
	}

	// Expect: '='
	OutputSymbol('=');

	// Expect: expression
	CompileExpression();

	// Expect: ';'
	OutputSymbol(';');

	DecIndent();
	OutputIndentation();
	OutFile << LetEnd;
}

void FCompilationEngine::CompileIf()
{
	/** 'if' '('expression')' '{' statements '}' ('else' '{' statements '}')? */
	static constexpr std::string_view IfBegin = "<ifStatement>\n";
	static constexpr std::string_view IfEnd = "</ifStatement>\n";

	OutputIndentation();
	OutFile << IfBegin;
	IncIndent();

	// Expect: 'if' (keyword)
	OutputKeyword("if"); // Checked by CompileStatements

	// Expect: '(' expression ')'
	OutputSymbol('(');
	CompileExpression();
	OutputSymbol(')');

	// Expect: '{' statments '}'
	OutputSymbol('{');
	CompileStatements();
	OutputSymbol('}');

	if (Tokenizer->TokenType() == KEYWORD && Tokenizer->Keyword() == "else")
	{
		OutputKeyword("else");

		OutputSymbol('{');
		CompileStatements();
		OutputSymbol('}');
	}

	DecIndent();
	OutputIndentation();
	OutFile << IfEnd;
}

void FCompilationEngine::CompileWhile()
{
	/** 'while' '('expression')' '{' statements '}' */
	static constexpr std::string_view WhileBegin = "<whileStatement>\n";
	static constexpr std::string_view WhileEnd = "</whileStatement>\n";

	OutputIndentation();
	OutFile << WhileBegin;
	IncIndent();

	// Expect: 'while' (keyword)
	OutputKeyword("while"); // Checked by CompileStatements

	// Expect: '(' expression ')'
	OutputSymbol('(');
	CompileExpression();
	OutputSymbol(')');

	// Expect: '{' statments '}'
	OutputSymbol('{');
	CompileStatements();
	OutputSymbol('}');

	DecIndent();
	OutputIndentation();
	OutFile << WhileEnd;
}

void FCompilationEngine::CompileDo()
{
	/** 'do' subroutineCall';' */
	static constexpr std::string_view DoBegin = "<doStatement>\n";
	static constexpr std::string_view DoEnd = "</doStatement>\n";

	OutputIndentation();
	OutFile << DoBegin;
	IncIndent();

	// Expect: 'do' (keyword)
	OutputKeyword("do"); // Checked by CompileStatements

	std::string Identifier = std::string(Tokenizer->Identifier());
	TryAdvanceTokenizer();
	CompileSubroutineCall(Identifier);
	OutputSymbol(';');

	DecIndent();
	OutputIndentation();
	OutFile << DoEnd;
}

void FCompilationEngine::CompileReturn()
{
	/** 'return' expression? ';' */
	static constexpr std::string_view ReturnBegin = "<returnStatement>\n";
	static constexpr std::string_view ReturnEnd = "</returnStatement>\n";

	OutputIndentation();
	OutFile << ReturnBegin;
	IncIndent();

	// Expect: 'return' (keyword)
	OutputKeyword("return"); // Checked by CompileStatements

	// if we're directly at ';' it means no prior expression.
	// Else expression then ';'
	if (Tokenizer->TokenType() == SYMBOL && Tokenizer->Symbol() == ';')
		OutputSymbol(';');
	else
	{
		CompileExpression();
		OutputSymbol(';'); // Tokenizer is advanced
	}

	DecIndent();
	OutputIndentation();
	OutFile << ReturnEnd;
}

void FCompilationEngine::CompileExpression()
{
	/** term(op term)* */
	static constexpr std::string_view ExpressionBegin = "<expression>\n";
	static constexpr std::string_view ExpressionEnd = "</expression>\n";

	OutputIndentation();
	OutFile << ExpressionBegin;
	IncIndent();

	// Expect: 1 term
	CompileTerm();

	static const std::unordered_set<char> ValidOpSet = {
		'+', '-', '*', '/', '&', '|', '<', '>', '='
	};

	static const std::unordered_map<char, std::string_view> SpecialOpMap = {
		{ '&', "&amp;" },
		{ '<', "&lt;" },
		{ '>', "&gt;" }
	};

	// Expect: (op term)* (i.e. 0 or more)
	while (Tokenizer->TokenType() == SYMBOL
		&& ValidOpSet.contains(Tokenizer->Symbol()))
	{
		const char Symbol = Tokenizer->Symbol();
		const auto SpecialIt = SpecialOpMap.find(Symbol);
		// Either use the special symbol map or directly the Symbol
		if (SpecialIt == SpecialOpMap.end())
			OutputSymbol(Symbol);
		else
			OutputSymbol(SpecialIt->second);

		CompileTerm();
	}

	DecIndent();
	OutputIndentation();
	OutFile << ExpressionEnd;
}

void FCompilationEngine::CompileTerm()
{
	/** integerConstant | stringConstant | keywordConstant | varName
	 * | varName'[' expression ']' | subroutineCall | '(' expression ')'
	 * | unaryOp term
	 */

	static constexpr std::string_view TermBegin = "<term>\n";
	static constexpr std::string_view TermEnd = "</term>\n";

	OutputIndentation();
	OutFile << TermBegin;
	IncIndent();

	const ETokenType TT = Tokenizer->TokenType();
	switch (TT)
	{
		case INT_CONST:
		{
			static constexpr std::string_view IntBegin = "<integerConstant> ";
			static constexpr std::string_view IntEnd = " </integerConstant>\n";

			OutputIndentation();
			OutFile << IntBegin << Tokenizer->IntVal() << IntEnd;
			TryAdvanceTokenizer();
			break;
		}
		case STRING_CONST:
		{
			static constexpr std::string_view StrBegin = "<stringConstant> ";
			static constexpr std::string_view StrEnd = " </stringConstant>\n";

			OutputIndentation();
			OutFile << StrBegin << Tokenizer->StringVal() << StrEnd;
			TryAdvanceTokenizer();
			break;
		}
		case KEYWORD:
		{
			OutputKeyword(Tokenizer->Keyword());
			break;
		}
		case IDENTIFIER:
		{
			const std::string Identifier = std::string(Tokenizer->Identifier());
			// We need to look 1 more token ahead to determine the compilation
			TryAdvanceTokenizer();
			if (Tokenizer->TokenType() == SYMBOL)
			{
				const char Symbol = Tokenizer->Symbol();
				if (Symbol == '[') // Array Entry
				{
					/** varName '[' expression ']' */
					CompileIdentifier(Identifier);
					OutputSymbol('[');
					CompileExpression();
					OutputSymbol(']');
				}
				else if (Symbol == '(' || Symbol == '.') // Check if SubCall
					CompileSubroutineCall(Identifier);
				else
					CompileIdentifier(Identifier); // varName (identifier);
			}
			else
				CompileIdentifier(Identifier); // varName (identifier);

			break;
		}
		case SYMBOL:
		{
			const char Symbol = Tokenizer->Symbol();
			if (Symbol == '(')
			{
				OutputSymbol('(');
				CompileExpression();
				OutputSymbol(')');
			}
			// Handle unaryOp
			else if (Symbol == '-' || Symbol == '~')
			{
				OutputSymbol(Symbol);
				CompileTerm();
			}
			break;
		}
		default:
			break;
	}

	DecIndent();
	OutputIndentation();
	OutFile << TermEnd;
}

void FCompilationEngine::CompileExpressionList()
{
	/** (expression(','expression)* )? */
	static constexpr std::string_view ExpListBegin = "<expressionList>\n";
	static constexpr std::string_view ExpListEnd = "</expressionList>\n";

	OutputIndentation();
	OutFile << ExpListBegin;
	IncIndent();

	// Check it's not an empty Expression List
	if (!(Tokenizer->TokenType() == SYMBOL && Tokenizer->Symbol() == ')'))
	{
		CompileExpression();

		while (Tokenizer->TokenType() == SYMBOL && Tokenizer->Symbol() == ',')
		{
			OutputSymbol(',');
			CompileExpression();
		}
	}

	DecIndent();
	OutputIndentation();
	OutFile << ExpListEnd;
}

void FCompilationEngine::CompileSubroutineCall(const std::string_view Identifier)
{
	/**
	 * subroutineName '(' expressionList ')' |
	 *  (className | varName)'.'subroutineName '(' expressionList ')'
	 */

	const char Symbol = Tokenizer->Symbol();

	if (Symbol == '(')
	{
		// Category: Argument
		/** subroutineName '(' expressionList ')' */
		CompileIdentifier(Identifier, "argument"); // subroutineName (id)
		OutputSymbol('(');
		CompileExpressionList();
		OutputSymbol(')');
	}

	/** (className | varName)'.'subroutineName '('expressionList')' */
	else if (Symbol == '.')
	{
		CompileIdentifier(Identifier); // (className | varName)

		OutputSymbol('.');
		CompileIdentifier("subroutine");

		OutputSymbol('(');
		CompileExpressionList();
		OutputSymbol(')'); // ')'
	}
}

void FCompilationEngine::TryAdvanceTokenizer()
{
	if (Tokenizer->HasMoreTokens())
	{
		Tokenizer->Advance();
		IncAdvance();
	}
	else
	{
		std::cout << "TryAdvanceTokenizer: Has No Tokens left... Exiting...";
		if (OutFile.is_open())
			OutFile.close();
		exit(0);
	}
}

void FCompilationEngine::OutputSymbol(char Symbol, bool bShouldAdvanceFile)
{
	if (Tokenizer->TokenType() != SYMBOL)
		std::cerr << "OutputSymbol: Expected Type Symbol, got: "
				  << Tokenizer->GetTokenTypeAsString() << '\n';
	else if (Tokenizer->Symbol() != Symbol)
		std::cerr
			<< "OutputSymbol: Expected Symbol: " << Symbol
			<< " got: " << Tokenizer->Symbol() << '\n';
	else
	{
		OutputIndentation();
		OutFile << SymBegin << Symbol << SymEnd;
		if (bShouldAdvanceFile)
			TryAdvanceTokenizer();
	}
}

void FCompilationEngine::OutputSymbol(const std::string_view Symbol)
{
	OutputIndentation();
	OutFile << SymBegin << Symbol << SymEnd;
	TryAdvanceTokenizer();
}

void FCompilationEngine::CompileIdentifier(const std::string_view IdentifierCategory, bool bIsDeclared)
{
	if (Tokenizer->TokenType() == IDENTIFIER)
	{
		OutputIndentation();
		OutFile << "<identifier>\n";
		IncIndent();

		// Always output name
		OutputIndentation();
		OutFile << "<name> " << Tokenizer->Identifier() << " </name>\n";

		// Always output category
		OutputIndentation();
		OutFile << "<category> " << IdentifierCategory << " </category>\n";

		// If it's a variable type, look up and output index
		if (IdentifierCategory == "var" || IdentifierCategory == "argument" || IdentifierCategory == "static" || IdentifierCategory == "field")
		{
			const int index = FSymbolTable::IndexOf(Tokenizer->Identifier());
			OutputIndentation();
			OutFile << "<index> " << index << " </index>\n";
		}

		// Always output usage
		OutputIndentation();
		OutFile << "<usage> " << (bIsDeclared ? "defined" : "used") << " </usage>\n";

		DecIndent();
		OutputIndentation();
		OutFile << "</identifier>\n";

		TryAdvanceTokenizer();
	}
}

void FCompilationEngine::CompileIdentifier(const std::string_view Identifier, const std::string_view IdentifierCategory)
{
	// Don't check in this override. Checking is done manually by the caller.
	// Also, we won't advance in this override as we're 1 token ahead.
	OutputIndentation();
	OutFile << IdBegin << Identifier << IdEnd;
}

void FCompilationEngine::OutputKeyword(const std::string_view Keyword)
{
	static constexpr std::string_view KeyBegin = "<keyword> ";
	static constexpr std::string_view KeyEnd = " </keyword>\n";

	OutputIndentation();
	OutFile << KeyBegin << Keyword << KeyEnd;
	TryAdvanceTokenizer();
}

bool FCompilationEngine::OutputType()
{
	/** 'int'|'char'|'boolean'|className */
	const std::string_view Keyword = Tokenizer->Keyword();
	const ETokenType	   TT = Tokenizer->TokenType();
	if (TT == KEYWORD && ValidTypeKeywords.contains(Keyword))
	{
		OutputKeyword(Keyword);
		return true;
	}
	else if (TT == IDENTIFIER) // This covers className?
	{
		CompileIdentifier();
		return true;
	}
	else
		return false;
}

void FCompilationEngine::OutputCommaSeparatedVarNames()
{
	while (Tokenizer->Symbol() == ',')
	{
		OutputSymbol(',');

		// Expect: varName (identifier)
		CompileIdentifier();
	}
}
