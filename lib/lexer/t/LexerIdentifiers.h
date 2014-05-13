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

#include <cxxtest/TestSuite.h>

#include "base/uc/Stream.h"

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
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("foo"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("bar"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testIdentifiers2(void)
  {
    std::istringstream in ("f00 b4r  ");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("f00"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("b4r"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testIdentifiers3(void)
  {
    std::istringstream in ("\xCE\xB1"); // alpha
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString.length(), 1);
    TS_ASSERT_EQUALS (token.tokenString[0], 0x3b1);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testIdentifiers4(void)
  {
    std::istringstream in ("_foo bar_ b_az ");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("_foo"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("bar_"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("b_az"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testCanonicalEquivalence(void)
  {
    std::istringstream in ("o\xCC\x88 \xC3\xB6"); // <o, combining-diaeresis>, <ö>
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token1;
    s1::Lexer::Token token2;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token1 = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token1.typeOrID, s1::Lexer::Identifier);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token2 = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token2.typeOrID, s1::Lexer::Identifier);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    
    // Both tokens are canonically equivalent, so they should result in the same token string
    TS_ASSERT_EQUALS (token1.tokenString, token2.tokenString);
  }
  
};
