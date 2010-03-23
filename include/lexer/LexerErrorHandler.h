#ifndef __LEXER_LEXERERRORHANDLER_H__
#define __LEXER_LEXERERRORHANDLER_H__

#include <unicode/utypes.h>

namespace s1
{
  struct LexerErrorHandler
  {
    virtual ~LexerErrorHandler() {}
    
    /**
     * Handler called when an invalid input sequence is encountered.
     * An example would be an incomplete encoding sequence in UTF-8.
     */
    virtual void InputInvalidCharacter () {}
    /**
     * Handler called when a character is found that can't occur in any
     * type of token.
     */
    virtual void StrayCharacter (UChar32 offender) {}
  };
} // namespace s1

#endif // __LEXER_LEXERERRORHANDLER_H__
