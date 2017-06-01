/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __LEXER_LEXER_H__
#define __LEXER_LEXER_H__

#include "base/uc/Stream.h"
#include "base/uc/String.h"

#include <boost/unordered_map.hpp>

namespace s1
{
  struct LexerErrorHandler;
  
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
      kwUniform,
      kwAttribute,
      
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

    /// Token location in the source
    struct Location
    {
      /// Line number (0-based)
      unsigned int line = 0;
      /// Columns number (0-based), in code points
      unsigned int column = 0;
    };

    /// Token object
    struct Token
    {
      /// Type/symbol/keyword ID of this token
      TokenType typeOrID = Invalid;
      /// Original string for this token
      uc::String tokenString;
      /// Classification (normal, vector or matrix) for type keywords
      TypeClassification typeClass = Normal;
      /// For vectors: vector dimension; For matrices: number of columns
      int dimension1 = 0;
      /// For matrices: number of rows
      int dimension2 = 0;

      /// Location of token in source
      Location location;

      Token () {}
      Token (TokenType type) : typeOrID (type) {}
      Token (TokenType type, const uc::String& tokenString) : typeOrID (type), tokenString (tokenString) {}
      Token (TokenType type, uc::Char32 tokenChar) : typeOrID (type), tokenString (tokenChar) {}
      Token (TokenType type, const char* tokenString) : typeOrID (type), tokenString (tokenString) {}
    };
    
    Lexer (uc::Stream& inputChars, LexerErrorHandler& errorHandler);
    
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
    uc::Stream& inputChars;
    LexerErrorHandler& errorHandler;
    
    typedef boost::unordered_map<uc::String, TokenType> KeywordMap;
    /// Map of identifier strings to keyword names
    KeywordMap keywords;
    
    Location currentLocation;
    Token currentToken;
    Location currentTokenLocation;

    /// Create a token using tokenBuffer
    Token MakeToken (TokenType type);
    
    /// Parse identifier
    void ParseIdentifier ();
    /// Parse numeric value
    void ParseNumeric ();
    /// Parse a comment. Current character must be leading '/' and will be consumed.
    bool ParseComment ();
    
    /// Current character. Set by NextChar().
    uc::Char32 currentChar = uc::InvalidChar32;
    /// Buffer for currently parse token
    uc::String tokenBuffer;
    /// Controls whether character should be added to tokenBuffer
    enum TokenBuffer { bufferUse = 0, bufferSkip = 0x1, bufferClearPrevious = 0x2 };
    /// Read next character from input stream.
    void NextChar (unsigned int buffer = bufferUse);
    enum { LookAhead = 2 };
    /// Next character in input stream. Set by NextChar().
    uc::Char32 nextChar[LookAhead];
    /// Look ahead to a character
    uc::Char32 PeekChar (int la = 0) const { return nextChar[la]; }
  };
} // namespace s1

#endif // __LEXER_LEXER_H__
