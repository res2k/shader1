#ifndef __ERRORHANDLER_H__
#define __ERRORHANDLER_H__

#include "intermediate/ErrorCode.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/ErrorHandler.h"

class ErrorHandler : public s1::LexerErrorHandler,
		     public s1::parser::ErrorHandler
{
public:
  /**\name s1::LexerErrorHandler
   * @{ */
  void InputInvalidCharacter () { }
  void StrayCharacter (UChar32 offender)
  {
  }
  /** @} */
  
  /**\name s1::LexerErrorHandler
   * @{ */
  void ParseError (s1::parser::ErrorCode code, const s1::Lexer::Token& encounteredToken,
		   s1::Lexer::TokenType expectedToken);
  /** @} */
  
  void IntermediateError (s1::intermediate::ErrorCode code);
};

#endif // __ERRORHANDLER_H__
