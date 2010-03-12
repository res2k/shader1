#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"

#include "base/UnicodeStreamInvalidCharacterException.h"

#include <assert.h>
#include <unicode/uchar.h>

namespace s1
{
  Lexer::Lexer (UnicodeStream& inputChars, LexerErrorHandler& errorHandler)
   : inputChars (inputChars), errorHandler (errorHandler),
     currentToken (EndOfFile), currentChar (-1), putback (0)
  {
    if (inputChars)
    {
      NextChar();
      // Skip BOM
      if (currentChar == 0xfeff)
	NextChar();
    
      if ((bool)*this)
	++(*this);
    }
  }
   
  Lexer::operator bool() const throw()
  {
    return (currentToken.type != EndOfFile) || currentChar != -1 || inputChars;
  }

  Lexer& Lexer::operator++() throw()
  {
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
	  UChar32 oldChar = currentChar;
	  NextChar();
	  if ((currentChar >= '0') && (currentChar <= '9'))
	  {
	    // '.' is start of a number
	    PutCharBack (currentChar);
	    // '.' must be consumed again by numeric parsing code below
	    currentChar = '.';
	    break;
	  }
	  else
	  {
	    // '.' is member operator
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
	  NextChar();
	  if (((currentChar >= '0') && (currentChar <= '9')) || (currentChar == '.'))
	  {
	    // '-' is start of a number
	    PutCharBack (currentChar);
	    // '-' must be consumed again by numeric parsing code below
	    currentChar = '-';
	    break;
	  }
	  else
	  {
	    // '-' is minus operator
	    currentToken = Token (Minus, '-'); 
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
      case '%': currentToken = Token (Mod); 			NextChar(); return *this;
      case '~': currentToken = Token (BitwiseInvert, currentChar);NextChar(); return *this;
      case '?': currentToken = Token (TernaryIf, currentChar); 	NextChar(); return *this;
      case ':': currentToken = Token (TernaryElse, currentChar);NextChar(); return *this;
      case '&':
	{
	  NextChar();
	  if (currentChar == '&')
	  {
	    currentToken = Token (LogicAnd, "&&");
	    NextChar();
	  }
	  else
	  {
	    // Lone '&' not a valid token. Let stray character handling deal with it
	    PutCharBack (currentChar);
	    currentChar = '&';
	    break;
	  }
	}
	return *this;
      case '|':
	{
	  NextChar();
	  if (currentChar == '|')
	  {
	    currentToken = Token (LogicOr, "||");
	    NextChar();
	  }
	  else
	  {
	    // Lone '|' not a valid token. Let stray character handling deal with it
	    PutCharBack (currentChar);
	    currentChar = '|';
	    break;
	  }
	}
	return *this;
      }
      
      if ((currentChar == '_') || u_isIDStart (currentChar))
      {
	// Identifier
	UnicodeString tokenStr;
	tokenStr += currentChar;
	NextChar();
	while (u_isIDPart (currentChar))
	{
	  tokenStr += currentChar;
	  NextChar();
	}
	currentToken = Token (Identifier, tokenStr);
      }
      else if ((currentChar == '-') || (currentChar == '.')
	  || ((currentChar >= '0') && (currentChar <= '9')))
      {
	// Number
	UnicodeString tokenStr;
	tokenStr += currentChar;
	if (currentChar == '0')
	{
	  NextChar();
	  if ((currentChar == 'x') || (currentChar == 'X'))
	  {
	    // Hex number
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
	    return *this;
	  }
	  else
	  {
	    PutCharBack (currentChar);
	    currentChar = '0';
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
	    NextChar();
	    if (currentChar == '-')
	    {
	      tokenStr += currentChar;
	    }
	    else
	      PutCharBack (currentChar);
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
      else
      {
	if (currentChar != 0xfffd)
	  /* Replacement character (0xFFFD) indicates an invalid input sequence.
	     Error handler was already called for that.
	     Otherwise, it's an unrecognized character. */
	  errorHandler.StrayCharacter (currentChar);
	currentToken = Token (Invalid, currentChar);
	NextChar();
      }
      
      return *this;
    }
  }
  
  void Lexer::NextChar()
  {
    // Handle "putback" character
    if (putback != 0)
    {
      currentChar = putback;
      putback = 0;
      return;
    }
    
    if (inputChars)
    {
      try
      {
	currentChar = *inputChars;
      }
      catch (UnicodeStreamInvalidCharacterException& e)
      {
	// Signal error handler ...
	errorHandler.InputInvalidCharacter();
	// ... and set character to the 'replacer' one
	currentChar = 0xfffd;
      }
      ++inputChars;
    }
    else
    {
      // If at the end of input, return -1
      currentChar = -1;
    }
  }

  void Lexer::PutCharBack (UChar32 ch)
  {
    assert (putback == 0);
    putback = ch;
  }
  
} // namespace s1
