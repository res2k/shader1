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

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "lexer/Diagnostics.h"

#include <assert.h>
#include <stdio.h>

#include "base/format/Formatter.ipp"

namespace s1
{
#define KEYWORDS                                        \
  KEYWORD ("return",            lexer::kwReturn)        \
  KEYWORD ("true",              lexer::kwTrue)          \
  KEYWORD ("false",             lexer::kwFalse)         \
  KEYWORD ("bool",              lexer::kwBool)          \
  KEYWORD ("unsigned",          lexer::kwUnsigned)      \
  KEYWORD ("int",               lexer::kwInt)           \
  KEYWORD ("float",             lexer::kwFloat)         \
  KEYWORD ("sampler1D",         lexer::kwSampler1D)     \
  KEYWORD ("sampler2D",         lexer::kwSampler2D)     \
  KEYWORD ("sampler3D",         lexer::kwSampler3D)     \
  KEYWORD ("samplerCUBE",       lexer::kwSamplerCUBE)   \
  KEYWORD ("typedef",           lexer::kwTypedef)       \
  KEYWORD ("void",              lexer::kwVoid)          \
  KEYWORD ("in",                lexer::kwIn)            \
  KEYWORD ("out",               lexer::kwOut)           \
  KEYWORD ("const",             lexer::kwConst)         \
  KEYWORD ("uniform",           lexer::kwUniform)       \
  KEYWORD ("attribute",         lexer::kwAttribute)     \
  KEYWORD ("if",                lexer::kwIf)            \
  KEYWORD ("else",              lexer::kwElse)          \
  KEYWORD ("while",             lexer::kwWhile)         \
  KEYWORD ("for",               lexer::kwFor)
  
  Lexer::Lexer (uc::Stream& inputChars, diagnostics::Handler& diagnosticsHandler)
   : inputChars (inputChars), diagnosticsHandler (diagnosticsHandler),
     currentToken (lexer::EndOfFile)
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
    return (currentToken.typeOrID != lexer::EndOfFile) || (currentChar != uc::InvalidChar32) || inputChars;
  }

  /* Helper functions to treat the replacement character as part of an Identifier.
   * Although the replacement character (0xFFFD) indicates an invalid input sequence
   * it's useful to treat it as an Identifier character this lets the parser produce
   * somewhat better output.
   * The Error handler will have been already called for the invalid input anyway.
   */
  static inline bool IsIDStartOrReplacer (uc::Char32 ch)
  {
    return uc::IsIDStart (ch) || (ch == uc::ReplacementChar);
  }

  static inline bool IsIDContinueOrReplacer (uc::Char32 ch)
  {
    return uc::IsIDContinue (ch) || (ch == uc::ReplacementChar);
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
        currentToken = Token (lexer::EndOfFile);
        return *this;
      }
      // Record token location after whitespace skipping
      currentTokenLocation = currentLocation;
      
      // Check if it's a "simple" token (can only start with a single known character)
      switch (currentChar)
      {
      case ';': currentToken = MakeToken (lexer::Semicolon); NextChar (); return *this;
      case '(': currentToken = MakeToken (lexer::ParenL);    NextChar (); return *this;
      case ')': currentToken = MakeToken (lexer::ParenR);    NextChar (); return *this;
      case '[': currentToken = MakeToken (lexer::BracketL);  NextChar (); return *this;
      case ']': currentToken = MakeToken (lexer::BracketR);  NextChar (); return *this;
      case '{': currentToken = MakeToken (lexer::BraceL);    NextChar (); return *this;
      case '}': currentToken = MakeToken (lexer::BraceR);    NextChar (); return *this;
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
            currentToken = MakeToken (lexer::Member); 
            NextChar ();
            return *this;
          }
        }
      case ',': currentToken = MakeToken (lexer::Separator); NextChar (); return *this;
      case '=':
        {
          if (PeekChar() == '=')
          {
            NextChar ();
            currentToken = MakeToken (lexer::Equals);
          }
          else
            currentToken = MakeToken (lexer::Assign);
          NextChar ();
        }
        return *this;
      case '!':
        {
          if (PeekChar() == '=')
          {
            NextChar ();
            currentToken = MakeToken (lexer::NotEquals);
          }
          else
            currentToken = MakeToken (lexer::LogicInvert);
          NextChar ();
        }
        return *this;
      case '>':
        {
          if (PeekChar() == '=')
          {
            NextChar ();
            currentToken = MakeToken (lexer::LargerEqual);
          }
          else
            currentToken = MakeToken (lexer::Larger);
          NextChar ();
        }
        return *this;
      case '<':
        {
          if (PeekChar() == '=')
          {
            NextChar ();
            currentToken = MakeToken (lexer::SmallerEqual);
          }
          else
            currentToken = MakeToken (lexer::Smaller);
          NextChar ();
        }
        return *this;
      case '+': currentToken = MakeToken (lexer::Plus);  NextChar (); return *this;
      case '-': currentToken = MakeToken (lexer::Minus); NextChar (); return *this;
      case '*': currentToken = MakeToken (lexer::Mult);  NextChar (); return *this;
      case '/':
        {
          if (ParseComment ())
          {
            continue;
          }
          else
          {
            // Division operator
            currentToken = MakeToken (lexer::Div);
            NextChar ();
          }
        }
        return *this;
      case '%': currentToken = MakeToken (lexer::Mod);           NextChar (); return *this;
      case '~': currentToken = MakeToken (lexer::BitwiseInvert); NextChar (); return *this;
      case '?': currentToken = MakeToken (lexer::TernaryIf);     NextChar (); return *this;
      case ':': currentToken = MakeToken (lexer::TernaryElse);   NextChar (); return *this;
      case '&':
        {
          uc::Char32 next = PeekChar();
          if (next == '&')
          {
            NextChar ();
            currentToken = MakeToken (lexer::LogicAnd);
            // Skip second '&'
            NextChar ();
          }
          else
          {
            // Lone '&' not a valid token
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
            currentToken = MakeToken (lexer::LogicOr);
            // Skip second '|'
            NextChar ();
          }
          else
          {
            // Lone '|' not a valid token
            break;
          }
        }
        return *this;
      }
      
      if ((currentChar == '_') || IsIDStartOrReplacer (currentChar))
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
        /* It's an unrecognized character. */
        currentToken = MakeToken (lexer::Invalid);
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

  static inline bool IsNumeric (uc::Char32 ch)
  {
    return (ch >= '0') && (ch <= '9');
  }

  static inline bool IsDimensionChar (uc::Char32 ch)
  {
    return (ch >= '1') && (ch <= '4');
  }

  void Lexer::ParseIdentifier ()
  {
    while (IsIDContinueOrReplacer (PeekChar()))
    {
      NextChar ();
    }

    currentToken = MakeToken (lexer::Identifier);
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
      TokenType typeCandidate = lexer::Invalid;
      uc::String dimensions;
      if (tokenStr.startsWith ("int"))
      {
        typeCandidate = lexer::kwInt;
        dimensions = uc::String (tokenStr, 3);
      }
      else if (tokenStr.startsWith ("float"))
      {
        typeCandidate = lexer::kwFloat;
        dimensions = uc::String (tokenStr, 5);
      }
      else if (tokenStr.startsWith ("bool"))
      {
        typeCandidate = lexer::kwBool;
        dimensions = uc::String (tokenStr, 4);
      }
      if (typeCandidate != lexer::Invalid)
      {
        auto dimensionsBuf = dimensions.data ();
        if ((dimensions.length() == 1) && IsNumeric (dimensionsBuf[0]))
        {
          if (IsDimensionChar (dimensionsBuf[0]))
          {
            // It's a vector!
            currentToken.typeOrID = static_cast<lexer::TokenType> (typeCandidate | lexer::VecFlag);
            currentToken.dimension1 = dimensionsBuf[0] - '1' + 1;
          }
          else
            diagnosticsHandler.LexerError (lexer::Warning::VectorLookalike, currentToken.location);
        }
        else if ((dimensions.length() == 3)
            && IsNumeric (dimensionsBuf[0])
            && (dimensionsBuf[1] == 'x')
            && IsNumeric (dimensionsBuf[2]))
        {
          if (IsDimensionChar (dimensionsBuf[0]) && IsDimensionChar (dimensionsBuf[2]))
          {
            // It's a matrix!
            currentToken.typeOrID =  static_cast<lexer::TokenType> (typeCandidate | lexer::MatFlag);
            currentToken.dimension1 = dimensionsBuf[0] - '1' + 1;
            currentToken.dimension2 = dimensionsBuf[2] - '1' + 1;
          }
          else
            diagnosticsHandler.LexerError (lexer::Warning::MatrixLookalike, currentToken.location);
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
        currentToken = MakeToken (hadNumeric ? lexer::Numeric : lexer::Invalid);
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
    
    currentToken = MakeToken (lexer::Numeric);
    NextChar ();
  }

  bool Lexer::ParseComment ()
  {
    S1_ASSERT(currentChar == '/', false);
    auto startLocation = currentLocation;
    uc::Char32 next = PeekChar();
    if (next == '/')
    {
      NextChar (bufferClearPrevious | bufferSkip);
      // Line comment
      do
      {
        NextChar (bufferSkip, InvalidInputSeverity::Warning);
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
        NextChar (bufferSkip, InvalidInputSeverity::Warning);
        if (currentChar == '*')
        {
          NextChar (bufferSkip, InvalidInputSeverity::Warning);
          // Check for ending '*/'
          if (currentChar == '/')
          {
            NextChar (bufferSkip);
            return true;
          }
        }
      }
      while (currentChar != uc::InvalidChar32);
      if (currentChar == uc::InvalidChar32)
        diagnosticsHandler.LexerError (lexer::Warning::UnterminatedBlockComment, startLocation);
      return true;
    }
    // else: something else
    return false;
  }

  void Lexer::NextChar (unsigned int buffer, InvalidInputSeverity iis)
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

    auto streamNextChar = nextChar[0];
    if (streamNextChar)
      currentChar = streamNextChar.value ();
    else if (streamNextChar.error () == uc::Stream::Error::EndOfInput)
    {
      currentChar = uc::InvalidChar32;
    }
    else
    {
      // Signal error handler ...
      if (iis == InvalidInputSeverity::Error)
        diagnosticsHandler.LexerError (lexer::Error::InvalidInputSequence, currentLocation);
      else
        diagnosticsHandler.LexerError (lexer::Warning::InvalidInputSequence, currentLocation);
      // ... and set character to the 'replacer' one
      currentChar = uc::ReplacementChar;
    }

    if ((currentChar != uc::InvalidChar32) && ((buffer & bufferSkip) == 0))
    {
      tokenBuffer.reserveExtra (1, uc::String::quantumGrow);
      tokenBuffer.append (currentChar);
    }

    for (int i = 1; i < LookAhead; i++)
      nextChar[i-1] = nextChar[i];
    nextChar[LookAhead - 1] = *inputChars;
    ++inputChars;
  }

  uc::Char32 Lexer::PeekChar (int la) const
  {
    if (nextChar[la])
      return nextChar[la].value ();
    else if (nextChar[la].error () == uc::Stream::Error::EndOfInput)
      return uc::InvalidChar32;
    else
      return uc::ReplacementChar;
  }

  const char* Lexer::GetTokenStr (TokenType token)
  {
    switch (token)
    {
#define KEYWORD(Str, Symbol)	\
    case Symbol: return Str;
KEYWORDS
#undef KEYWORD

    case lexer::Invalid:        return "<Invalid>";
    case lexer::EndOfFile:      return "<EOF>";
    case lexer::Unknown:        return "<Unknown>";
    case lexer::Identifier:     return "<Identifier>";
    case lexer::Numeric:        return "<Numeric>";
    
    case lexer::Semicolon:      return ";";
    case lexer::ParenL:         return "(";
    case lexer::ParenR:         return ")";
    case lexer::BracketL:       return "[";
    case lexer::BracketR:       return "]";
    case lexer::BraceL:         return "{";
    case lexer::BraceR:         return "}";
    case lexer::Member:         return ".";
    case lexer::Separator:      return ",";
    case lexer::Equals:         return "==";
    case lexer::NotEquals:      return "!=";
    case lexer::Larger:         return ">";
    case lexer::LargerEqual:    return ">=";
    case lexer::Smaller:        return "<";
    case lexer::SmallerEqual:   return "<=";
    case lexer::Assign:         return "=";
    case lexer::Plus:           return "+";
    case lexer::Minus:          return "-";
    case lexer::Mult:           return "*";
    case lexer::Div:            return "/";
    case lexer::Mod:            return "%";
    case lexer::BitwiseInvert:  return "~";
    case lexer::LogicInvert:    return "!";
    case lexer::TernaryIf:      return "?";
    case lexer::TernaryElse:    return ":";
    case lexer::LogicOr:        return "||";
    case lexer::LogicAnd:       return "&&";

    case lexer::kwBoolVec:      return "<boolN>";
    case lexer::kwUnsignedVec:  return "<unsignedN>";
    case lexer::kwIntVec:       return "<intN>";
    case lexer::kwFloatVec:     return "<floatN>";
    case lexer::kwBoolMat:      return "<boolNxM>";
    case lexer::kwUnsignedMat:  return "<unsignedNxM>";
    case lexer::kwIntMat:       return "<intNxM>";
    case lexer::kwFloatMat:     return "<floatNxM>";

    case lexer::MatFlag:
    case lexer::VecFlag:
    case lexer::TypeFlagMask:
      break;
    }
    return nullptr;
  }

  DECLARE_STATIC_FORMATTER(FormatVector, "{0}{1}");
  DECLARE_STATIC_FORMATTER(FormatMatrix, "{0}{1}x{2}");
  DECLARE_STATIC_FORMATTER(FormatDoubleQuote, "\"{0}\"");
  DECLARE_STATIC_FORMATTER(FormatSingleQuote, "'{0}'");
  
  uc::String Lexer::GetTokenStr (const Token& token)
  {
    if (token.typeOrID == lexer::Invalid) return "<Invalid>";

    if ((token.typeOrID == lexer::Identifier)
        || (token.typeOrID == lexer::Numeric))
      return FormatDoubleQuote.to<uc::String> (token.tokenString);
    uc::String tokenStr (GetTokenStr (static_cast<lexer::TokenType> (token.typeOrID & ~lexer::TypeFlagMask)));
    if ((token.typeOrID & lexer::VecFlag) != 0)
    {
      tokenStr = FormatVector.to<uc::String> (tokenStr, token.dimension1);
    }
    else if ((token.typeOrID & lexer::MatFlag) != 0)
    {
      tokenStr = FormatMatrix.to<uc::String> (tokenStr, token.dimension1, token.dimension2);
    }
    if (token.typeOrID > lexer::Unknown)
      tokenStr = FormatSingleQuote.to<uc::String> (tokenStr);
    return tokenStr;
  }
} // namespace s1
