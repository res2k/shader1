#include <cxxtest/TestSuite.h>

#include "base/UnicodeStream.h"

#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"

#include "LexerTestTraits.h"

class LexerNumericTestSuite : public CxxTest::TestSuite 
{
public:
  void testNumeric(void)
  {
    std::istringstream in ("0 0xabc 0x12AB -0 0.0 .0 -.0 0.0 .1 -.1 1e2 1e-2 .1E3 1.E-4 ");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // All of the above are valid numbers
    while (lexer)
    {
      TS_ASSERT_THROWS_NOTHING ((token = *lexer));
      // Token should be a "numeric"
      TS_ASSERT_EQUALS (token.type, s1::Lexer::Numeric);
      TS_ASSERT_DIFFERS (token.tokenString, UnicodeString (""));
      // Trying to forward never throws
      TS_ASSERT_THROWS_NOTHING (++lexer);
    }

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid1(void)
  {
    std::istringstream in ("0.0.0");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("0.0"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString (".0"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid2(void)
  {
    std::istringstream in ("0.-0");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("0."));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("-0"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid3(void)
  {
    std::istringstream in ("1e2.3");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("1e2"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString (".3"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid4(void)
  {
    std::istringstream in ("1e2e3");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("1e2"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("e3"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid5(void)
  {
    std::istringstream in ("0xa.e2");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("0xa"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "member"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Member);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("e2"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
};
