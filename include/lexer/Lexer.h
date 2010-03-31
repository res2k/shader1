#ifndef __LEXER_LEXER_H__
#define __LEXER_LEXER_H__

#include <unicode/unistr.h>

#include "base/UnicodeStream.h"

#include "base/unordered_map"

namespace s1
{
  class LexerErrorHandler;
  
  class Lexer
  {
  public:
    enum TokenType
    {
      /// Invalid token (resulting from e.g. a stray character)
      Invalid = -2,
      /// End of input was reached
      EndOfFile = -1,
      
      /// Unrecognized token
      Unknown = 0,
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
      
      kwWhile
      /** @} */
    };
    
    enum TypeClassification
    {
      Normal, Vector, Matrix
    };

//@@SNIP LexerToken.txt
    /// Token object
    struct Token
    {
      /// Type/symbol/keyword ID of this token
      TokenType typeOrID;
      /// Original string for this token
      UnicodeString tokenString;
      /// Classification (normal, vector or matrix) for type keywords
      TypeClassification typeClass;
      /// For vectors: vector dimension; For matrices: number of columns
      int dimension1;
      /// For matrices: number of rows
      int dimension2;
//@@ENDSNIP 

      Token () : typeOrID (Invalid), typeClass (Normal), dimension1 (0), dimension2 (0) {}
      Token (TokenType type) : typeOrID (type), typeClass (Normal), dimension1 (0), dimension2 (0) {}
      Token (TokenType type, const UnicodeString& tokenString)
       : typeOrID (type), tokenString (tokenString), typeClass (Normal), dimension1 (0), dimension2 (0) {}
      Token (TokenType type, UChar32 tokenChar)
       : typeOrID (type), tokenString (tokenChar), typeClass (Normal), dimension1 (0), dimension2 (0) {}
      Token (TokenType type, const char* tokenString)
       : typeOrID (type), tokenString (tokenString), typeClass (Normal), dimension1 (0), dimension2 (0) {}
//@@SNIP LexerToken.txt
    };
//@@ENDSNIP 
    
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
    void NextChar ();
    enum { LookAhead = 2 };
    /// Next character in input stream. Set by NextChar().
    UChar32 nextChar[LookAhead];
    /// Look ahead to a character
    UChar32 PeekChar (int la = 0) { return nextChar[la]; }
  };
} // namespace s1

#endif // __LEXER_LEXER_H__
