// Implemented by BarakXYZ 2025

#include "CompilationEngine.h"
#include <iostream>
#include "SymbolTable.h"
#include "SymbolTableVisualizer.h"
#include "AnalyzerUtils.h"

const std::unordered_set<std::string_view> FCompilationEngine::ValidTypeKeywords = {
	"int", "char", "boolean"
};

FCompilationEngine::FCompilationEngine(
	const std::string&				InFileName,
	std::shared_ptr<FJackTokenizer> JackTokenizer)
{
	InitNewEntry(InFileName);
	Tokenizer = JackTokenizer;
};

FCompilationEngine::~FCompilationEngine()
{
	if (OutFileXML.is_open())
		OutFileXML.close();
}

bool FCompilationEngine::InitNewEntry(const std::string& InFileName)
{
	const std::string OutFileNameXML = AnalyzerUtils::ReplaceExtension(InFileName, ".xml");
	std::cout << "Output XML file: " << OutFileNameXML << '\n';

	// Create (open) the file for writing
	OutFileXML.open(OutFileNameXML, std::ios::trunc);
	if (!OutFileXML)
	{
		std::cerr << "Error opening XML output file: " << OutFileNameXML << std::endl;
		return false;
	}

	VMWriter = std::make_unique<FVMWriter>(InFileName);
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
		OutFileXML << ClassBegin;
		IncIndent();
		OutputKeyword("class");
	}
	else
	{
		std::cerr << "Expected: 'class'\n";
		exit(1);
	}

	// Expect: 'className'
	CompiledClassName = std::string(Tokenizer->Identifier());
	CompileIdentifier(ClassCategory, EUsage::Declared);

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
	OutFileXML << ClassEnd;

	PrintSymbolTable(ClassSymTable, CompiledClassName); // Debug
}

void FCompilationEngine::CompileClassVarDec()
{
	/**
	 * ('static') | ('field') type varName (',' varName)* ';'
	 */
	static constexpr std::string_view VarBegin = "<classVarDec>\n";
	static constexpr std::string_view VarEnd = "</classVarDec>\n";

	OutputIndentation();
	OutFileXML << VarBegin;
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
	CompileIdentifier(Keyword, EUsage::Declared);

	// Expect: 0 or more comma separated varNames
	OutputCommaSeparatedVarNames(ESymbolTableType::Class, Type, Kind, Keyword, EUsage::Declared);

	// Expect: ';' (Symbol)
	OutputSymbol(';');

	DecIndent();
	OutputIndentation();
	OutFileXML << VarEnd;
}

void FCompilationEngine::CompileSubroutineDec()
{
	/**
	 ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
	 * '(' parameterList ')' subroutineBody
	 */
	static constexpr std::string_view SubDecBegin = "<subroutineDec>\n";
	static constexpr std::string_view SubDecEnd = "</subroutineDec>\n";

	SubroutineSymTable = FSymbolTable();
	OutputIndentation();
	OutFileXML << SubDecBegin;
	IncIndent();

	// Expect: ('constructor'|'function'|'method') Keywords (checked by CompileClass() in a 'while' loop)
	const std::string FuncKeyword = std::string(Tokenizer->Keyword());
	HandleIfMethodImplicitArg(FuncKeyword);
	OutputKeyword(FuncKeyword);

	// Expect: ('void' | type)
	static constexpr std::string_view Void = "void";
	if (Tokenizer->Keyword() == Void)
		OutputKeyword(Void);
	else
		OutputType();

	// Expect: subroutineName (identifier)
	const std::string SubroutineName = std::string(Tokenizer->Identifier());
	CompileIdentifier(SubroutineCategory, EUsage::Declared);

	// Expect: '(' (symbol))
	OutputSymbol('(');

	// Expect: parameterList (0 or more)
	const int NumOfArgs = CompileParameterList();
	VMWriter->WriteFunction(CompiledClassName + "." + SubroutineName, NumOfArgs);

	// Expect: ')' (symbol))
	OutputSymbol(')');

	// Expect: subroutineBody
	CompileSubroutineBody();

	DecIndent();
	OutputIndentation();
	OutFileXML << SubDecEnd;
	PrintSymbolTable(SubroutineSymTable, SubroutineName); // Debug
}

int FCompilationEngine::CompileParameterList()
{
	/** ((type varName)(',' type varName)*)? */
	static constexpr std::string_view ParamBegin = "<parameterList>\n";
	static constexpr std::string_view ParamEnd = "</parameterList>\n";
	int								  NumOfArgs = 0;

	OutputIndentation();
	OutFileXML << ParamBegin;
	IncIndent();

	std::string Type = GetType();
	if (OutputType()) // Check if empty parameterList
	{
		// Expect: varName
		SubroutineSymTable.Define(std::string(Tokenizer->Identifier()), Type, EKind::ARG);
		CompileIdentifier(ArgumentCategory, EUsage::Declared);

		// Expect: (',' type varName)*
		while (Tokenizer->TokenType() == ETokenType::SYMBOL && Tokenizer->Symbol() == ',')
		{
			OutputSymbol(',');

			Type = GetType();
			OutputType();

			SubroutineSymTable.Define(std::string(Tokenizer->Identifier()), Type, EKind::ARG);
			CompileIdentifier(ArgumentCategory, EUsage::Declared);

			++NumOfArgs;
		}
	}

	DecIndent();
	OutputIndentation();
	OutFileXML << ParamEnd;
	return NumOfArgs;
}

void FCompilationEngine::CompileSubroutineBody()
{
	/** '{' varDec* statements '}' */

	static constexpr std::string_view SubBodyBegin = "<subroutineBody>\n";
	static constexpr std::string_view SubBodyEnd = "</subroutineBody>\n";

	OutputIndentation();
	OutFileXML << SubBodyBegin;
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
	OutFileXML << SubBodyEnd;
}

void FCompilationEngine::CompileVarDec()
{
	/** 'var' type varName (','varName)* ';' */

	static constexpr std::string_view VarDecBegin = "<varDec>\n";
	static constexpr std::string_view VarDecEnd = "</varDec>\n";

	OutputIndentation();
	OutFileXML << VarDecBegin;
	IncIndent();

	// Expect: 'var' (checked by subroutineBody in a while loop)
	OutputKeyword("var");

	// Expect: type
	const std::string Type = GetType();
	OutputType();

	SubroutineSymTable.Define(std::string(Tokenizer->Identifier()), Type, EKind::VAR);

	// Expect: varName (identifier)
	CompileIdentifier(VariableCategory, EUsage::Declared);

	// Expect: (',' varName)*
	OutputCommaSeparatedVarNames(ESymbolTableType::Subroutine, Type, EKind::VAR, VariableCategory, EUsage::Declared);

	// Expect: ';' (symbol)
	OutputSymbol(';');

	DecIndent();
	OutputIndentation();
	OutFileXML << VarDecEnd;
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
	OutFileXML << StatementsBegin;
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
	OutFileXML << StatementsEnd;
}

void FCompilationEngine::CompileLet()
{
	/** 'let' varName ('['expression']')? '=' expression';' */
	static constexpr std::string_view LetBegin = "<letStatement>\n";
	static constexpr std::string_view LetEnd = "</letStatement>\n";

	OutputIndentation();
	OutFileXML << LetBegin;
	IncIndent();

	// Expect: 'let' (keyword)
	OutputKeyword("let"); // Checked by CompileStatements

	// Expect: varName (identifier)
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
	OutFileXML << LetEnd;
}

void FCompilationEngine::CompileIf()
{
	/** 'if' '('expression')' '{' statements '}' ('else' '{' statements '}')? */
	static constexpr std::string_view IfBegin = "<ifStatement>\n";
	static constexpr std::string_view IfEnd = "</ifStatement>\n";

	OutputIndentation();
	OutFileXML << IfBegin;
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
	OutFileXML << IfEnd;
}

void FCompilationEngine::CompileWhile()
{
	/** 'while' '('expression')' '{' statements '}' */
	static constexpr std::string_view WhileBegin = "<whileStatement>\n";
	static constexpr std::string_view WhileEnd = "</whileStatement>\n";

	OutputIndentation();
	OutFileXML << WhileBegin;
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
	OutFileXML << WhileEnd;
}

void FCompilationEngine::CompileDo()
{
	/** 'do' subroutineCall';' */
	static constexpr std::string_view DoBegin = "<doStatement>\n";
	static constexpr std::string_view DoEnd = "</doStatement>\n";

	OutputIndentation();
	OutFileXML << DoBegin;
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
	OutFileXML << DoEnd;
}

void FCompilationEngine::CompileReturn()
{
	/** 'return' expression? ';' */
	static constexpr std::string_view ReturnBegin = "<returnStatement>\n";
	static constexpr std::string_view ReturnEnd = "</returnStatement>\n";

	OutputIndentation();
	OutFileXML << ReturnBegin;
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
	OutFileXML << ReturnEnd;
}

void FCompilationEngine::CompileExpression()
{
	/** term(op term)* */
	static constexpr std::string_view ExpressionBegin = "<expression>\n";
	static constexpr std::string_view ExpressionEnd = "</expression>\n";

	OutputIndentation();
	OutFileXML << ExpressionBegin;
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
	OutFileXML << ExpressionEnd;
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
	OutFileXML << TermBegin;
	IncIndent();

	const ETokenType TT = Tokenizer->TokenType();
	switch (TT)
	{
		case ETokenType::INT_CONST:
		{
			static constexpr std::string_view IntBegin = "<integerConstant> ";
			static constexpr std::string_view IntEnd = " </integerConstant>\n";

			OutputIndentation();
			const int IntValue = Tokenizer->IntVal();
			OutFileXML << IntBegin << IntValue << IntEnd;
			VMWriter->WritePush(ESegment::CONST, IntValue);
			TryAdvanceTokenizer();
			break;
		}
		case ETokenType::STRING_CONST:
		{
			static constexpr std::string_view StrBegin = "<stringConstant> ";
			static constexpr std::string_view StrEnd = " </stringConstant>\n";

			OutputIndentation();
			OutFileXML << StrBegin << Tokenizer->StringVal() << StrEnd;
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
					CompileIdentifier(UnknownCategory, EUsage::Used, true /*UseCachedIdentifier*/);
					OutputSymbol('[');
					CompileExpression();
					OutputSymbol(']');
				}
				else if (Symbol == '(' || Symbol == '.') // Check if SubCall
					CompileSubroutineCall();
				else
					CompileIdentifier(UnknownCategory, EUsage::Used, true /*UseCachedIdentifier*/);
			}
			else
				CompileIdentifier(UnknownCategory, EUsage::Used, true /*UseCachedIdentifier*/);

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
	OutFileXML << TermEnd;
}

void FCompilationEngine::CompileExpressionList()
{
	/** (expression(','expression)* )? */
	static constexpr std::string_view ExpListBegin = "<expressionList>\n";
	static constexpr std::string_view ExpListEnd = "</expressionList>\n";

	OutputIndentation();
	OutFileXML << ExpListBegin;
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
	OutFileXML << ExpListEnd;
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
		CompileExpressionList();
		OutputSymbol(')');
	}

	/** (className | varName)'.'subroutineName '('expressionList')' */
	else if (Symbol == '.')
	{
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
		if (OutFileXML.is_open())
			OutFileXML.close();
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
		OutFileXML << SymBegin << Symbol << SymEnd;
		if (bShouldAdvanceFile)
			TryAdvanceTokenizer();
	}
}

void FCompilationEngine::OutputSymbol(const std::string_view Symbol)
{
	OutputIndentation();
	OutFileXML << SymBegin << Symbol << SymEnd;
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
		OutFileXML << "<identifier>\n";
		IncIndent();

		// Always output name
		OutputIndentation();
		OutFileXML << "<name> " << Identifier << " </name>\n";

		// We're persisting the string through this var because we have a view.
		std::string FoundCategory;
		if (IdentifierCategory == UnknownCategory)
		{
			FoundCategory = GetIdentifierCategory(std::string(Identifier));
			IdentifierCategory = FoundCategory;
		}

		// Always output category
		OutputIndentation();
		OutFileXML << "<category> " << IdentifierCategory << " </category>\n";

		// If it's a variable type, look up and output index
		FSymbolTable* SymTable = nullptr;
		if (IdentifierCategory == "static" || IdentifierCategory == "field")
			SymTable = &ClassSymTable;

		else if (IdentifierCategory == "local" || IdentifierCategory == "argument")
			SymTable = &SubroutineSymTable;

		if (SymTable)
		{
			const int Index = SymTable->IndexOf(Identifier);
			OutputIndentation();
			OutFileXML << "<index> " << Index << " </index>\n";
		}

		// Always output usage
		OutputIndentation();
		OutFileXML << "<usage> " << (Usage == EUsage::Declared ? "declared" : "used") << " </usage>\n";

		DecIndent();
		OutputIndentation();
		OutFileXML << "</identifier>\n";

		// In this case we're looking 1 token ahead, so no need to advance.
		if (!bUseCachedIdentifier)
			TryAdvanceTokenizer();
	}
}

void FCompilationEngine::OutputKeyword(const std::string_view Keyword)
{
	static constexpr std::string_view KeyBegin = "<keyword> ";
	static constexpr std::string_view KeyEnd = " </keyword>\n";

	OutputIndentation();
	OutFileXML << KeyBegin << Keyword << KeyEnd;
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

void FCompilationEngine::HandleIfMethodImplicitArg(const std::string& FuncKeyword)
{
	static const std::string This = "this";
	if (FuncKeyword == "method")
	{
		SubroutineSymTable.Define(This, CompiledClassName, EKind::ARG);
	}
}
