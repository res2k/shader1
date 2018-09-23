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

BOOST_AUTO_TEST_SUITE(LexerKeywords)

BOOST_AUTO_TEST_CASE(Keywords)
{
  std::string inStr ("return true false sampler1D sampler2D sampler3D samplerCUBE "
                      "typedef void in out const uniform attribute if else while for "
                      "bool unsigned int float");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  s1::Lexer::TokenType expectedToken = s1::lexer::kwReturn;
  // All of the above are valid tokens
  while (lexer)
  {
    BOOST_CHECK_NO_THROW ((token = *lexer));
    // Token should be a certain token
    BOOST_CHECK_EQUAL (token.typeOrID, expectedToken);
    BOOST_CHECK_NE (token.tokenString, s1::uc::String (""));
    expectedToken = (s1::Lexer::TokenType)(expectedToken+1);
    // Trying to forward never throws
    BOOST_CHECK_NO_THROW (++lexer);
  }

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_CASE(VectorMatrixKeywords)
{
  std::string inStr ("int2 int3x2 float5 float32 bool3");
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
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::kwIntVec);
  BOOST_CHECK_EQUAL (token.dimension1, 2);
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::kwIntMat);
  BOOST_CHECK_EQUAL (token.dimension1, 3);
  BOOST_CHECK_EQUAL (token.dimension2, 2);
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 5);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 12);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 19);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::kwBoolVec);
  BOOST_CHECK_EQUAL (token.dimension1, 3);
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 27);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);
  
  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
}

BOOST_AUTO_TEST_SUITE_END()
