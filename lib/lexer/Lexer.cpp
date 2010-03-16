// The hash<> specialization must come before Lexer.h is included
#if defined(__GNUC__) && !defined(__GXX_EXPERIMENTAL_CXX0X__)
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

#include <unicode/unistr.h>

namespace std
{
  namespace tr1
  {
    template<>
    class hash<UnicodeString>
    {
    public:
      size_t operator() (const UnicodeString& s) const
      { return s.hashCode(); }
    };
  }
}

#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"

#include "base/UnicodeStreamInvalidCharacterException.h"

#include <assert.h>
#include <unicode/uchar.h>

namespace s1
{
  Lexer::Lexer (UnicodeStream& inputChars, LexerErrorHandler& errorHandler)
   : inputChars (inputChars), errorHandler (errorHandler),
     currentToken (EndOfFile)
  {
    keywords[UnicodeString ("return")] 		= kwReturn;
    keywords[UnicodeString ("true")] 		= kwTrue;
    keywords[UnicodeString ("false")] 		= kwFalse;
    keywords[UnicodeString ("bool")] 		= kwBool;
    keywords[UnicodeString ("unsigned")] 	= kwUnsigned;
    keywords[UnicodeString ("int")] 		= kwInt;
    keywords[UnicodeString ("float")] 		= kwFloat;
    keywords[UnicodeString ("sampler1D")] 	= kwSampler1D;
    keywords[UnicodeString ("sampler2D")] 	= kwSampler2D;
    keywords[UnicodeString ("sampler3D")] 	= kwSampler3D;
    keywords[UnicodeString ("samplerCUBE")] 	= kwSamplerCUBE;
    keywords[UnicodeString ("typedef")] 	= kwTypedef;
    keywords[UnicodeString ("void")] 		= kwVoid;
    keywords[UnicodeString ("in")] 		= kwIn;
    keywords[UnicodeString ("out")] 		= kwOut;
    keywords[UnicodeString ("const")] 		= kwConst;
    keywords[UnicodeString ("if")] 		= kwIf;
    keywords[UnicodeString ("else")] 		= kwElse;
    keywords[UnicodeString ("while")] 		= kwWhile;
    
    // Fill lookahead characters
    for (int i = 0; i < LookAhead; i++)
      NextChar();
    // ... and one to set the actual current char
    NextChar();
    
    if ((bool)*this)
      ++(*this);
  }
   
  Lexer::operator bool() const throw()
  {
    return (currentToken.typeOrID != EndOfFile) || currentChar != -1 || inputChars;
  }

  Lexer& Lexer::operator++() throw()
  {
    /* This is a loop as, after a comment was handled, we still have to obtain
       the real desired 'next' token */
    while (true)
    {
      // Skip whitespace
      while (u_isUWhiteSpace (currentChar)) NextChar();
      
      // Check for end of input
      if (currentChar == -1)
      {
	currentToken = Token (EndOfFile);
	return *this;
      }
      
      // Check if it's a "simple" token (can only start with a single known character)
      switch (currentChar)
      {
      case ';': currentToken = Token (Semicolon, currentChar); 	NextChar(); return *this;
      case '(': currentToken = Token (ParenL, currentChar); 	NextChar(); return *this;
      case ')': currentToken = Token (ParenR, currentChar); 	NextChar(); return *this;
      case '[': currentToken = Token (BracketL, currentChar); 	NextChar(); return *this;
      case ']': currentToken = Token (BracketR, currentChar); 	NextChar(); return *this;
      case '{': currentToken = Token (BraceL, currentChar); 	NextChar(); return *this;
      case '}': currentToken = Token (BraceR, currentChar); 	NextChar(); return *this;
      case '.':
	{
	  UChar32 next = PeekChar();
	  if ((next >= '0') && (next <= '9'))
	  {
	    // '.' is start of a number
	    break;
	  }
	  else
	  {
	    // '.' is member operator
	    NextChar();
	    currentToken = Token (Member, '.'); 
	    return *this;
	  }
	}
      case ',': currentToken = Token (Separator, currentChar); 	NextChar(); return *this;
      case '=':
	{
	  NextChar();
	  if (currentChar == '=')
	  {
	    currentToken = Token (Equals, "==");
	    NextChar();
	  }
	  else
	    currentToken = Token (Assign, "=");
	}
	return *this;
      case '!':
	{
	  NextChar();
	  if (currentChar == '=')
	  {
	    currentToken = Token (NotEquals, "!=");
	    NextChar();
	  }
	  else
	    currentToken = Token (LogicInvert, "!");
	}
	return *this;
      case '>':
	{
	  NextChar();
	  if (currentChar == '=')
	  {
	    currentToken = Token (LargerEqual, ">=");
	    NextChar();
	  }
	  else
	    currentToken = Token (Larger, ">");
	}
	return *this;
      case '<':
	{
	  NextChar();
	  if (currentChar == '=')
	  {
	    currentToken = Token (SmallerEqual, "<=");
	    NextChar();
	  }
	  else
	    currentToken = Token (Smaller, "<");
	}
	return *this;
      case '+': currentToken = Token (Plus, currentChar); 	NextChar(); return *this;
      case '-':
	{
	  UChar32 next = PeekChar();
	  UChar32 next2 = PeekChar(1);
	  if (((next >= '0') && (next <= '9'))
	      || ((next == '.')
		&& ((next2 >= '0') && (next2 <= '9'))))
	  {
	    // '-' is start of a number
	    break;
	  }
	  else
	  {
	    // '-' is minus operator
	    currentToken = Token (Minus, '-'); 
	    NextChar();
	    return *this;
	  }
	}
      case '*': currentToken = Token (Mult, currentChar); 	NextChar(); return *this;
      case '/':
	{
	  NextChar();
	  if (currentChar == '/')
	  {
	    // Line comment
	    do
	    {
	      NextChar();
	    }
	    while ((currentChar != -1) && (currentChar != '\n'));
	    // Let '\n' be handled by regular code
	    continue;
	  }
	  else if (currentChar == '*')
	  {
	    // Block comment
	    do
	    {
	      NextChar();
	      if (currentChar == '*')
	      {
		NextChar();
		// Check for ending '*/'
		if (currentChar == '/')
		{
		  NextChar();
		  break;
		}
	      }
	    }
	    while (currentChar != -1);
	    continue;
	  }
	  else
	    // Division operator
	    currentToken = Token (Div, "/");
	}
	return *this;
      case '%': currentToken = Token (Mod, currentChar); 	NextChar(); return *this;
      case '~': currentToken = Token (BitwiseInvert, currentChar);NextChar(); return *this;
      case '?': currentToken = Token (TernaryIf, currentChar); 	NextChar(); return *this;
      case ':': currentToken = Token (TernaryElse, currentChar);NextChar(); return *this;
      case '&':
	{
	  UChar32 next = PeekChar();
	  if (next == '&')
	  {
	    currentToken = Token (LogicAnd, "&&");
	    // Skip first & second '&'
	    NextChar();
	    NextChar();
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
	  UChar32 next = PeekChar();
	  if (next == '|')
	  {
	    currentToken = Token (LogicOr, "||");
	    // Skip first & second '|'
	    NextChar();
	    NextChar();
	  }
	  else
	  {
	    // Lone '|' not a valid token. Let stray character handling deal with it
	    break;
	  }
	}
	return *this;
      }
      
      if ((currentChar == '_') || u_isIDStart (currentChar))
      {
	// Identifier
	ParseIdentifier ();
      }
      else if ((currentChar == '-') || (currentChar == '.')
	  || ((currentChar >= '0') && (currentChar <= '9')))
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
	  currentToken = Token (Unknown, currentChar);
	}
	else
	  /* Otherwise, it's an unrecognized character. */
	  currentToken = Token (Invalid, currentChar);
	NextChar();
      }
      
      return *this;
    }
  }

  void Lexer::ParseIdentifier ()
  {
    UnicodeString tokenStr;
    tokenStr += currentChar;
    NextChar();
    while (u_isIDPart (currentChar))
    {
      tokenStr += currentChar;
      NextChar();
    }
    
    currentToken = Token (Identifier, tokenStr);
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
      UnicodeString dimensions;
      if (tokenStr.startsWith ("int"))
      {
	typeCandidate = kwInt;
	dimensions = UnicodeString (tokenStr, 3);
      }
      else if (tokenStr.startsWith ("float"))
      {
	typeCandidate = kwFloat;
	dimensions = UnicodeString (tokenStr, 5);
      }
      else if (tokenStr.startsWith ("bool"))
      {
	typeCandidate = kwBool;
	dimensions = UnicodeString (tokenStr, 4);
      }
      if (typeCandidate != Invalid)
      {
	if ((dimensions.length() == 1)
	    && (dimensions[0] >= '1')
	    && (dimensions[0] <= '4'))
	{
	  // It's a vector!
	  currentToken.typeOrID = typeCandidate;
	  currentToken.typeClass = Vector;
	  currentToken.dimension1 = dimensions[0] - '1' + 1;
	}
	else if ((dimensions.length() == 3)
	    && (dimensions[0] >= '1')
	    && (dimensions[0] <= '4')
	    && (dimensions[1] == 'x')
	    && (dimensions[2] >= '1')
	    && (dimensions[2] <= '4'))
	{
	  // It's a matrix!
	  currentToken.typeOrID = typeCandidate;
	  currentToken.typeClass = Matrix;
	  currentToken.dimension1 = dimensions[0] - '1' + 1;
	  currentToken.dimension2 = dimensions[2] - '1' + 1;
	}
      }
    }
  }
  
  void Lexer::ParseNumeric()
  {
    UnicodeString tokenStr;
    tokenStr += currentChar;
    if (currentChar == '0')
    {
      UChar32 next = PeekChar();
      if ((next == 'x') || (next == 'X'))
      {
	// Hex number
	NextChar();
	tokenStr += currentChar;
	NextChar();
	while (((currentChar >= '0') && (currentChar <= '9'))
	  || ((currentChar >= 'a') && (currentChar <= 'f'))
	  || ((currentChar >= 'A') && (currentChar <= 'F')))
	{
	  tokenStr += currentChar;
	  NextChar();
	}
	currentToken = Token (Numeric, tokenStr);
	return;
      }
    }

    // Float number
    bool hasDecimal = currentChar == '.';
    bool hasExp = false;
    while (true)
    {
      NextChar();
      if ((currentChar == '.') && !hasDecimal)
      {
	hasDecimal = true;
	tokenStr += currentChar;
      }
      else if (((currentChar == 'E') || (currentChar == 'e')) && !hasExp)
      {
	hasExp = true;
	tokenStr += currentChar;
	// Force to 'true' since decimal exponents are not allowed
	hasDecimal = true;
	// Accept '-' again, once
	UChar32 next = PeekChar();
	if (next == '-')
	{
	  NextChar();
	  tokenStr += currentChar;
	}
      }
      else if ((currentChar >= '0') && (currentChar <= '9'))
      {
	tokenStr += currentChar;
      }
      else
      {
	// End of number
	break;
      }
    }
    
    currentToken = Token (Numeric, tokenStr);
  }

  void Lexer::NextChar()
  {
    currentChar = nextChar[0];
    for (int i = 1; i < LookAhead; i++)
      nextChar[i-1] = nextChar[i];
    if (inputChars)
    {
      try
      {
	nextChar[LookAhead-1] = *inputChars;
      }
      catch (UnicodeStreamInvalidCharacterException& e)
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

} // namespace s1
