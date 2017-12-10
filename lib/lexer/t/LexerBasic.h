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

#include "base/common.h"

#include "base/uc/SimpleBufferStreamSource.h"
#include "base/uc/Stream.h"

#include "lexer/Diagnostics.h"
#include "lexer/Lexer.h"

#include "LexerTestTraits.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class LexerBasicTestSuite : public CxxTest::TestSuite 
{
public:
  void testEmptyInput (void)
  {
    std::string empty;
    s1::uc::SimpleBufferStreamSource in (empty.data (), empty.size ());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Empty stream should report "no tokens"
    TS_ASSERT_EQUALS ((bool)lexer, false);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // We should be at EOF from the start
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
  }
  
  void testWhitespace(void)
  {
    std::string inStr ("  \r   \t  \n   ");
    s1::uc::SimpleBufferStreamSource in (inStr.data (), inStr.size ());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Whitespace-only stream should report "no tokens"
    TS_ASSERT_EQUALS ((bool)lexer, false);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // We should be at EOF from the start
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
  }
  
  void testInvalidInput(void)
  {
    std::string inStr ("\xE2\x98");
    s1::uc::SimpleBufferStreamSource in (inStr.data (), inStr.size ());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Error handle should've been triggered
    TS_ASSERT_EQUALS (errorHandler.lexerError.code,
                      static_cast<unsigned int> (s1::lexer::Error::InvalidInputSequence));
    TS_ASSERT_EQUALS (errorHandler.lexerError.location.line, 0);
    TS_ASSERT_EQUALS (errorHandler.lexerError.location.column, 0);
    // Invalid input sequence should result in an Invalid token
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::Invalid);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
  }
  
  void testInvalidInput2(void)
  {
    std::string inStr ("a" "\xE2\x98" "b");
    s1::uc::SimpleBufferStreamSource in (inStr.data (), inStr.size ());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("a"));
    TS_ASSERT_EQUALS (token.location.line, 0);
    TS_ASSERT_EQUALS (token.location.column, 0);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.lexerError.code,
                      static_cast<unsigned int> (s1::lexer::Error::InvalidInputSequence));
    TS_ASSERT_EQUALS (errorHandler.lexerError.location.line, 0);
    TS_ASSERT_EQUALS (errorHandler.lexerError.location.column, 1);
    // Invalid input sequence should result in an Invalid token
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::Invalid);
    TS_ASSERT_EQUALS (token.location.line, 0);
    TS_ASSERT_EQUALS (token.location.column, 1);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("b"));
    TS_ASSERT_EQUALS (token.location.line, 0);
    TS_ASSERT_EQUALS (token.location.column, 2);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
  }
  
  void testTokens(void)
  {
    std::string inStr ("; ( ) [ ] { } . , == != > >= < <= = + - * / % ~ ! ? : || &&");
    struct Expected
    {
      s1::uc::String str;
      unsigned int col;
    };
    const Expected expected[] =
      { {";", 0}, {"(", 2}, {")", 4}, {"[", 6}, {"]", 8}, {"{", 10}, {"}", 12},
        {".", 14}, {",", 16}, {"==", 18}, {"!=", 21}, {">", 24}, {">=", 26},
        {"<", 29}, {"<=", 31}, {"=", 34}, {"+", 36}, {"-", 38}, {"*", 40},
        {"/", 42}, {"%", 44}, {"~", 46}, {"!", 48}, {"?", 50}, {":", 52},
        {"||", 54}, {"&&", 57} };
    s1::uc::SimpleBufferStreamSource in (inStr.data (), inStr.size ());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    s1::Lexer::TokenType expectedToken = s1::lexer::Semicolon;
    int expectedIndex = 0;
    // All of the above are valid tokens
    while (lexer)
    {
      TS_ASSERT_THROWS_NOTHING ((token = *lexer));
      // Token should be a certain token
      TS_ASSERT_EQUALS (token.typeOrID, expectedToken);
      TS_ASSERT_EQUALS (token.tokenString, expected[expectedIndex].str);
      TS_ASSERT_EQUALS (token.location.line, 0);
      TS_ASSERT_EQUALS (token.location.column, expected[expectedIndex].col);
      expectedToken = (s1::lexer::TokenType)(expectedToken+1);
      ++expectedIndex;
      // Trying to forward never throws
      TS_ASSERT_THROWS_NOTHING (++lexer);
    }

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
  }
  
  void testTokensInvalid(void)
  {
    std::string inStr ("| &");
    s1::uc::SimpleBufferStreamSource in (inStr.data (), inStr.size ());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Stray input character should result in an Unknown token
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::Unknown);
    TS_ASSERT_EQUALS (token.tokenString, "|");
    TS_ASSERT_EQUALS (token.location.line, 0);
    TS_ASSERT_EQUALS (token.location.column, 0);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Stray input character should result in an Unknown token
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::Unknown);
    TS_ASSERT_EQUALS (token.tokenString, "&");
    TS_ASSERT_EQUALS (token.location.line, 0);
    TS_ASSERT_EQUALS (token.location.column, 2);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
  }
};
