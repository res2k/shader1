#ifndef __ERRORHANDLER_H__
#define __ERRORHANDLER_H__

#include "lexer/LexerErrorHandler.h"

class ErrorHandler : public s1::LexerErrorHandler
{
public:
  void InputInvalidCharacter () { }
  void StrayCharacter (UChar32 offender)
  {
  }
};

#endif // __ERRORHANDLER_H__
