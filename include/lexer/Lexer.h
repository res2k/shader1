#ifndef __LEXER_LEXER_H__
#define __LEXER_LEXER_H__

#include <unicode/unistr.h>

#include "base/UnicodeStream.h"

#include <boost/unordered_map.hpp>

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
      
      kwWhile,
      kwFor
      /** @} */
    };
    
    enum TypeClassification
    {
      Normal, Vector, Matrix
    };

    /// Token object
//@@SNIP LexerToken.txt
    // Token-Objekt
    struct Token
    {
//@@ENDSNIP
      /// Type/symbol/keyword ID of this token
//@@SNIP LexerToken.txt
      // Typ/Symbol/Schlüsselwort-ID dieses Tokens
      TokenType typeOrID;
//@@ENDSNIP
      /// Original string for this token
//@@SNIP LexerToken.txt
      // Originale Zeichenkette des Tokens
      UnicodeString tokenString;
//@@ENDSNIP
      /// Classification (normal, vector or matrix) for type keywords
//@@SNIP LexerToken.txt
      // Art (Normal, Vektor oder Matrix) von Typ-Schlüsselwörtern
      TypeClassification typeClass;
//@@ENDSNIP
      /// For vectors: vector dimension; For matrices: number of columns
//@@SNIP LexerToken.txt
      // Für Vektoren: Komponentanzahl; Für Matrizen: Anzahl Spalten
      int dimension1;
//@@ENDSNIP
      /// For matrices: number of rows
//@@SNIP LexerToken.txt
      // Für Matrizen: Anzahl Zeilen
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

    //@{
    /// Return token string
    static const char* GetTokenStr (TokenType token);
    static std::string GetTokenStr (const Token& token);
    //@}
  protected:
    UnicodeStream& inputChars;
    LexerErrorHandler& errorHandler;
    
    typedef boost::unordered_map<UnicodeString, TokenType> KeywordMap;
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
