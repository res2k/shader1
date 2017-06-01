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

#include "base/common.h"
#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"

#include "base/format/Formatter.h"
#include "base/format/std_string.h"

#include <assert.h>
#include <stdio.h>

#include "base/format/Formatter.tpp"

namespace s1
{
#define KEYWORDS				\
  KEYWORD ("return",		kwReturn)	\
  KEYWORD ("true",		kwTrue)		\
  KEYWORD ("false",		kwFalse)	\
  KEYWORD ("bool",		kwBool)		\
  KEYWORD ("unsigned",		kwUnsigned)	\
  KEYWORD ("int",		kwInt)		\
  KEYWORD ("float",		kwFloat)	\
  KEYWORD ("sampler1D",		kwSampler1D)	\
  KEYWORD ("sampler2D",		kwSampler2D)	\
  KEYWORD ("sampler3D",		kwSampler3D)	\
  KEYWORD ("samplerCUBE",	kwSamplerCUBE)	\
  KEYWORD ("typedef",		kwTypedef)	\
  KEYWORD ("void",		kwVoid)		\
  KEYWORD ("in",		kwIn)		\
  KEYWORD ("out",		kwOut)		\
  KEYWORD ("const",		kwConst)        \
  KEYWORD ("uniform",		kwUniform)      \
  KEYWORD ("attribute",		kwAttribute)	\
  KEYWORD ("if",		kwIf)		\
  KEYWORD ("else",		kwElse)		\
  KEYWORD ("while",		kwWhile)	\
  KEYWORD ("for",		kwFor)
  
  Lexer::Lexer (uc::Stream& inputChars, LexerErrorHandler& errorHandler)
   : inputChars (inputChars), errorHandler (errorHandler),
     currentToken (EndOfFile)
  {
#define KEYWORD(Str, Symbol)	\
    keywords[uc::String (Str)] 		= Symbol;
KEYWORDS
#undef KEYWORD

    // Invalidate lookahead
    for (int i = 0; i < LookAhead; i++)
      nextChar[i] = uc::InvalidChar32;
    
    // Fill lookahead characters
    for (int i = 0; i < LookAhead; i++)
      NextChar (bufferSkip);
    // ... and one to set the actual current char
    NextChar ();
    
    if ((bool)*this)
      ++(*this);
  }
   
  Lexer::operator bool() const throw()
  {
    return (currentToken.typeOrID != EndOfFile) || (currentChar != uc::InvalidChar32) || inputChars;
  }

  Lexer& Lexer::operator++() throw()
  {
    /* This is a loop as, after a comment was handled, we still have to obtain
       the real desired 'next' token */
    while (true)
    {
      // Skip whitespace
      while (uc::IsWhiteSpace (currentChar)) NextChar (bufferClearPrevious);
      
      // Check for end of input
      if (currentChar == uc::InvalidChar32)
      {
        currentToken = Token (EndOfFile);
        return *this;
      }
      // Record token location after whitespace skipping
      currentTokenLocation = currentLocation;
      
      // Check if it's a "simple" token (can only start with a single known character)
      switch (currentChar)
      {
      case ';': currentToken = MakeToken (Semicolon); NextChar (); return *this;
      case '(': currentToken = MakeToken (ParenL);    NextChar (); return *this;
      case ')': currentToken = MakeToken (ParenR);    NextChar (); return *this;
      case '[': currentToken = MakeToken (BracketL);  NextChar (); return *this;
      case ']': currentToken = MakeToken (BracketR);  NextChar (); return *this;
      case '{': currentToken = MakeToken (BraceL);    NextChar (); return *this;
      case '}': currentToken = MakeToken (BraceR);    NextChar (); return *this;
      case '.':
        {
          uc::Char32 next = PeekChar();
          if ((next >= '0') && (next <= '9'))
          {
            // '.' is start of a number
            break;
          }
          else
          {
            // '.' is member operator
            currentToken = MakeToken (Member); 
            NextChar ();
            return *this;
          }
        }
      case ',': currentToken = MakeToken (Separator); NextChar (); return *this;
      case '=':
        {
          if (PeekChar() == '=')
          {
            NextChar ();
            currentToken = MakeToken (Equals);
          }
          else
            currentToken = MakeToken (Assign);
          NextChar ();
        }
        return *this;
      case '!':
        {
          if (PeekChar() == '=')
          {
            NextChar ();
            currentToken = MakeToken (NotEquals);
          }
          else
            currentToken = MakeToken (LogicInvert);
          NextChar ();
        }
        return *this;
      case '>':
        {
          if (PeekChar() == '=')
          {
            NextChar ();
            currentToken = MakeToken (LargerEqual);
          }
          else
            currentToken = MakeToken (Larger);
          NextChar ();
        }
        return *this;
      case '<':
        {
          if (PeekChar() == '=')
          {
            NextChar ();
            currentToken = MakeToken (SmallerEqual);
          }
          else
            currentToken = MakeToken (Smaller);
          NextChar ();
        }
        return *this;
      case '+': currentToken = MakeToken (Plus);  NextChar (); return *this;
      case '-': currentToken = MakeToken (Minus); NextChar (); return *this;
      case '*': currentToken = MakeToken (Mult);  NextChar (); return *this;
      case '/':
        {
          if (ParseComment ())
          {
            continue;
          }
          else
          {
            // Division operator
            currentToken = MakeToken (Div);
            NextChar ();
          }
        }
        return *this;
      case '%': currentToken = MakeToken (Mod); 	        NextChar (); return *this;
      case '~': currentToken = MakeToken (BitwiseInvert); NextChar (); return *this;
      case '?': currentToken = MakeToken (TernaryIf);     NextChar (); return *this;
      case ':': currentToken = MakeToken (TernaryElse);   NextChar (); return *this;
      case '&':
        {
          uc::Char32 next = PeekChar();
          if (next == '&')
          {
            NextChar ();
            currentToken = MakeToken (LogicAnd);
            // Skip second '&'
            NextChar ();
          }
          else
          {
            // Lone '&' not a valid token. Let stray character handling deal with it
            break;
          }
        }
        return *this;
      case '|':
        {
          uc::Char32 next = PeekChar();
          if (next == '|')
          {
            NextChar ();
            currentToken = MakeToken (LogicOr);
            // Skip second '|'
            NextChar ();
          }
          else
          {
            // Lone '|' not a valid token. Let stray character handling deal with it
            break;
          }
        }
        return *this;
      }
      
      if ((currentChar == '_') || uc::IsIDStart (currentChar))
      {
        // Identifier
        ParseIdentifier ();
      }
      else if ((currentChar == '.') || ((currentChar >= '0') && (currentChar <= '9')))
      {
        // Number
        ParseNumeric ();
      }
      else
      {
        if (currentChar != 0xfffd)
        {
          /* Replacement character (0xFFFD) indicates an invalid input sequence.
             Error handler was already called for that. */
          errorHandler.StrayCharacter (currentChar);
          currentToken = MakeToken (Unknown);
        }
        else
          /* Otherwise, it's an unrecognized character. */
          currentToken = MakeToken (Invalid);
        NextChar ();
      }
      
      return *this;
    }
  }

  Lexer::Token Lexer::MakeToken (TokenType type)
  {
    /* Normalize token string.
       Spec wants canonically equivalent character sequences to be treated
       identical for identifiers. Unicode 5.2 Ch. 3 Def. D70, which defines
       canonical equivalence as identical canonical decompositions, is
       explicitly refered to. Hence normalize token string to NFD (canonical
       decomposition).
     */
    auto tokenStr = tokenBuffer.Normalized (uc::String::normNFD);

    auto nextToken = Token (type, tokenStr);
    tokenBuffer.clear ();
    nextToken.location = currentTokenLocation;

    return nextToken;
  }

  void Lexer::ParseIdentifier ()
  {
    while (uc::IsIDContinue (PeekChar()))
    {
      NextChar ();
    }

    currentToken = MakeToken (Identifier);
    auto tokenStr = currentToken.tokenString;
    KeywordMap::iterator kwType = keywords.find (tokenStr);
    if (kwType != keywords.end())
    {
      // Identifier is a well-known keyword
      currentToken.typeOrID = kwType->second;
    }
    else
    {
      /* Special handling of vector/matrix types.
         Instead of specifying a new enum for each possible vector or matrix
         type parse these specially by manually extracting the dimension(s). */
      TokenType typeCandidate = Invalid;
      uc::String dimensions;
      if (tokenStr.startsWith ("int"))
      {
        typeCandidate = kwInt;
        dimensions = uc::String (tokenStr, 3);
      }
      else if (tokenStr.startsWith ("float"))
      {
        typeCandidate = kwFloat;
        dimensions = uc::String (tokenStr, 5);
      }
      else if (tokenStr.startsWith ("bool"))
      {
        typeCandidate = kwBool;
        dimensions = uc::String (tokenStr, 4);
      }
      if (typeCandidate != Invalid)
      {
        auto dimensionsBuf = dimensions.data ();
        if ((dimensions.length() == 1)
            && (dimensionsBuf[0] >= '1')
            && (dimensionsBuf[0] <= '4'))
        {
          // It's a vector!
          currentToken.typeOrID = typeCandidate;
          currentToken.typeClass = Vector;
          currentToken.dimension1 = dimensionsBuf[0] - '1' + 1;
        }
        else if ((dimensions.length() == 3)
            && (dimensionsBuf[0] >= '1')
            && (dimensionsBuf[0] <= '4')
            && (dimensionsBuf[1] == 'x')
            && (dimensionsBuf[2] >= '1')
            && (dimensionsBuf[2] <= '4'))
        {
          // It's a matrix!
          currentToken.typeOrID = typeCandidate;
          currentToken.typeClass = Matrix;
          currentToken.dimension1 = dimensionsBuf[0] - '1' + 1;
          currentToken.dimension2 = dimensionsBuf[2] - '1' + 1;
        }
      }
    }

    NextChar ();
  }
  
  void Lexer::ParseNumeric()
  {
    if (currentChar == '0')
    {
      uc::Char32 next = PeekChar();
      if ((next == 'x') || (next == 'X'))
      {
        // Hex number
        NextChar ();
        next = PeekChar();
        bool hadNumeric = false;
        while (((next >= '0') && (next <= '9'))
          || ((next >= 'a') && (next <= 'f'))
          || ((next >= 'A') && (next <= 'F')))
        {
          NextChar ();
          next = PeekChar();
          hadNumeric = true;
        }
        currentToken = MakeToken (hadNumeric ? Numeric : Invalid);
        NextChar ();
        return;
      }
    }

    // Float number
    bool hasDecimal = currentChar == '.';
    bool hasExp = false;
    while (true)
    {
      uc::Char32 next = PeekChar();
      if ((next == '.') && !hasDecimal)
      {
        NextChar ();
        hasDecimal = true;
      }
      else if (((next == 'E') || (next == 'e')) && !hasExp)
      {
        NextChar ();
        hasExp = true;
        // Force to 'true' since decimal exponents are not allowed
        hasDecimal = true;
        // Accept '-' again, once
        uc::Char32 next = PeekChar();
        if (next == '-')
        {
          NextChar ();
        }
      }
      else if ((next >= '0') && (next <= '9'))
      {
        // Collect
        NextChar ();
        continue;
      }
      else
      {
        // End of number
        break;
      }
    }
    
    currentToken = MakeToken (Numeric);
    NextChar ();
  }

  bool Lexer::ParseComment ()
  {
    S1_ASSERT(currentChar == '/', false);
    uc::Char32 next = PeekChar();
    if (next == '/')
    {
      NextChar (bufferClearPrevious | bufferSkip);
      // Line comment
      do
      {
        NextChar (bufferSkip);
      }
      while ((currentChar != uc::InvalidChar32) && (currentChar != '\n'));
      // Let '\n' be handled by regular code
      return true;
    }
    else if (next == '*')
    {
      NextChar (bufferClearPrevious | bufferSkip);
      // Block comment
      do
      {
        NextChar (bufferSkip);
        if (currentChar == '*')
        {
          NextChar (bufferSkip);
          // Check for ending '*/'
          if (currentChar == '/')
          {
            NextChar (bufferSkip);
            break;
          }
        }
      }
      while (currentChar != uc::InvalidChar32);
      return true;
    }
    // else: something else
    return false;
  }

  void Lexer::NextChar (unsigned int buffer)
  {
    if ((buffer & bufferClearPrevious) != 0) tokenBuffer.clear();

    /* Advance location *before* fetching next char,
     * so it truly points to the 'current' character
     * (otherwise it'd be the next one) */
    if (currentChar != uc::InvalidChar32)
    {
      if (currentChar == '\n')
      {
        currentLocation.line++;
        currentLocation.column = 0;
      }
      else
      {
        currentLocation.column++;
      }
    }

    currentChar = nextChar[0];
    if ((currentChar != uc::InvalidChar32) && ((buffer & bufferSkip) == 0))
    {
      tokenBuffer.reserveExtra (1, uc::String::quantumGrow);
      tokenBuffer.append (currentChar);
    }
    for (int i = 1; i < LookAhead; i++)
      nextChar[i-1] = nextChar[i];
    if (inputChars)
    {
      auto streamNextChar = *inputChars;
      if (streamNextChar)
        nextChar[LookAhead - 1] = streamNextChar.value ();
      else
      {
        // Signal error handler ...
        errorHandler.InputInvalidCharacter();
        // ... and set character to the 'replacer' one
        nextChar[LookAhead-1] = 0xfffd;
      }
      ++inputChars;
    }
    else
    {
      // If at the end of input, return -1
      nextChar[LookAhead-1] = -1;
    }
  }

  const char* Lexer::GetTokenStr (TokenType token)
  {
    switch (token)
    {
#define KEYWORD(Str, Symbol)	\
    case Symbol: return Str;
KEYWORDS
#undef KEYWORD

    case Invalid:		return "<Invalid>";
    case EndOfFile:		return "<EOF>";
    case Unknown:		return "<Unknown>";
    case Identifier:		return "<Identifier>";
    case Numeric:		return "<Numeric>";
    
    case Semicolon:		return ";";
    case ParenL:		return "(";
    case ParenR:		return ")";
    case BracketL:		return "[";
    case BracketR:		return "]";
    case BraceL:		return "{";
    case BraceR:		return "}";
    case Member:		return ".";
    case Separator:		return ",";
    case Equals:		return "==";
    case NotEquals:		return "!=";
    case Larger:		return ">";
    case LargerEqual:		return ">=";
    case Smaller:		return "<";
    case SmallerEqual:		return "<=";
    case Assign:		return "=";
    case Plus:			return "+";
    case Minus:			return "-";
    case Mult:			return "*";
    case Div:			return "/";
    case Mod:			return "%";
    case BitwiseInvert:		return "~";
    case LogicInvert:		return "!";
    case TernaryIf:		return "?";
    case TernaryElse:		return ":";
    case LogicOr:		return "||";
    case LogicAnd:		return "&&";
    }
    return 0;
  }

  format::StaticFormatter FormatVector ("{0}{1}");
  format::StaticFormatter FormatMatrix ("{0}{1}x{2}");
  
  std::string Lexer::GetTokenStr (const Token& token)
  {
    const char* baseTokenStr (GetTokenStr (token.typeOrID));
    if (token.typeClass == Vector)
    {
      std::string s;
      FormatVector (s, baseTokenStr, token.dimension1);
      return s;
    }
    else if (token.typeClass == Matrix)
    {
      std::string s;
      FormatMatrix (s, baseTokenStr, token.dimension1, token.dimension2);
      return s;
    }
    return baseTokenStr;
  }
} // namespace s1
