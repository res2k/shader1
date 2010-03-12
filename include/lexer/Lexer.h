#ifndef __LEXER_LEXER_H__
#define __LEXER_LEXER_H__

#include <unicode/unistr.h>

#include "base/UnicodeStream.h"

namespace s1
{
  class LexerErrorHandler;
  
  class Lexer
  {
  public:
    enum TokenType
    {
      EndOfFile = -1,
      Invalid = 0,
      
      Identifier,
      Numeric,
      
      /// ';'
      Semicolon,
      
      /// '('
      ParenL,
      /// ')'
      ParenR,
      /// '['
      BracketL,
      /// ']'
      BracketR,
      /// '{'
      BraceL,
      /// '}'
      BraceR,
      
      /// '.'
      Member,
      /// ','
      Separator,
      
      /// '=='
      Equals,
      /// '!='
      NotEquals,
      /// '>'
      Larger,
      /// '>='
      LargerEqual,
      /// '<'
      Smaller,
      /// '<='
      SmallerEqual,
      
      /// '='
      Assign,
      
      /// '+'
      Plus,
      /// '-'
      Minus,
      /// '*'
      Mult,
      /// '/'
      Div,
      /// '%'
      Mod,
      
      /// '~'
      BitwiseInvert,
      /// '!'
      LogicInvert,
      
      /// '?'
      TernaryIf,
      /// ':'
      TernaryElse,
      
      /// '||'
      LogicOr,
      /// '&&'
      LogicAnd
    };
    
    struct Token
    {
      TokenType type;
      UnicodeString tokenString;
      
      Token () : type (Invalid) {}
      Token (TokenType type) : type (type) {}
      Token (TokenType type, const UnicodeString& tokenString)
       : type (type), tokenString (tokenString) {}
      Token (TokenType type, UChar32 tokenChar)
       : type (type), tokenString (tokenChar) {}
      Token (TokenType type, const char* tokenString)
       : type (type), tokenString (tokenString) {}
    };
    
    Lexer (UnicodeStream& inputChars, LexerErrorHandler& errorHandler);
    
    /// Returns whether more token are available
    operator bool() const throw();
    bool operator!() const throw() { return !(bool)(*this); }

    /**
     * Advance to next token.
     * Will not throw an exception, but may call into error handler.
     */
    Lexer& operator++() throw();
    
    /// Return current token
    Token operator* () const throw() { return currentToken; }
  protected:
    UnicodeStream& inputChars;
    LexerErrorHandler& errorHandler;
    
    Token currentToken;
    
    /// Current character. Set by NextChar().
    UChar32 currentChar;
    /// Read next character from input stream.
    void NextChar();
    /**
     * Put back a single character; this will be used as the "next" character
     * once.
     * Used in some rare cases where a character may need to be consumed again,
     * by a different code path, after it has been consumed by another.
     */
    void PutCharBack (UChar32 ch);
    /// "Put back" character, set by PutCharBack()
    UChar32 putback;
  };
} // namespace s1

#endif // __LEXER_LEXER_H__
