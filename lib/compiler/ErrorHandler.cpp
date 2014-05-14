/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include "base/common.h"

#include "ErrorHandler.h"

#include <iostream>

namespace s1
{
namespace compiler
{
  
void ErrorHandler::ParseError (parser::ErrorCode code, const Lexer::Token& encounteredToken,
			       Lexer::TokenType expectedToken)
{
  const char* errorStr = "???";
  switch (code)
  {
#define ERRORCODE(X)	\
  case parser::X: errorStr = #X; break
  ERRORCODE(UnexpectedToken);
  ERRORCODE(IdentifierAlreadyDeclared);
  ERRORCODE(IdentifierUndeclared);
  ERRORCODE(DeclarationNotAllowedInScope);
  ERRORCODE(ExpectedTypeName);
  ERRORCODE(OutParameterWithDefault);
#undef ERRORCODE
  }
  
  std::cerr << errorStr;
  std::cerr << "; ";
  if (encounteredToken.typeOrID != Lexer::Invalid)
  {
    std::cerr << "encountered: '";
    std::cerr << Lexer::GetTokenStr (encounteredToken);
    std::cerr << "'; ";
  }
  if (expectedToken != Lexer::Invalid)
  {
    std::cerr << "expected: '";
    std::cerr << Lexer::GetTokenStr (expectedToken);
    std::cerr << "'; ";
  }
  std::cerr << std::endl;
}

void ErrorHandler::IntermediateError (s1::intermediate::ErrorCode code)
{
  const char* errorStr = "???";
  switch (code)
  {
#define ERRORCODE(X)	\
  case intermediate::X: errorStr = #X; break
  ERRORCODE(OperandTypesIncompatible);
  ERRORCODE(OperandTypesInvalid);
  ERRORCODE(AssignmentTypesIncompatible);
  ERRORCODE(AssignmentTargetIsNotAnLValue);
  ERRORCODE(InvalidTypeCast);
  ERRORCODE(NoMatchingFunctionOverload);
  ERRORCODE(AmbiguousFunctionOverload);
  ERRORCODE(ActualParameterNotAnLValue);
  ERRORCODE(TernaryExpressionTypesIncompatible);
  ERRORCODE(NumberParseError);
  ERRORCODE(TooManyTypeCtorArgs);
  ERRORCODE(TooFewTypeCtorArgs);
  ERRORCODE(InvalidAttribute);
  ERRORCODE(NotAnArray);
  ERRORCODE(IndexNotAnInteger);
  ERRORCODE(SwizzledExpressionNotAnLValue);
  ERRORCODE(MultipleUseOfComponentInLValueSwizzle);
  ERRORCODE(ArrayNotAnLValue);
#undef ERRORCODE
  }
  
  std::cerr << errorStr;
  std::cerr << std::endl;
}

} // namespace compiler
} // namespace s1
