#include <cxxtest/TestSuite.h>

#include "base/UnicodeStream.h"

#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"

#include "LexerTestTraits.h"
#include "TestErrorHandler.h"

class LexerIdentifierTestSuite : public CxxTest::TestSuite 
{
public:
  void testIdentifiers1(void)
  {
    std::istringstream in ("foo bar  ");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("foo"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("bar"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testIdentifiers2(void)
  {
    std::istringstream in ("f00 b4r  ");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("f00"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("b4r"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testIdentifiers3(void)
  {
    std::istringstream in ("\xCE\xB1"); // alpha
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("\xCE\xB1"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testIdentifiers4(void)
  {
    std::istringstream in ("_foo bar_ b_az ");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("_foo"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("bar_"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, UnicodeString ("b_az"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
};
