#include <cxxtest/TestSuite.h>

#include "base/UnicodeStream.h"

#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"

#include "LexerTestTraits.h"
#include "TestErrorHandler.h"

class LexerBasicTestSuite : public CxxTest::TestSuite 
{
public:
  void testEmptyInput (void)
  {
    std::string empty;
    std::istringstream in (empty);
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Empty stream should report "no tokens"
    TS_ASSERT_EQUALS ((bool)lexer, false);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // We should be at EOF from the start
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testWhitespace(void)
  {
    std::istringstream in ("  \r   \t  \n   ");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Whitespace-only stream should report "no tokens"
    TS_ASSERT_EQUALS ((bool)lexer, false);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // We should be at EOF from the start
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testInvalidInput(void)
  {
    std::istringstream in ("\xE2\x98");
    s1::UnicodeStream ustream (in, "utf-8");
    TestErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Error handle should've been triggered
    TS_ASSERT_EQUALS (errorHandler.invalidCharFound, true);
    // Invalid input sequence should result in an Invalid token
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Invalid);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testInvalidInput2(void)
  {
    std::istringstream in ("a" "\xE2\x98" "b");
    s1::UnicodeStream ustream (in, "utf-8");
    TestErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("a"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.invalidCharFound, true);
    // Invalid input sequence should result in an Invalid token
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Invalid);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("b"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testTokens(void)
  {
    std::istringstream in ("; ( ) [ ] { } . , == != > >= < <= = + - * / % ~ ! ? : || &&");
    s1::UnicodeStream ustream (in, "utf-8");
    TestErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    s1::Lexer::TokenType expectedToken = s1::Lexer::Semicolon;
    // All of the above are valid tokens
    while (lexer)
    {
      TS_ASSERT_THROWS_NOTHING ((token = *lexer));
      // Token should be a certain token
      TS_ASSERT_EQUALS (token.type, expectedToken);
      TS_ASSERT_DIFFERS (token.tokenString, UnicodeString (""));
      expectedToken = (s1::Lexer::TokenType)(expectedToken+1);
      // Trying to forward never throws
      TS_ASSERT_THROWS_NOTHING (++lexer);
    }

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testTokensInvalid(void)
  {
    std::istringstream in ("| &");
    s1::UnicodeStream ustream (in, "utf-8");
    TestErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.strayCharFound, true);
    TS_ASSERT_EQUALS (errorHandler.strayCharOffender, '|');
    errorHandler.Reset();
    // Invalid input sequence should result in an Invalid token
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Invalid);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.strayCharFound, true);
    TS_ASSERT_EQUALS (errorHandler.strayCharOffender, '&');
    // Invalid input sequence should result in an Invalid token
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Invalid);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
};
