// Implemented by BarakXYZ 2025

#include "CompilationEngine.h"
#include <iostream>
#include "SymbolTable.h"
#include "SymbolTableVisualizer.h"

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
	ClassSymTable = FSymbolTable();

	/** 'class' className '{' classVarDec* subroutineDec* '}' */
	TryAdvanceTokenizer();

	// Expect: 'class'
	if (Tokenizer->TokenType() == ETokenType::KEYWORD && Tokenizer->Keyword() == "class")
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

	// Expect: 'className'
	const std::string ClassName = std::string(Tokenizer->Identifier());
	CompileIdentifier(ClassCategory, EUsage::Defined);

	// Expect: '{'
	if (Tokenizer->TokenType() == ETokenType::SYMBOL && Tokenizer->Symbol() == '{')
		OutputSymbol('{');
	else
		return;

	/** Expect: 0 or more Var Declarations */
	static const std::unordered_set<std::string_view> ValidVarKeywordSet = {
		"static", "field"
	};

	while (Tokenizer->TokenType() == ETokenType::KEYWORD
		&& ValidVarKeywordSet.contains(Tokenizer->Keyword()))
	{
		// Only Fields & Statics
		CompileClassVarDec();
	}

	/** Expect: 0 or more Subroutine Declarations */
	static const std::unordered_set<std::string_view> ValidRoutineKeywordSet = {
		"constructor", "function", "method"
	};

	while (Tokenizer->TokenType() == ETokenType::KEYWORD
		&& ValidRoutineKeywordSet.contains(Tokenizer->Keyword()))
	{
		CompileSubroutineDec();
	}

	// Expect: '}'
	if (Tokenizer->TokenType() == ETokenType::SYMBOL && Tokenizer->Symbol() == '}')
		OutputSymbol('}', false /** Don't advance as we may be at the EOF */);

	DecIndent();
	OutFile << ClassEnd;

	PrintSymbolTable(ClassSymTable, ClassName); // Debug
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

	// Store as string as we're going to advance the tokenzier.
	const std::string Keyword = std::string(Tokenizer->Keyword());
	const EKind		  Kind = Keyword == "static"
			  ? EKind::STATIC
			  : EKind::FIELD;
	OutputKeyword(Keyword);

	// Expect: type (keyword) || className (identifier)
	std::string Type = GetType();
	if (!OutputType()) // if this return false, we have an error in the in-file.
	{
		std::cerr << "Type error in CompileClassVarDec!\n";
		return;
	}

	// Expect: varName (identifier) -> At least one.
	ClassSymTable.Define(std::string(Tokenizer->Identifier()), Type, Kind);
	CompileIdentifier(Keyword, EUsage::Defined);

	// Expect: 0 or more comma separated varNames
	OutputCommaSeparatedVarNames(ESymbolTableType::Class, Type, Kind, Keyword, EUsage::Defined);

	// Expect: ';' (Symbol)
	OutputSymbol(';');

	DecIndent();
	OutputIndentation();
	OutFile << VarEnd;
}

void FCompilationEngine::CompileSubroutineDec()
{
	/**
	 ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
	 * '(' parameterList ')' subroutineBody
	 */
	static constexpr std::string_view SubDecBegin = "<subroutineDec>\n";
	static constexpr std::string_view SubDecEnd = "</subroutineDec>\n";

	OutputIndentation();
	OutFile << SubDecBegin;
	IncIndent();

	// Expect: ('constructor'|'function'|'method') Keywords (checked by CompileClass() in a 'while' loop)
	OutputKeyword(Tokenizer->Keyword());

	// Expect: ('void' | type)
	static constexpr std::string_view Void = "void";
	if (Tokenizer->Keyword() == Void)
		OutputKeyword(Void);
	else
		OutputType();

	// Expect: subroutineName (identifier)
	const std::string SubroutineName = std::string(Tokenizer->Identifier());
	SubroutineSymTable = FSymbolTable();
	CompileIdentifier(SubroutineCategory, EUsage::Defined);

	// Expect: '(' (symbol))
	OutputSymbol('(');

	// Expect: parameterList (0 or more)
	// TODO:
	CompileParameterList();

	// Expect: ')' (symbol))
	OutputSymbol(')');

	// Expect: subroutineBody
	CompileSubroutineBody();

	DecIndent();
	OutputIndentation();
	OutFile << SubDecEnd;
	PrintSymbolTable(SubroutineSymTable, SubroutineName); // Debug
}

void FCompilationEngine::CompileParameterList()
{
	/** ((type varName)(',' type varName)*)? */
	static constexpr std::string_view ParamBegin = "<parameterList>\n";
	static constexpr std::string_view ParamEnd = "</parameterList>\n";

	OutputIndentation();
	OutFile << ParamBegin;
	IncIndent();

	std::string Type = GetType();
	if (OutputType()) // Check if empty parameterList
	{
		// Expect: varName
		SubroutineSymTable.Define(std::string(Tokenizer->Identifier()), Type, EKind::ARG);
		CompileIdentifier(ArgumentCategory, EUsage::Defined);

		// Expect: (',' type varName)*
		while (Tokenizer->TokenType() == ETokenType::SYMBOL && Tokenizer->Symbol() == ',')
		{
			OutputSymbol(',');

			Type = GetType();
			OutputType();

			SubroutineSymTable.Define(std::string(Tokenizer->Identifier()), Type, EKind::ARG);
			CompileIdentifier(ArgumentCategory, EUsage::Defined);
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
	const std::string Type = GetType();
	OutputType();

	SubroutineSymTable.Define(std::string(Tokenizer->Identifier()), Type, EKind::VAR);

	// Expect: varName (identifier)
	CompileIdentifier(VariableCategory, EUsage::Defined);

	// Expect: (',' varName)*
	OutputCommaSeparatedVarNames(ESymbolTableType::Subroutine, Type, EKind::VAR, VariableCategory, EUsage::Defined);

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

	auto it = (Tokenizer->TokenType() == ETokenType::KEYWORD)
		? CompileStatementByKeyword.find(Tokenizer->Keyword())
		: CompileStatementByKeyword.end();
	while (it != CompileStatementByKeyword.end())
	{
		(this->*(it->second))();
		it = (Tokenizer->TokenType() == ETokenType::KEYWORD)
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
	// auto Pair = SubroutineSymTable.FindEntry(Tokenizer->Identifier());
	// if (!Pair)
	// 	Pair = ClassSymTable.FindEntry(Tokenizer->Identifier());
	// if (!Pair)
	// {
	// 	std::cerr << "CompileLet() -> Identifier couldn't be found in both Subroutine or Class Symbol Tables.\n";
	// 	exit(10);
	// }
	// CompileIdentifier(KindToString(Pair.value()->second.Kind), EUsage::Used);
	CompileIdentifier(UnknownCategory, EUsage::Used);

	// Expect: ('['expression']')? -> (i.e. 0 or 1)
	if (Tokenizer->TokenType() == ETokenType::SYMBOL && Tokenizer->Symbol() == '[')
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

	if (Tokenizer->TokenType() == ETokenType::KEYWORD && Tokenizer->Keyword() == "else")
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

	// Cache before advancing the Tokenizer
	CachedIdentifier = std::string(Tokenizer->Identifier());
	TryAdvanceTokenizer();
	CompileSubroutineCall();
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
	if (Tokenizer->TokenType() == ETokenType::SYMBOL && Tokenizer->Symbol() == ';')
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
	while (Tokenizer->TokenType() == ETokenType::SYMBOL
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
		case ETokenType::INT_CONST:
		{
			static constexpr std::string_view IntBegin = "<integerConstant> ";
			static constexpr std::string_view IntEnd = " </integerConstant>\n";

			OutputIndentation();
			OutFile << IntBegin << Tokenizer->IntVal() << IntEnd;
			TryAdvanceTokenizer();
			break;
		}
		case ETokenType::STRING_CONST:
		{
			static constexpr std::string_view StrBegin = "<stringConstant> ";
			static constexpr std::string_view StrEnd = " </stringConstant>\n";

			OutputIndentation();
			OutFile << StrBegin << Tokenizer->StringVal() << StrEnd;
			TryAdvanceTokenizer();
			break;
		}
		case ETokenType::KEYWORD:
		{
			OutputKeyword(Tokenizer->Keyword());
			break;
		}
		case ETokenType::IDENTIFIER:
		{
			CachedIdentifier = std::string(Tokenizer->Identifier());
			// We need to look 1 more token ahead to determine the compilation
			TryAdvanceTokenizer();
			if (Tokenizer->TokenType() == ETokenType::SYMBOL)
			{
				const char Symbol = Tokenizer->Symbol();
				if (Symbol == '[') // Array Entry
				{
					/** varName '[' expression ']' */
					CompileIdentifier(CachedIdentifier);
					OutputSymbol('[');
					CompileExpression();
					OutputSymbol(']');
				}
				else if (Symbol == '(' || Symbol == '.') // Check if SubCall
					CompileSubroutineCall();
				else
					CompileIdentifier(CachedIdentifier); // varName (identifier);
			}
			else
				CompileIdentifier(CachedIdentifier); // varName (identifier);

			break;
		}
		case ETokenType::SYMBOL:
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
	if (!(Tokenizer->TokenType() == ETokenType::SYMBOL && Tokenizer->Symbol() == ')'))
	{
		CompileExpression();

		while (Tokenizer->TokenType() == ETokenType::SYMBOL && Tokenizer->Symbol() == ',')
		{
			OutputSymbol(',');
			CompileExpression();
		}
	}

	DecIndent();
	OutputIndentation();
	OutFile << ExpListEnd;
}

void FCompilationEngine::CompileSubroutineCall()
{
	/**
	 * subroutineName '(' expressionList ')' |
	 *  (className | varName)'.'subroutineName '(' expressionList ')'
	 */

	// CachedIdentifier = Identifier;  // We don't really need this...
	// It probably make more sense to cache the Identifier before calling
	// CompileSubroutineCall()
	const char Symbol = Tokenizer->Symbol();

	if (Symbol == '(')
	{
		/** subroutineName '(' expressionList ')' */
		CompileIdentifier(SubroutineCategory, EUsage::Used,
			true /*UseCachedIdentifier*/);
		OutputSymbol('(');
		CompileExpressionList(); // TODO: Look into the ExpressionList setup
		OutputSymbol(')');
	}

	/** (className | varName)'.'subroutineName '('expressionList')' */
	else if (Symbol == '.')
	{
		// TODO: Identify if className or varName by looking up in the SymbolTable?

		// (className | varName)
		CompileIdentifier(ClassCategory, EUsage::Used,
			true /*UseCachedIdentifier*/);

		OutputSymbol('.');
		CompileIdentifier(SubroutineCategory, EUsage::Used);

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
	if (Tokenizer->TokenType() != ETokenType::SYMBOL)
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

std::string FCompilationEngine::GetIdentifierCategory(const std::string Identifier)
{
	EKind Kind = SubroutineSymTable.KindOf(Identifier);
	if (Kind == EKind::NONE
		&& (Kind = ClassSymTable.KindOf(Identifier)) == EKind::NONE)
	{
		std::cerr << "GetIdentifierCategory -> Identifier couldn't be found in both Subroutine or Class Symbol Tables.\n";
		exit(99);
	}
	return KindToString(Kind);
}

void FCompilationEngine::CompileIdentifier(std::string_view IdentifierCategory, EUsage Usage, bool bUseCachedIdentifier)
{
	const std::string_view Identifier = bUseCachedIdentifier ? CachedIdentifier : Tokenizer->Identifier();
	if (bUseCachedIdentifier || Tokenizer->TokenType() == ETokenType::IDENTIFIER)
	{
		OutputIndentation();
		OutFile << "<identifier>\n";
		IncIndent();

		// Always output name
		OutputIndentation();
		OutFile << "<name> " << Identifier << " </name>\n";

		// We're persisting the string through this var because we have a view.
		std::string FoundCategory;
		if (IdentifierCategory == UnknownCategory)
		{
			FoundCategory = GetIdentifierCategory(std::string(Identifier));
			IdentifierCategory = FoundCategory;
		}

		// Always output category
		OutputIndentation();
		OutFile << "<category> " << IdentifierCategory << " </category>\n";

		// If it's a variable type, look up and output index
		FSymbolTable* SymTable = nullptr;
		if (IdentifierCategory == "static" || IdentifierCategory == "field")
			SymTable = &ClassSymTable;

		else if (IdentifierCategory == "var" || IdentifierCategory == "argument")
			SymTable = &SubroutineSymTable;

		if (SymTable)
		{
			const int Index = SymTable->IndexOf(Identifier);
			OutputIndentation();
			OutFile << "<index> " << Index << "</index>\n";
		}

		// Always output usage
		OutputIndentation();
		OutFile << "<usage> " << (Usage == EUsage::Defined ? "defined" : "used") << " </usage>\n";

		DecIndent();
		OutputIndentation();
		OutFile << "</identifier>\n";

		// In this case we're looking 1 token ahead, so no need to advance.
		if (!bUseCachedIdentifier)
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
	if (TT == ETokenType::KEYWORD && ValidTypeKeywords.contains(Keyword))
	{
		OutputKeyword(Keyword);
		return true;
	}
	else if (TT == ETokenType::IDENTIFIER) // In cases of className
	{
		CompileIdentifier(ClassCategory,
			EUsage::Used /*class is used (not declared)*/);
		return true;
	}
	else
		return false;
}

std::string FCompilationEngine::GetType()
{
	/** 'int'|'char'|'boolean'|className */
	const std::string Keyword = std::string(Tokenizer->Keyword());
	const ETokenType  TT = Tokenizer->TokenType();
	if (TT == ETokenType::KEYWORD && ValidTypeKeywords.contains(Keyword))
	{
		return Keyword;
	}
	else if (TT == ETokenType::IDENTIFIER) // In cases of className
	{
		return std::string(Tokenizer->Identifier());
	}
	else
		return "Invalid Type -> GetType()";
}

void FCompilationEngine::OutputCommaSeparatedVarNames(ESymbolTableType SymTableType, std::string Type, EKind Kind, const std::string_view IdentifierCategory, EUsage Usage)
{
	while (Tokenizer->Symbol() == ',')
	{
		OutputSymbol(',');

		// Expect: varName (identifier)
		FSymbolTable* const SymTablePtr = SymTableType == ESymbolTableType::Class
			? &ClassSymTable
			: &SubroutineSymTable;
		SymTablePtr->Define(std::string(Tokenizer->Identifier()), Type, Kind);
		CompileIdentifier(IdentifierCategory, Usage);
	}
}
