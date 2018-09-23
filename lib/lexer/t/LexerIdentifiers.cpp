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

#include "base/uc/SimpleBufferStreamSource.h"
#include "base/uc/Stream.h"

#include "lexer/Lexer.h"

#include "print_lexer_TokenType.h"

#include "../../base/t/print_uc_String.h"
#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(LexerIdentifier)

BOOST_AUTO_TEST_CASE(Identifiers1)
{
  std::string inStr ("foo bar  ");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
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
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("foo"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("bar"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 4);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(Identifiers2)
{
  std::string inStr ("f00 b4r  ");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
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
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("f00"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("b4r"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 4);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(Identifiers3)
{
  std::string inStr ("\xCE\xB1"); // alpha
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
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
  BOOST_CHECK_EQUAL (token.tokenString.length(), 1);
  BOOST_CHECK_EQUAL (token.tokenString.data()[0], 0x3b1);
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(Identifiers4)
{
  std::string inStr ("_foo bar_ b_az ");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
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
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("_foo"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("bar_"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 5);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("b_az"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 10);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(CanonicalEquivalence)
{
  std::string inStr ("o\xCC\x88 \xC3\xB6"); // <o, combining-diaeresis>, <ö>
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token1;
  s1::Lexer::Token token2;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token1 = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token1.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token1.location.line, 0);
  BOOST_CHECK_EQUAL (token1.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token2 = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token2.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token2.location.line, 0);
  BOOST_CHECK_EQUAL (token2.location.column, 3);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);
  
  // Both tokens are canonically equivalent, so they should result in the same token string
  BOOST_CHECK_EQUAL (token1.tokenString, token2.tokenString);
}

BOOST_AUTO_TEST_SUITE_END()
