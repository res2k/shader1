#include "parser/Parser.h"

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
    if (currentToken.typeOrID != tokenType) UnexpectedToken();
  }
  
  void Parser::UnexpectedToken ()
  {
    /* should throw or so */
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
