#ifndef __LEXER_LEXER_H__
#define __LEXER_LEXER_H__

#include <unicode/unistr.h>

#include "base/UnicodeStream.h"

#if defined(__GNUC__) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

namespace s1
{
  class LexerErrorHandler;
  
  class Lexer
  {
  public:
    enum TokenType
    {
      /// End of input was reached
      EndOfFile = -1,
      /// Invalid token (resulting from e.g. a stray character)
      Invalid = 0,
      
      /// Identifier
      Identifier,
      /// Numeric (hex or float number)
      Numeric,
      
      /**\name Operators, Symbols
       * @{ */
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
      LogicAnd,
      /** @} */
      
      /**\name Keywords
       * @{ */
      kwReturn,
      
      kwTrue,
      kwFalse,
      
      kwBool,
      kwUnsigned,
      kwInt,
      kwFloat,
      
      kwSampler1D,
      kwSampler2D,
      kwSampler3D,
      kwSamplerCUBE,
      
      kwTypedef,
      kwVoid,
      kwIn,
      kwOut,
      kwConst,
      
      kwIf,
      kwElse,
      
      kwWhile,
      /** @} */
      
      /**\name Keyword flags
       * @{ */
      /// Keyword is a vector type identifier
      kwfVector = 0x1000,
      /// Keyword is a matrix type identifier
      kwfMatrix = 0x2000
      /** @} */
    };
    
    /// Token object
    struct Token
    {
      /// Type of this token
      TokenType type;
      /// Input string for this token
      UnicodeString tokenString;
      /// For vectors: vector dimension; For matrices: number of columns
      int dimension1;
      /// For matrices: number of rows
      int dimension2;
      
      Token () : type (Invalid), dimension1 (0), dimension2 (0) {}
      Token (TokenType type) : type (type), dimension1 (0), dimension2 (0) {}
      Token (TokenType type, const UnicodeString& tokenString)
       : type (type), tokenString (tokenString), dimension1 (0), dimension2 (0) {}
      Token (TokenType type, UChar32 tokenChar)
       : type (type), tokenString (tokenChar), dimension1 (0), dimension2 (0) {}
      Token (TokenType type, const char* tokenString)
       : type (type), tokenString (tokenString), dimension1 (0), dimension2 (0) {}
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
    
    typedef std::tr1::unordered_map<UnicodeString, TokenType> KeywordMap;
    /// Map of identifier strings to keyword names
    KeywordMap keywords;
    
    Token currentToken;
    
    /// Parse identifier
    void ParseIdentifier ();
    /// Parse numeric value
    void ParseNumeric ();
    
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
