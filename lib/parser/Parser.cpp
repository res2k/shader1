#include "parser/Parser.h"

#include "parser/ErrorCode.h"
#include "parser/Exception.h"

#include <assert.h>
#include <bitset>

namespace s1
{
  using namespace parser;
  
  Parser::Parser (Lexer& inputLexer, SemanticsHandler& semanticsHandler)
   : inputLexer (inputLexer), semanticsHandler (semanticsHandler)
  {
    NextToken ();
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
	}
	else if (isType && (Peek (beyondType).typeOrID == Lexer::Identifier))
	{
	  /* Variable declaration */
	  ParseVarDeclare (blockScope);
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
      }
      catch (const Exception&)
      {
	/* TODO: emit error */
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
  
  bool Parser::IsCommand (Scope scope)
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
  
  bool Parser::IsExpression (Scope scope)
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
    /* Check if function call - can be <Identifier> '('
       or <type> '(', where <type> can take up multiple tokens,
       especially for arrays */
    if ((currentToken.typeOrID == Lexer::Identifier)
	  && (Peek ().typeOrID == Lexer::ParenL))
      return true;
    int checkForParens = 0;
    if (IsType (scope, checkForParens))
    {
      while ((Peek (checkForParens).typeOrID == Lexer::BracketL)
	&& (Peek (checkForParens).typeOrID == Lexer::BracketR))
      {
	checkForParens += 2;
      }
      if (Peek (checkForParens).typeOrID == Lexer::ParenL)
	return true;
    }
    return false;
  }
  
  Parser::Expression Parser::ParseExpression (Scope scope)
  {
    Expression expr = ParseExprTernary (scope);
    if (currentToken.typeOrID == Lexer::Assign)
    {
      // Assignment expression
      NextToken();
      Expression assignedExpr = ParseExpression (scope);
      expr = semanticsHandler.CreateAssignExpression (expr, assignedExpr);
    }
    return expr;
  }

  Parser::Expression Parser::ParseExprBase (Scope scope)
  {
    Expression expr;
    if (currentToken.typeOrID == Lexer::Identifier)
    {
      /* Expression could be: 
         * a variable/attribute value,
	 * function call 
       */
      Name idName = scope->ResolveIdentifier (currentToken.tokenString);
      NextToken();
      /* if identifier is function name ... */
      if (idName->GetType() == SemanticsHandler::Name::Function)
      {
	// ... parse function call
	Expect (Lexer::ParenL);
	NextToken ();
	// TODO: Parse actual parameters list, query function call expression from semanticsHandler
	Expect (Lexer::ParenR);
	NextToken ();
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
  
  Parser::Expression Parser::ParseAttributeOrArrayAccess (Scope scope,
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
  
  Parser::Expression Parser::ParseExprMultiplication (Scope scope)
  {
    Expression expr = ParseExprUnary (scope);
    if ((currentToken.typeOrID == Lexer::Mult)
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
      }
      NextToken();
      Expression expr2 = ParseExprMultiplication (scope);
      expr = semanticsHandler.CreateArithmeticExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprAddition (Scope scope)
  {
    Expression expr = ParseExprMultiplication (scope);
    if ((currentToken.typeOrID == Lexer::Plus)
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
      }
      NextToken();
      Expression expr2 = ParseExprAddition (scope);
      expr = semanticsHandler.CreateArithmeticExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprUnary (Scope scope)
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
      }
      NextToken();
      expr = ParseExprBase (scope);
      expr = semanticsHandler.CreateUnaryExpression (op, expr);
    }
    else
      expr = ParseExprBase (scope);
    return expr;
  }
  
  Parser::Expression Parser::ParseExprTernary (Scope scope)
  {
    Expression expr = ParseExprLogicOr (scope);
    if (currentToken.typeOrID == Lexer::TernaryIf)
    {
      NextToken();
      Expression expr2 = ParseExpression (scope);
      Expect (Lexer::TernaryElse);
      NextToken();
      Expression expr3 = ParseExpression (scope);
      expr = semanticsHandler.CreateTernaryExpression (expr, expr2, expr3);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprCompareEqual (Scope scope)
  {
    Expression expr = ParseExprComparison (scope);
    if ((currentToken.typeOrID == Lexer::Equals)
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
      }
      NextToken();
      Expression expr2 = ParseExprCompareEqual (scope);
      expr = semanticsHandler.CreateComparisonExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprComparison (Scope scope)
  {
    Expression expr = ParseExprAddition (scope);
    if ((currentToken.typeOrID == Lexer::Larger)
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
      }
      NextToken();
      Expression expr2 = ParseExprComparison (scope);
      expr = semanticsHandler.CreateComparisonExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprLogicOr (Scope scope)
  {
    Expression expr = ParseExprLogicAnd (scope);
    if (currentToken.typeOrID == Lexer::LogicOr)
    {
      NextToken();
      Expression expr2 = ParseExprLogicOr (scope);
      expr = semanticsHandler.CreateLogicExpression (SemanticsHandler::Or, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprLogicAnd (Scope scope)
  {
    Expression expr = ParseExprCompareEqual (scope);
    if (currentToken.typeOrID == Lexer::LogicAnd)
    {
      NextToken();
      Expression expr2 = ParseExprLogicAnd (scope);
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
  
  bool Parser::IsType (Scope scope, int& peekAfterType)
  {
    Lexer::TokenType tokenID = currentToken.typeOrID;
    peekAfterType = 0;
    if (currentToken.typeOrID == Lexer::kwUnsigned)
    {
      peekAfterType++;
      tokenID = Peek().typeOrID;
    }
    switch (tokenID)
    {
    case Lexer::kwBool:
    case Lexer::kwInt:
    case Lexer::kwFloat:
    case Lexer::kwSampler1D:
    case Lexer::kwSampler2D:
    case Lexer::kwSampler3D:
    case Lexer::kwSamplerCUBE:
      return true;
    case Lexer::Identifier:
      /* Might be a type alias */
      {
	Name typeName = scope->ResolveIdentifier (currentToken.tokenString);
	return (typeName->GetType() == SemanticsHandler::Name::TypeAlias);
      }
      break;
    default:
      break;
    }
    return false;
  }
  
  Parser::Type Parser::ParseTypeBase (Scope scope)
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
      return Type ();
    }
  }
  
  Parser::Type Parser::ParseType (Scope scope)
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
  
  void Parser::ParseTypedef (Scope scope)
  {
    // Skip typedef
    NextToken();
    Type aliasedType = ParseType (scope);
    // Add to scope
    scope->AddTypeAlias (aliasedType, currentToken.tokenString);
    NextToken();
  }
  
  void Parser::ParseVarDeclare (Scope scope)
  {
    Type type = ParseType (scope);
    ParseVarIdentifierAndInitializerList (scope, type);
  }
  
  void Parser::ParseVarIdentifierAndInitializerList (Scope scope, Type type)
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
  
  void Parser::ParseVarIdentifierAndInitializer (Scope scope, Type type)
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
      
  void Parser::ParseConstDeclare (Scope scope)
  {
    NextToken(); // skip 'const'
    Type type = ParseType (scope);
    ParseConstIdentifierAndInitializerList (scope, type);
  }
  
  void Parser::ParseConstIdentifierAndInitializerList (Scope scope, Type type)
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
  
  void Parser::ParseConstIdentifierAndInitializer (Scope scope, Type type)
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
    
  void Parser::ParseIf (Block block)
  {
    NextToken();
    Expect (Lexer::ParenL);
    NextToken();
    Expression conditionExpr = ParseExpression (block->GetInnerScope());
    Expect (Lexer::ParenR);
    NextToken();
    Expect (Lexer::BraceL);
    NextToken();
    Block ifBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
    ParseBlock (ifBlock);
    Expect (Lexer::BraceR);
    NextToken();
    Block elseBlock;
    if (currentToken.typeOrID == Lexer::kwElse)
    {
      NextToken();
      Expect (Lexer::BraceL);
      NextToken();
      elseBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
      ParseBlock (elseBlock);
      Expect (Lexer::BraceR);
      NextToken();
    }
    block->AddBranching (conditionExpr, ifBlock, elseBlock);
  }
  
  void Parser::ParseLoopFor (Block block)
  {
    NextToken();
    Expect (Lexer::ParenL);
    NextToken();
    Expression initExpr = ParseExpression (block->GetInnerScope());
    Expect (Lexer::Semicolon);
    NextToken();
    Expression loopTestExpr = ParseExpression (block->GetInnerScope());
    Expect (Lexer::Semicolon);
    NextToken();
    Expression loopFootExpr = ParseExpression (block->GetInnerScope());
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
