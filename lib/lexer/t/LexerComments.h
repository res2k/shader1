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

class LexerCommentsTestSuite : public CxxTest::TestSuite 
{
public:
  void testLineComment1(void)
  {
    std::istringstream in ("// foo");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
    TS_ASSERT_EQUALS ((bool)lexer, false);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // We should be at EOF from the start
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testLineComment2(void)
  {
    std::istringstream in ("// foo\n");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
    TS_ASSERT_EQUALS ((bool)lexer, false);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // We should be at EOF from the start
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testLineComment3(void)
  {
    std::istringstream in ("a\n// foo\nb");
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
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("a"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("b"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testBlockComment1(void)
  {
    std::istringstream in ("/* foo */");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
    TS_ASSERT_EQUALS ((bool)lexer, false);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // We should be at EOF from the start
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testBlockComment2(void)
  {
    std::istringstream in ("/* foo\n bar */");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
    TS_ASSERT_EQUALS ((bool)lexer, false);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // We should be at EOF from the start
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testBlockComment3(void)
  {
    std::istringstream in ("a\n/* foo */\nb");
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
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("a"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("b"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
};
