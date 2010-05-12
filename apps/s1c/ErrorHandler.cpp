#include "ErrorHandler.h"

#include <iostream>

using namespace s1;

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
    std::cerr << "encountered: '";
    std::cerr << Lexer::GetTokenStr (expectedToken);
    std::cerr << "'; ";
  }
  std::cerr << std::endl;
}
