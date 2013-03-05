#ifndef __PARSER_ERRORHANDLER_H__
#define __PARSER_ERRORHANDLER_H__

#include "ErrorCode.h"
#include "lexer/Lexer.h"

namespace s1
{
  namespace parser
  {
    struct ErrorHandler
    {
      virtual void ParseError (ErrorCode code, const Lexer::Token& encounteredToken,
			       Lexer::TokenType expectedToken) {}
    };
  } // namespace parser
} // namespace s1

#endif // __PARSER_ERRORHANDLER_H__
