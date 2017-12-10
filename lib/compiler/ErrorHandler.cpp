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

#include "intermediate/Diagnostics.h"
#include "lexer/Lexer.h"
#include "parser/Diagnostics.h"

#include <iostream>

namespace s1
{
namespace compiler
{
void ErrorHandler::LexerErrorImpl (unsigned int code, const lexer::TokenLocation& location)
{
  // TODO: Actually do something
}

void ErrorHandler::ParseErrorImpl (unsigned int code,
                                   const lexer::Token& encounteredToken,
                                   lexer::TokenType expectedToken)
{
  const char* errorStr = "???";
  switch (static_cast<parser::Error> (code))
  {
#define ERRORCODE(X)	\
  case parser::Error::X: errorStr = #X; break
  ERRORCODE(UnexpectedToken);
  ERRORCODE(IdentifierAlreadyDeclared);
  ERRORCODE(IdentifierUndeclared);
  ERRORCODE(DeclarationNotAllowedInScope);
  ERRORCODE(ExpectedTypeName);
  ERRORCODE(OutParameterWithDefault);
  ERRORCODE(ConflictingQualifiersForInParam);
  ERRORCODE(QualifiersNotAllowed);
#undef ERRORCODE
  }
  
  std::cerr << errorStr;
  std::cerr << "; ";
  if (encounteredToken.typeOrID != lexer::Invalid)
  {
    std::cerr << "encountered: '";
    std::cerr << Lexer::GetTokenStr (encounteredToken);
    std::cerr << "'; ";
  }
  if (expectedToken != lexer::Invalid)
  {
    std::cerr << "expected: '";
    std::cerr << Lexer::GetTokenStr (expectedToken);
    std::cerr << "'; ";
  }
  std::cerr << std::endl;
}

void ErrorHandler::SemanticErrorImpl (unsigned int code)
{
  const char* errorStr = "???";
  switch (static_cast<s1::intermediate::Error> (code))
  {
#define ERRORCODE(X)	\
  case intermediate::Error::X: errorStr = #X; break
  ERRORCODE(OperandTypesIncompatible);
  ERRORCODE(OperandTypesInvalid);
  ERRORCODE(AssignmentTypesIncompatible);
  ERRORCODE(AssignmentTargetIsNotAnLvalue);
  ERRORCODE(InvalidTypeCast);
  ERRORCODE(NoMatchingFunctionOverload);
  ERRORCODE(AmbiguousFunctionOverload);
  ERRORCODE(ActualParameterNotAnLvalue);
  ERRORCODE(TernaryExpressionTypesIncompatible);
  ERRORCODE(NumberParseError);
  ERRORCODE(TooManyTypeCtorArgs);
  ERRORCODE(TooFewTypeCtorArgs);
  ERRORCODE(InvalidAttribute);
  ERRORCODE(NotAnArray);
  ERRORCODE(IndexNotAnInteger);
  ERRORCODE(SwizzledExpressionNotAnLvalue);
  ERRORCODE(MultipleUseOfComponentInLvalueSwizzle);
  ERRORCODE(ArrayNotAnLvalue);
#undef ERRORCODE
  }
  
  std::cerr << errorStr;
  std::cerr << std::endl;
}

} // namespace compiler
} // namespace s1
