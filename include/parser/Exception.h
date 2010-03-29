#ifndef __PARSER_EXCEPTION_H__
#define __PARSER_EXCEPTION_H__

#include "ErrorCode.h"
#include "lexer/Lexer.h"

#include <exception>

namespace s1
{
  namespace parser
  {
    class Exception : public std::exception
    {
      ErrorCode code;
      Lexer::Token encounteredToken;
      Lexer::TokenType expectedToken;
    public:
      ~Exception() throw () {}
      
      Exception (ErrorCode code) : code (code), expectedToken (Lexer::Invalid) {}
      Exception (ErrorCode code, const Lexer::Token& encounteredToken,
		 Lexer::TokenType expectedToken = Lexer::Invalid)
       : code (code), encounteredToken (encounteredToken),
         expectedToken (expectedToken) {}
      
      ErrorCode GetCode() const { return code; }
      Lexer::Token GetEncounteredToken() const { return encounteredToken; }
      Lexer::TokenType GetExpectedToken() const { return expectedToken; }
    };
  } // namespace parser
} // namespace s1

#endif // __PARSER_EXCEPTION_H__
