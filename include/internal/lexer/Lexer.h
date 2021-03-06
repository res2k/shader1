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

#include "Token.h"
#include "TokenLocation.h"
#include "TokenType.h"

#include "base/uc/Stream.h"
#include "base/uc/String.h"
#include "diagnostics/Handler.h"

#include <boost/unordered_map.hpp>

namespace s1
{
  class Lexer
  {
  public:
    typedef lexer::Token Token;
    typedef lexer::TokenLocation Location;
    typedef lexer::TokenType TokenType;
    
    Lexer (uc::Stream& inputChars, diagnostics::Handler& diagnosticsHandler);
    
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
    static uc::String GetTokenStr (const Token& token);
    //@}
  protected:
    uc::Stream& inputChars;
    diagnostics::Handler& diagnosticsHandler;
    
    typedef boost::unordered_map<uc::String, TokenType> KeywordMap;
    /// Map of identifier strings to keyword names
    KeywordMap keywords;
    
    Location currentLocation = Location{ 0, 0 };
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
    /// Severity of invalid characters
    enum struct InvalidInputSeverity { Error, Warning };
    /// Read next character from input stream.
    void NextChar (unsigned int buffer = bufferUse,
                   InvalidInputSeverity iis = InvalidInputSeverity::Error);
    enum { LookAhead = 2 };
    /// Next character in input stream. Set by NextChar().
    uc::Stream::FetchResult nextChar[LookAhead];
    /// Look ahead to a character
    uc::Char32 PeekChar (int la = 0) const;
  };
} // namespace s1

#endif // __LEXER_LEXER_H__
