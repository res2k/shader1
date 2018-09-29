/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <boost/test/unit_test.hpp>

#include "base/common.h"

#include "base/uc/SimpleBufferStreamSource.h"
#include "base/uc/Stream.h"

#include "lexer/Diagnostics.h"
#include "lexer/Lexer.h"

#include "print_lexer_TokenType.h"

#include "../../base/t/print_uc_String.h"
#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(LexerBasic)

BOOST_AUTO_TEST_CASE(EmptyInput)
{
  std::string empty;
  s1::uc::SimpleBufferStreamSource in (empty.data (), empty.size ());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Empty stream should report "no tokens"
  BOOST_CHECK_EQUAL ((bool)lexer, false);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // We should be at EOF from the start
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);
  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(Whitespace)
{
  std::string inStr ("  \r   \t  \n   ");
  s1::uc::SimpleBufferStreamSource in (inStr.data (), inStr.size ());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Whitespace-only stream should report "no tokens"
  BOOST_CHECK_EQUAL ((bool)lexer, false);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // We should be at EOF from the start
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);
  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(InvalidInput)
{
  std::string inStr ("\xE2\x98");
  s1::uc::SimpleBufferStreamSource in (inStr.data (), inStr.size ());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Error handle should've been triggered
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code,
                    static_cast<unsigned int> (s1::lexer::Error::InvalidInputSequence));
  BOOST_CHECK_EQUAL (errorHandler.lexerError.location.line, 0u);
  BOOST_CHECK_EQUAL (errorHandler.lexerError.location.column, 0u);
  // Invalid input sequence results in a replacement char, which is treated like an identifier
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);
  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(InvalidInput2)
{
  std::string inStr ("a" "\xE2\x98" "b");
  s1::uc::SimpleBufferStreamSource in (inStr.data (), inStr.size ());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String::fromUTF8 ("a" "\xEF\xBF\xBD" "b"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(Tokens)
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
    BOOST_CHECK_NO_THROW ((token = *lexer));
    // Token should be a certain token
    BOOST_CHECK_EQUAL (token.typeOrID, expectedToken);
    BOOST_CHECK_EQUAL (token.tokenString, expected[expectedIndex].str);
    BOOST_CHECK_EQUAL (token.location.line, 0u);
    BOOST_CHECK_EQUAL (token.location.column, expected[expectedIndex].col);
    expectedToken = (s1::lexer::TokenType)(expectedToken+1);
    ++expectedIndex;
    // Trying to forward never throws
    BOOST_CHECK_NO_THROW (++lexer);
  }

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(TokensInvalid)
{
  std::string inStr ("| &");
  s1::uc::SimpleBufferStreamSource in (inStr.data (), inStr.size ());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Stray input character should result in an Invalid token
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Invalid);
  BOOST_CHECK_EQUAL (token.tokenString, "|");
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Stray input character should result in an Invalid token
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Invalid);
  BOOST_CHECK_EQUAL (token.tokenString, "&");
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 2u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_SUITE_END()