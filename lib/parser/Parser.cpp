#include "parser/Parser.h"

#include "parser/ErrorCode.h"
#include "parser/Exception.h"

#include <assert.h>
#include <bitset>

namespace s1
{
  using namespace parser;
  
  Parser::Parser (Lexer& inputLexer, SemanticsHandler& semanticsHandler,
		  ErrorHandler& errorHandler)
   : inputLexer (inputLexer), semanticsHandler (semanticsHandler),
     errorHandler (errorHandler)
  {
    NextToken ();
    
    builtinScope = semanticsHandler.CreateScope (Scope (), SemanticsHandler::Builtin);
  }

  void Parser::Parse ()
  {
    try
    {
      ParseProgram();
    }
    catch (const Exception& e)
    {
      /* emit error */
      errorHandler.ParseError (e.GetCode(), e.GetEncounteredToken(), e.GetExpectedToken());
    }
  }
  
  void Parser::NextToken ()
  {
    if (nextTokens.size() == 0)
    {
      currentToken = *inputLexer;
      ++inputLexer;
    }
    else
    {
      currentToken = nextTokens[0];
      nextTokens.erase (nextTokens.begin());
    }
  }
  
  const Lexer::Token& Parser::Peek (size_t lookahead)
  {
    while (lookahead >= nextTokens.size())
    {
      nextTokens.push_back (*inputLexer);
      ++inputLexer;
    }
    return nextTokens[lookahead];
  }
  
  void Parser::Expect (Lexer::TokenType tokenType)
  {
    if (currentToken.typeOrID != tokenType)
      throw Exception (parser::UnexpectedToken, currentToken, tokenType);
  }
  
  void Parser::UnexpectedToken ()
  {
    throw Exception (parser::UnexpectedToken, currentToken);
  }

  void Parser::ParseProgram ()
  {
    Scope globalScope (semanticsHandler.CreateScope (builtinScope, SemanticsHandler::Global));
    ParseProgramStatements (globalScope);
    Expect (Lexer::EndOfFile);
  }
  
//@@SNIP ParseProgramStatements.txt
  void Parser::ParseProgramStatements (const Scope& scope)
  {
    while (true)
    {
      int beyondType;
      bool isType = IsType (scope, beyondType);
      if (currentToken.typeOrID == Lexer::kwConst)
      {
	/* Konstantendeklaration */
//@@ENDSNIP
	/* constant declaration */
//@@SNIP ParseProgramStatements.txt
	ParseConstDeclare (scope);
	Expect (Lexer::Semicolon);
	NextToken();
      }
      else if ((isType
	  && (Peek (beyondType).typeOrID == Lexer::Identifier))
	|| (currentToken.typeOrID == Lexer::kwVoid))
      {
	if ((currentToken.typeOrID == Lexer::kwVoid)
	    || (Peek (beyondType+1).typeOrID == Lexer::ParenL))
	{
	  /* Funktionsdeklaration */
//@@ENDSNIP
	  /* Function declaration */
//@@SNIP ParseProgramStatements.txt
	  ParseFuncDeclare (scope);
	}
	else
	{
	  /* Variablendeklaration */
//@@ENDSNIP
	  /* Variable declaration */
//@@SNIP ParseProgramStatements.txt
	  ParseVarDeclare (scope);
	  Expect (Lexer::Semicolon);
	  NextToken();
	}
      }
      else if (currentToken.typeOrID == Lexer::kwTypedef)
      {
	/* Typdefinition */
//@@ENDSNIP
	/* Type definition */
//@@SNIP ParseProgramStatements.txt
	ParseTypedef (scope);
	Expect (Lexer::Semicolon);
	NextToken();
      }
      else
	/* Unbekanntes Token - aufrufende Methode wird Fehler
	   auslösen (erwartet "end of file"-Token) */
//@@ENDSNIP
	/* Unknown token - let ParseProgram throw error */
//@@SNIP ParseProgramStatements.txt
	break;
    }
  }
//@@ENDSNIP
  
  void Parser::ParseBlock (Block block)
  {
    while (true)
    {
      try
      {
	int beyondType;
	Scope blockScope = block->GetInnerScope();
	bool isType = IsType (blockScope, beyondType);
	if (currentToken.typeOrID == Lexer::kwConst)
	{
	  /* constant declaration */
	  ParseConstDeclare (blockScope);
	  Expect (Lexer::Semicolon);
	  NextToken();
	}
	else if (isType && (Peek (beyondType).typeOrID == Lexer::Identifier))
	{
	  /* Variable declaration */
	  ParseVarDeclare (blockScope);
	  Expect (Lexer::Semicolon);
	  NextToken();
	}
	else if (currentToken.typeOrID == Lexer::kwTypedef)
	{
	  /* Type definition */
	  ParseTypedef (blockScope);
	  Expect (Lexer::Semicolon);
	  NextToken();
	}
	else if (IsCommand (blockScope))
	{
	  ParseCommand (block);
	}
	else
	  break;
	/* TODO: could improve behaviour in case unexpected tokens are encountered */
      }
      catch (const Exception& e)
      {
	/* emit error */
	errorHandler.ParseError (e.GetCode(), e.GetEncounteredToken(), e.GetExpectedToken());
	// Seek next ';' (end of statement) or '}' (end of block)
	while ((currentToken.typeOrID != Lexer::Semicolon)
	  && (currentToken.typeOrID != Lexer::BraceR)
	  && (currentToken.typeOrID != Lexer::EndOfFile))
	{
	  NextToken();
	}
	if (currentToken.typeOrID == Lexer::Semicolon)
	{
	  NextToken ();
	  continue;
	}
	else
	  // '}', exit block
	  break;
      }
    }
  }
  
  bool Parser::IsCommand (const Scope& scope)
  {
    return IsExpression (scope)
      || (currentToken.typeOrID == Lexer::kwReturn)
      || (currentToken.typeOrID == Lexer::kwIf)
      || (currentToken.typeOrID == Lexer::kwWhile)
      || (currentToken.typeOrID == Lexer::kwFor)
      || (currentToken.typeOrID == Lexer::BraceL);
  }
  
  void Parser::ParseCommand (Block block)
  {
    Scope blockScope = block->GetInnerScope();
    if (IsExpression (blockScope))
    {
      Expression expr = ParseExpression (blockScope);
      Expect (Lexer::Semicolon);
      NextToken();
      block->AddExpressionCommand (expr);
    }
    else if (currentToken.typeOrID == Lexer::kwReturn)
    {
      /* 'return' instruction */
      NextToken();
      Expression returnExpr;
      if (IsExpression (blockScope))
      {
	/* Return with some value */
	returnExpr = ParseExpression (blockScope);
      }
      block->AddReturnCommand (returnExpr);
      Expect (Lexer::Semicolon);
      NextToken();
    }
    else if (currentToken.typeOrID == Lexer::kwIf)
    {
      /* 'if' */
      ParseIf (block);
    }
    else if (currentToken.typeOrID == Lexer::kwWhile)
    {
      /* 'while' */
      ParseLoopWhile (block);
    }
    else if (currentToken.typeOrID == Lexer::kwFor)
    {
      /* 'for' */
      ParseLoopFor (block);
    }
    else if (currentToken.typeOrID == Lexer::BraceL)
    {
      /* nested block */
      NextToken ();
      Block newBlock = semanticsHandler.CreateBlock (blockScope);
      ParseBlock (newBlock);
      Expect (Lexer::BraceR);
      NextToken ();
      block->AddNestedBlock (newBlock);
    }
  }
  
  bool Parser::IsBinaryOperationToken (Lexer::TokenType tokenType)
  {
    switch (tokenType)
    {
    case Lexer::Equals:
    case Lexer::NotEquals:
    case Lexer::Larger:
    case Lexer::LargerEqual:
    case Lexer::Smaller:
    case Lexer::SmallerEqual:
    case Lexer::Plus:
    case Lexer::Minus:
    case Lexer::Mult:
    case Lexer::Div:
    case Lexer::Mod:
    case Lexer::LogicOr:
    case Lexer::LogicAnd:
      return true;
    default:
      break;
    }
    return false;
  }
  
  bool Parser::IsUnaryOperationToken (Lexer::TokenType tokenType)
  {
    switch (tokenType)
    {
    case Lexer::Minus:
    case Lexer::BitwiseInvert:
    case Lexer::LogicInvert:
      return true;
    default:
      break;
    }
    return false;
  }
  
  bool Parser::IsExpression (const Scope& scope)
  {
    if ((currentToken.typeOrID == Lexer::ParenL)
	|| (currentToken.typeOrID == Lexer::kwTrue)
	|| (currentToken.typeOrID == Lexer::kwFalse)
	|| (currentToken.typeOrID == Lexer::Numeric))
      return true;
    // Check if assignment
    if ((currentToken.typeOrID == Lexer::Identifier)
	  && (Peek ().typeOrID == Lexer::Assign))
      return true;
    // Check if attribute
    if ((currentToken.typeOrID == Lexer::Identifier)
	  && (Peek ().typeOrID == Lexer::Member))
      return true;
    // Check if array access
    if ((currentToken.typeOrID == Lexer::Identifier)
	  && (Peek ().typeOrID == Lexer::BracketL))
      return true;
    // Check if 'stand-alone' expression
    if (((currentToken.typeOrID == Lexer::Identifier)
	    && (IsBinaryOperationToken (Peek ().typeOrID) 	// X op Y
	      || (Peek ().typeOrID == Lexer::TernaryIf)))	// X ? ...
	  || IsUnaryOperationToken (currentToken.typeOrID))	// !X, ~X ...
      return true;
    /* Check if function call - can be <Identifier> '('
       or <type> '(', where <type> can take up multiple tokens,
       especially for arrays */
    if ((currentToken.typeOrID == Lexer::Identifier)
	  && (Peek ().typeOrID == Lexer::ParenL))
      return true;
    int checkForParens = 0;
    if (IsType (scope, checkForParens))
    {
      if (Peek (checkForParens).typeOrID == Lexer::ParenL)
	return true;
    }
    return false;
  }
  
  Parser::Expression Parser::ParseExpression (const Scope& scope)
  {
    Expression expr = ParseExprLogicOr (scope);
    if (currentToken.typeOrID == Lexer::Assign)
    {
      // Assignment expression
      NextToken();
      Expression assignedExpr = ParseExpression (scope);
      expr = semanticsHandler.CreateAssignExpression (expr, assignedExpr);
    }
    else if (currentToken.typeOrID == Lexer::TernaryIf)
    {
      expr = ParseExprTernary (expr, scope);
    }
    return expr;
  }

  Parser::Expression Parser::ParseExprBase (const Scope& scope)
  {
    Expression expr;
    int beyondType = 0;
    bool isType;
    if ((currentToken.typeOrID == Lexer::Identifier)
	|| ((isType = IsType (scope, beyondType))
	  && (Peek (beyondType).typeOrID == Lexer::ParenL)))
    {
      /* Expression could be: 
         * a variable/attribute value,
	 * function call,
	 * type constructor
       */
      Type type;
      Name idName;
      if (isType)
	type = ParseType (scope);
      else
      {
	idName = scope->ResolveIdentifier (currentToken.tokenString);
	NextToken();
      }
      /* if identifier is function name ... */
      if (isType || (idName->GetType() == SemanticsHandler::Name::Function))
      {
	// ... parse function call
	SemanticsHandler::ExpressionVector params;
	ParseFuncParamActual (scope, params);
	if (isType)
	  expr = semanticsHandler.CreateTypeConstructorExpression (type, params);
	else
	  expr = semanticsHandler.CreateFunctionCallExpression (idName, params);
      }
      else
      {
	expr = semanticsHandler.CreateVariableExpression (idName);
      }
    }
    else if (currentToken.typeOrID == Lexer::ParenL)
    {
      // '(' - nested expression
      NextToken();
      expr = ParseExpression (scope);
      Expect (Lexer::ParenR);
      NextToken ();
    }
    else if ((currentToken.typeOrID == Lexer::kwTrue) || (currentToken.typeOrID == Lexer::kwFalse))
    {
      // boolean constant
      expr = ParseExprConstBool();
    }
    else if (currentToken.typeOrID == Lexer::Numeric)
    {
      // numeric constant
      expr = semanticsHandler.CreateConstNumericExpression (currentToken.tokenString);
      NextToken();
    }
    else
    {
      UnexpectedToken();
    }
    return ParseAttributeOrArrayAccess (scope, expr);
  }
  
  Parser::Expression Parser::ParseAttributeOrArrayAccess (const Scope& scope,
							  Expression baseExpr)
  {
    Parser::Expression expr = baseExpr;
    while (true)
    {
      if (currentToken.typeOrID == Lexer::Member)
      {
	NextToken ();
	Expect (Lexer::Identifier);
	expr = semanticsHandler.CreateAttributeAccess (expr, currentToken.tokenString);
	NextToken();
      }
      else if (currentToken.typeOrID == Lexer::BracketL)
      {
	NextToken();
	Expression indexExpr = ParseExpression (scope);
	Expect (Lexer::BracketR);
	NextToken();
	expr = semanticsHandler.CreateArrayElementAccess (expr, indexExpr);
      }
      else
	break;
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprMultiplication (const Scope& scope)
  {
    Expression expr = ParseExprUnary (scope);
    while ((currentToken.typeOrID == Lexer::Mult)
      || (currentToken.typeOrID == Lexer::Div)
      || (currentToken.typeOrID == Lexer::Mod))
    {
      SemanticsHandler::ArithmeticOp op;
      switch (currentToken.typeOrID)
      {
      case Lexer::Mult:
	op = SemanticsHandler::Mul;
	break;
      case Lexer::Div:
	op = SemanticsHandler::Div;
	break;
      case Lexer::Mod:
	op = SemanticsHandler::Mod;
	break;
      default:
	assert (false);
      }
      NextToken();
      Expression expr2 = ParseExprUnary (scope);
      expr = semanticsHandler.CreateArithmeticExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprAddition (const Scope& scope)
  {
    Expression expr = ParseExprMultiplication (scope);
    while ((currentToken.typeOrID == Lexer::Plus)
      || (currentToken.typeOrID == Lexer::Minus))
    {
      SemanticsHandler::ArithmeticOp op;
      switch (currentToken.typeOrID)
      {
      case Lexer::Plus:
	op = SemanticsHandler::Add;
	break;
      case Lexer::Minus:
	op = SemanticsHandler::Sub;
	break;
      default:
	assert (false);
      }
      NextToken();
      Expression expr2 = ParseExprMultiplication (scope);
      expr = semanticsHandler.CreateArithmeticExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprUnary (const Scope& scope)
  {
    Expression expr;
    if ((currentToken.typeOrID == Lexer::BitwiseInvert)
      || (currentToken.typeOrID == Lexer::LogicInvert)
      || (currentToken.typeOrID == Lexer::Minus))
    {
      SemanticsHandler::UnaryOp op;
      switch (currentToken.typeOrID)
      {
      case Lexer::BitwiseInvert:
	op = SemanticsHandler::Inv;
	break;
      case Lexer::LogicInvert:
	op = SemanticsHandler::Not;
	break;
      case Lexer::Minus:
	op = SemanticsHandler::Neg;
	break;
      default:
	assert (false);
      }
      NextToken();
      expr = ParseExprBase (scope);
      expr = semanticsHandler.CreateUnaryExpression (op, expr);
    }
    else
      expr = ParseExprBase (scope);
    return expr;
  }
  
  Parser::Expression Parser::ParseExprTernary (const Expression& prefix, const Scope& scope)
  {
    Expression expr (prefix);
    NextToken();
    Expression expr2 = ParseExpression (scope);
    Expect (Lexer::TernaryElse);
    NextToken();
    Expression expr3 = ParseExpression (scope);
    return semanticsHandler.CreateTernaryExpression (expr, expr2, expr3);
  }
  
  Parser::Expression Parser::ParseExprCompareEqual (const Scope& scope)
  {
    Expression expr = ParseExprComparison (scope);
    while ((currentToken.typeOrID == Lexer::Equals)
      || (currentToken.typeOrID == Lexer::NotEquals))
    {
      SemanticsHandler::CompareOp op;
      switch (currentToken.typeOrID)
      {
      case Lexer::Equals:
	op = SemanticsHandler::Equals;
	break;
      case Lexer::NotEquals:
	op = SemanticsHandler::NotEquals;
	break;
      default:
	assert (false);
      }
      NextToken();
      Expression expr2 = ParseExprComparison (scope);
      expr = semanticsHandler.CreateComparisonExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprComparison (const Scope& scope)
  {
    Expression expr = ParseExprAddition (scope);
    while ((currentToken.typeOrID == Lexer::Larger)
      || (currentToken.typeOrID == Lexer::LargerEqual)
      || (currentToken.typeOrID == Lexer::Smaller)
      || (currentToken.typeOrID == Lexer::SmallerEqual))
    {
      SemanticsHandler::CompareOp op;
      switch (currentToken.typeOrID)
      {
      case Lexer::Larger:
	op = SemanticsHandler::Larger;
	break;
      case Lexer::LargerEqual:
	op = SemanticsHandler::LargerEqual;
	break;
      case Lexer::Smaller:
	op = SemanticsHandler::Smaller;
	break;
      case Lexer::SmallerEqual:
	op = SemanticsHandler::SmallerEqual;
	break;
      default:
	assert (false);
      }
      NextToken();
      Expression expr2 = ParseExprAddition (scope);
      expr = semanticsHandler.CreateComparisonExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprLogicOr (const Scope& scope)
  {
    Expression expr = ParseExprLogicAnd (scope);
    while (currentToken.typeOrID == Lexer::LogicOr)
    {
      NextToken();
      Expression expr2 = ParseExprLogicAnd (scope);
      expr = semanticsHandler.CreateLogicExpression (SemanticsHandler::Or, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprLogicAnd (const Scope& scope)
  {
    Expression expr = ParseExprCompareEqual (scope);
    while (currentToken.typeOrID == Lexer::LogicAnd)
    {
      NextToken();
      Expression expr2 = ParseExprCompareEqual (scope);
      expr = semanticsHandler.CreateLogicExpression (SemanticsHandler::And, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprConstBool ()
  {
    Expression expr = semanticsHandler.CreateConstBoolExpression (
      currentToken.typeOrID == Lexer::kwTrue);
    NextToken();
    return expr;
  }
  
  bool Parser::IsType (const Scope& scope, int& peekAfterType)
  {
    Lexer::TokenType tokenID = currentToken.typeOrID;
    peekAfterType = 0;
    if (currentToken.typeOrID == Lexer::kwUnsigned)
    {
      peekAfterType++;
      tokenID = Peek().typeOrID;
    }
    bool isType = false;
    switch (tokenID)
    {
    case Lexer::kwBool:
    case Lexer::kwInt:
    case Lexer::kwFloat:
    case Lexer::kwSampler1D:
    case Lexer::kwSampler2D:
    case Lexer::kwSampler3D:
    case Lexer::kwSamplerCUBE:
      isType = true;
      break;
    case Lexer::Identifier:
      /* Might be a type alias */
      {
	Name typeName = scope->ResolveIdentifier (currentToken.tokenString);
	isType = (typeName->GetType() == SemanticsHandler::Name::TypeAlias);
      }
      break;
    default:
      break;
    }
    if (isType)
    {
      while ((Peek (peekAfterType).typeOrID == Lexer::BracketL)
	&& (Peek (peekAfterType+1).typeOrID == Lexer::BracketR))
      {
	peekAfterType += 2;
      }
    }
    return isType;
  }
  
  Parser::Type Parser::ParseTypeBase (const Scope& scope)
  {
    bool isUnsigned = false;
    if (currentToken.typeOrID == Lexer::kwUnsigned)
    {
      isUnsigned = true;
      NextToken();
    }
    switch (currentToken.typeOrID)
    {
    case Lexer::kwBool:
    case Lexer::kwInt:
    case Lexer::kwFloat:
      {
	switch (currentToken.typeClass)
	{
	case Lexer::Normal:
	  if (currentToken.typeOrID == Lexer::kwBool)
	    return ParseTypeBool ();
	  else
	    return ParseTypeNumeric (isUnsigned);
	case Lexer::Vector:
	  return ParseTypeVector (isUnsigned);
	case Lexer::Matrix:
	  return ParseTypeMatrix (isUnsigned);
	}
      }
      break;
    case Lexer::kwSampler1D:
    case Lexer::kwSampler2D:
    case Lexer::kwSampler3D:
    case Lexer::kwSamplerCUBE:
      return ParseTypeSampler ();
    case Lexer::Identifier:
      {
	Name typeName = scope->ResolveIdentifier (currentToken.tokenString);
	Type type;
	if (typeName->GetType() == SemanticsHandler::Name::TypeAlias)
	{
	  type = typeName->GetAliasedType();
	}
	else
	{
	  throw Exception (parser::ExpectedTypeName, currentToken);
	}
	NextToken ();
	return type;
      }
      break;
    default:
      UnexpectedToken ();
    }
    return Type ();
  }
  
  Parser::Type Parser::ParseType (const Scope& scope)
  {
    Type type = ParseTypeBase (scope);
    if (currentToken.typeOrID == Lexer::BracketL)
    {
      type = ParseTypeArray (type);
    }
    return type;
  }
  
  Parser::Type Parser::ParseTypeBool ()
  {
    NextToken();
    return semanticsHandler.CreateType (SemanticsHandler::Bool);
  }
  
  Parser::Type Parser::ParseTypeNumeric (bool isUnsigned)
  {
    Type type;
    switch (currentToken.typeOrID)
    {
    case Lexer::kwInt:
      /* int */
      type = semanticsHandler.CreateType (
	isUnsigned ? SemanticsHandler::UInt : SemanticsHandler::Int);
      NextToken();
      break;
    case Lexer::kwFloat:
      /* float */
      type = semanticsHandler.CreateType (SemanticsHandler::Float);
      NextToken();
      break;
    default:
      UnexpectedToken ();
    }
    return type;
  }
  
  Parser::Type Parser::ParseTypeVector (bool isUnsigned)
  {
    Type baseType;
    switch (currentToken.typeOrID)
    {
    case Lexer::kwBool:
      baseType = semanticsHandler.CreateType (SemanticsHandler::Bool);
      break;
    case Lexer::kwInt:
      /* int */
      baseType = semanticsHandler.CreateType (
	isUnsigned ? SemanticsHandler::UInt : SemanticsHandler::Int);
      break;
    case Lexer::kwFloat:
      /* float */
      baseType = semanticsHandler.CreateType (SemanticsHandler::Float);
      break;
    default:
      UnexpectedToken ();
    }
    Type vecType = semanticsHandler.CreateVectorType (baseType, currentToken.dimension1);
    NextToken();
    return vecType;
  }
  
  Parser::Type Parser::ParseTypeMatrix (bool isUnsigned)
  {
    Type baseType;
    switch (currentToken.typeOrID)
    {
    case Lexer::kwBool:
      baseType = semanticsHandler.CreateType (SemanticsHandler::Bool);
      break;
    case Lexer::kwInt:
      /* int */
      baseType = semanticsHandler.CreateType (
	isUnsigned ? SemanticsHandler::UInt : SemanticsHandler::Int);
      break;
    case Lexer::kwFloat:
      /* float */
      baseType = semanticsHandler.CreateType (SemanticsHandler::Float);
      break;
    default:
      UnexpectedToken ();
    }
    Type matType = semanticsHandler.CreateMatrixType (baseType, currentToken.dimension1,
						   currentToken.dimension2);
    NextToken();
    return matType;
  }
  
  Parser::Type Parser::ParseTypeSampler ()
  {
    Type type;
    switch (currentToken.typeOrID)
    {
    case Lexer::kwSampler1D:
      /* 1D sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::_1D);
      NextToken();
      break;
    case Lexer::kwSampler2D:
      /* 2D sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::_2D);
      NextToken();
      break;
    case Lexer::kwSampler3D:
      /* 3D sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::_3D);
      NextToken();
      break;
    case Lexer::kwSamplerCUBE:
      /* CUBE sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::CUBE);
      NextToken();
      break;
    default:
      UnexpectedToken ();
    }
    return type;
  }
  
  Parser::Type Parser::ParseTypeArray (Type baseType)
  {
    NextToken();
    Expect (Lexer::BracketR);
    Type type = semanticsHandler.CreateArrayType (baseType);
    NextToken ();
    if (currentToken.typeOrID == Lexer::BracketL)
    {
      type = ParseTypeArray (type);
    }
    return type;
  }
  
  void Parser::ParseTypedef (const Scope& scope)
  {
    // Skip typedef
    NextToken();
    Type aliasedType = ParseType (scope);
    // Add to scope
    Expect (Lexer::Identifier);
    scope->AddTypeAlias (aliasedType, currentToken.tokenString);
    NextToken();
  }

//@@SNIP ParseFuncDeclare.txt
  void Parser::ParseFuncDeclare (const Scope& scope)
  {
    // Rückgabetyp parsen
    Type returnType = ParseFuncType (scope);
    // Funktionsbezeichner parsen
    Expect (Lexer::Identifier);
    UnicodeString functionIdentifier (currentToken.tokenString);
    NextToken ();
    // Formale Parameter parsen
    Expect (Lexer::ParenL);
    SemanticsHandler::Scope::FunctionFormalParameters params;
    ParseFuncParamFormal (scope, params);
    // Funktion zu 'Scope' hinzufügen, Anweisungsblock speichern
//@@ENDSNIP
    // Add function to scope, get block
//@@SNIP ParseFuncDeclare.txt
    Block inFunctionBlock = scope->AddFunction (returnType, functionIdentifier, params);
    Expect (Lexer::BraceL);
    NextToken();
    // Anweisungen in Block verarbeiten
    ParseBlock (inFunctionBlock);
    Expect (Lexer::BraceR);
    NextToken();
  }
//@@ENDSNIP
  
  Parser::Type Parser::ParseFuncType (const Scope& scope)
  {
    if (currentToken.typeOrID == Lexer::kwVoid)
    {
      Parser::Type type = semanticsHandler.CreateType (SemanticsHandler::Void);
      NextToken ();
      return type;
    }
    else if (IsType (scope))
    {
      return ParseType (scope);
    }
    else
      UnexpectedToken();
    return Parser::Type ();
  }
  
  void Parser::ParseFuncParamFormal (const Scope& scope, SemanticsHandler::Scope::FunctionFormalParameters& params)
  {
    // Skip '('
    NextToken();
    while (true)
    {
      if (currentToken.typeOrID == Lexer::ParenR)
      {
	// End of list
	NextToken ();
	break;
      }

      // TODO: In case of error, skip to next param
      int paramDirection = 0;
      if (currentToken.typeOrID == Lexer::kwIn)
      {
	// 'in' parameter
	NextToken();
	paramDirection |= SemanticsHandler::Scope::dirIn;
      }
      if (currentToken.typeOrID == Lexer::kwOut)
      {
	// 'in' parameter
	NextToken();
	paramDirection |= SemanticsHandler::Scope::dirOut;
      }
      // If no explicit direction is given, use 'in'
      if (paramDirection == 0) paramDirection = SemanticsHandler::Scope::dirIn;
      SemanticsHandler::Scope::FunctionFormalParameter newParam;
      newParam.dir = (SemanticsHandler::Scope::FormalParameterDirection)paramDirection;
      newParam.type = ParseType (scope);
      Expect (Lexer::Identifier);
      newParam.identifier = currentToken.tokenString;
      NextToken ();
      if (currentToken.typeOrID == Lexer::Assign)
      {
	// Handle default value
	NextToken ();
	if (paramDirection == SemanticsHandler::Scope::dirOut)
	  throw Exception (OutParameterWithDefault);
	newParam.defaultValue = ParseExpression (scope);
      }
      params.push_back (newParam);
      if (currentToken.typeOrID == Lexer::Separator)
	NextToken ();
      else if (currentToken.typeOrID != Lexer::ParenR)
	UnexpectedToken ();
    }
  }
  
  //void ParseFuncCall ();

  void Parser::ParseFuncParamActual (const Scope& scope, parser::SemanticsHandler::ExpressionVector& params)
  {
    // Skip '('
    NextToken();
    while (true)
    {
      if (currentToken.typeOrID == Lexer::ParenR)
      {
	// End of list
	NextToken ();
	break;
      }
      
      Expression expr = ParseExpression (scope);
      params.push_back (expr);

      if (currentToken.typeOrID == Lexer::Separator)
	NextToken ();
      else if (currentToken.typeOrID != Lexer::ParenR)
	UnexpectedToken ();
    }
  }

  void Parser::ParseVarDeclare (const Scope& scope)
  {
    Type type = ParseType (scope);
    ParseVarIdentifierAndInitializerList (scope, type);
  }
  
  void Parser::ParseVarIdentifierAndInitializerList (const Scope& scope, Type type)
  {
    while (true)
    {
      ParseVarIdentifierAndInitializer (scope, type);
      if (currentToken.typeOrID == Lexer::Separator)
      {
	// ',' - more variables follow
	NextToken();
	continue;
      }
      break;
    }
  }
  
  void Parser::ParseVarIdentifierAndInitializer (const Scope& scope, Type type)
  {
    Expect (Lexer::Identifier);
    UnicodeString varIdentifier = currentToken.tokenString;
    NextToken ();
    Expression initExpr;
    if (currentToken.typeOrID == Lexer::Assign)
    {
      // Variable has an initializer value
      NextToken ();
      initExpr = ParseExpression (scope);
    }
    scope->AddVariable (type, varIdentifier, initExpr, false);
  }
      
  void Parser::ParseConstDeclare (const Scope& scope)
  {
    NextToken(); // skip 'const'
    Type type = ParseType (scope);
    ParseConstIdentifierAndInitializerList (scope, type);
  }
  
  void Parser::ParseConstIdentifierAndInitializerList (const Scope& scope, Type type)
  {
    do
    {
      ParseConstIdentifierAndInitializer (scope, type);
      if (currentToken.typeOrID == Lexer::Separator)
      {
	// ',' - more constants follow
	NextToken();
	continue;
      }
    }
    while (false);
  }
  
  void Parser::ParseConstIdentifierAndInitializer (const Scope& scope, Type type)
  {
    Expect (Lexer::Identifier);
    UnicodeString varIdentifier = currentToken.tokenString;
    NextToken ();
    // initializer value is required
    Expect (Lexer::Assign);
    NextToken ();
    Expression initExpr = ParseExpression (scope);
    scope->AddVariable (type, varIdentifier, initExpr, true);
  }

//@@SNIP ParseIf.txt
  void Parser::ParseIf (Block block)
  {
    // 'if' überspringen
    NextToken();
    Expect (Lexer::ParenL);
    NextToken();
    // Bedingung parsen
    Expression conditionExpr = ParseExpression (block->GetInnerScope());
    Expect (Lexer::ParenR);
    NextToken();
    Expect (Lexer::BraceL);
    NextToken();
    // 'if'-Block erstellen, ...
    Block ifBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
    // ... einlesen
    ParseBlock (ifBlock);
    Expect (Lexer::BraceR);
    NextToken();
    Block elseBlock;
    if (currentToken.typeOrID == Lexer::kwElse)
    {
      NextToken();
      Expect (Lexer::BraceL);
      NextToken();
      // 'else'-Block erstellen, ...
      elseBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
      // ... einlesen
      ParseBlock (elseBlock);
      Expect (Lexer::BraceR);
      NextToken();
    }
    // Instruktion zu umgebenden Block hinzufügen
    block->AddBranching (conditionExpr, ifBlock, elseBlock);
  }
//@@ENDSNIP

  void Parser::ParseLoopFor (Block block)
  {
    NextToken();
    Expect (Lexer::ParenL);
    NextToken();
    Expression initExpr;
    if (currentToken.typeOrID != Lexer::Semicolon)
      initExpr = ParseExpression (block->GetInnerScope());
    Expect (Lexer::Semicolon);
    NextToken();
    Expression loopTestExpr;
    if (currentToken.typeOrID != Lexer::Semicolon)
      loopTestExpr = ParseExpression (block->GetInnerScope());
    Expect (Lexer::Semicolon);
    NextToken();
    Expression loopFootExpr;
    if (currentToken.typeOrID != Lexer::Semicolon)
      loopFootExpr = ParseExpression (block->GetInnerScope());
    Expect (Lexer::ParenR);
    NextToken();
    Expect (Lexer::BraceL);
    NextToken();
    Block newBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
    ParseBlock (newBlock);
    Expect (Lexer::BraceR);
    NextToken();
    block->AddForLoop (initExpr, loopTestExpr, loopFootExpr, newBlock);
  }
  
  void Parser::ParseLoopWhile (Block block)
  {
    NextToken();
    Expect (Lexer::ParenL);
    NextToken();
    Expression loopTestExpr = ParseExpression (block->GetInnerScope());
    Expect (Lexer::ParenR);
    NextToken();
    Expect (Lexer::BraceL);
    NextToken();
    Block newBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
    ParseBlock (newBlock);
    Expect (Lexer::BraceR);
    NextToken();
    block->AddWhileLoop (loopTestExpr, newBlock);
  }
} // namespace s1
