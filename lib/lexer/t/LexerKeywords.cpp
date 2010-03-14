#include <cxxtest/TestSuite.h>

#include "base/UnicodeStream.h"

#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"

#include "LexerTestTraits.h"
#include "TestErrorHandler.h"

class LexerKeywordsTestSuite : public CxxTest::TestSuite 
{
public:
  void testKeywords(void)
  {
    std::istringstream in ("return true false bool unsigned int float sampler1D sampler2D sampler3D samplerCUBE "
			   "typedef void in out const if else while");
    s1::UnicodeStream ustream (in, "utf-8");
    TestErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    s1::Lexer::TokenType expectedToken = s1::Lexer::kwReturn;
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
  
  void testVectorMatrixKeywords(void)
  {
    std::istringstream in ("int2 int3x2 float5 float32");
    s1::UnicodeStream ustream (in, "utf-8");
    TestErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::kwInt);
    TS_ASSERT_EQUALS (token.typeType, s1::Lexer::Vector);
    TS_ASSERT_EQUALS (token.dimension1, 2);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::kwInt);
    TS_ASSERT_EQUALS (token.typeType, s1::Lexer::Matrix);
    TS_ASSERT_EQUALS (token.dimension1, 3);
    TS_ASSERT_EQUALS (token.dimension2, 2);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Identifier);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.type, s1::Lexer::Identifier);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.type, s1::Lexer::EndOfFile);
  }
  
};
