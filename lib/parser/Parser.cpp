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
    currentToken = *inputLexer;
    ++inputLexer;
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
  
  Parser::Type Parser::ParseTypeBase ()
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
      /* Type alias */
      break;
    default:
      UnexpectedToken ();
      return Type ();
    }
  }
  
  Parser::Type Parser::ParseType ()
  {
    Type type = ParseTypeBase ();
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
  
  //void ParseTypedef ();
  
} // namespace s1
