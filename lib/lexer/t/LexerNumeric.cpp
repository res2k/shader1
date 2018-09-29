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

BOOST_AUTO_TEST_SUITE(LexerNumeric)

BOOST_AUTO_TEST_CASE(Numeric)
{
  std::string inStr ("0 0xabc 0x12AB 0.0 .0 0.0 .1 1e2 1e-2 .1E3 1.E-4");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // All of the above are valid numbers
  while (lexer)
  {
    BOOST_CHECK_NO_THROW ((token = *lexer));
    // Token should be a "numeric"
    BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Numeric);
    BOOST_CHECK_NE (token.tokenString, s1::uc::String (""));
    // Trying to forward never throws
    BOOST_CHECK_NO_THROW (++lexer);
  }

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(NumericInvalid1)
{
  std::string inStr ("0.0.0");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "numeric"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Numeric);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("0.0"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "numeric"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Numeric);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String (".0"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 3u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(NumericInvalid2)
{
  std::string inStr ("0.-0");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "numeric"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Numeric);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("0."));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be a minus
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Minus);
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 2u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);
  
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be a "numeric"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Numeric);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("0"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 3u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(NumericInvalid3)
{
  std::string inStr ("1e2.3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "numeric"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Numeric);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("1e2"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "numeric"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Numeric);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String (".3"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 3u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(NumericInvalid4)
{
  std::string inStr ("1e2e3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "numeric"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Numeric);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("1e2"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("e3"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 3u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(NumericInvalid5)
{
  std::string inStr ("0xa.e2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);
  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "numeric"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Numeric);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("0xa"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "member"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Member);
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 3u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("e2"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 4u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(NumericInvalid6)
{
  std::string inStr (".e2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "member"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Member);
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("e2"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 1u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(NumericInvalid7)
{
  std::string inStr (".x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "member"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Member);
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("x"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 1u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(NumericInvalid8)
{
  std::string inStr ("0x");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Should report token available
  BOOST_CHECK_EQUAL ((bool)lexer, true);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be invalid
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Invalid);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("0x"));
  BOOST_CHECK_EQUAL (token.location.line, 0u);
  BOOST_CHECK_EQUAL (token.location.column, 0u);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_SUITE_END()
