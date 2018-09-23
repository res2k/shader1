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

#include "lexer/Diagnostics.h"
#include "lexer/Lexer.h"

#include "print_lexer_TokenType.h"

#include "../../base/t/print_uc_String.h"
#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(LexerComments)

BOOST_AUTO_TEST_CASE(LineComment1)
{
  std::string inStr ("// foo");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code, 0);
}

BOOST_AUTO_TEST_CASE(LineComment2)
{
  std::string inStr ("// foo\n");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code, 0);
}

BOOST_AUTO_TEST_CASE(LineComment3)
{
  std::string inStr ("a\n// foo\nb");
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
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("a"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("b"));
  BOOST_CHECK_EQUAL (token.location.line, 2);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code, 0);
}

BOOST_AUTO_TEST_CASE(BlockComment1)
{
  std::string inStr ("/* foo */");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code, 0);
}

BOOST_AUTO_TEST_CASE(BlockComment2)
{
  std::string inStr ("/* foo\n bar */");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code, 0);
}

BOOST_AUTO_TEST_CASE(BlockComment3)
{
  std::string inStr ("a\n/* foo */\nb");
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
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("a"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("b"));
  BOOST_CHECK_EQUAL (token.location.line, 2);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code, 0);
}

BOOST_AUTO_TEST_CASE(BlockComment4)
{
  std::string inStr ("a\n/* foo\nbar */\nb");
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
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("a"));
  BOOST_CHECK_EQUAL (token.location.line, 0);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Any attempt to get current token should never throw anything
  BOOST_CHECK_NO_THROW ((token = *lexer));
  // Token should be an "identifier"
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::Identifier);
  BOOST_CHECK_EQUAL (token.tokenString, s1::uc::String ("b"));
  BOOST_CHECK_EQUAL (token.location.line, 3);
  BOOST_CHECK_EQUAL (token.location.column, 0);
  // Trying to forward never throws
  BOOST_CHECK_NO_THROW (++lexer);

  // Still at end
  BOOST_CHECK_NO_THROW ((token = *lexer));
  BOOST_CHECK_EQUAL (token.typeOrID, s1::lexer::EndOfFile);
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code, 0);
}

BOOST_AUTO_TEST_CASE(LineCommentInvalidInput)
{
  std::string inStr ("// foo" "\xE2\x98" "\n");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  // Error handler should've been triggered
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code,
                     static_cast<unsigned int> (s1::lexer::Warning::InvalidInputSequence));
}

BOOST_AUTO_TEST_CASE(LineCommentInvalidInput2)
{
  std::string inStr ("// foo" "\xE2\x98");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  // Error handler should've been triggered
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code,
                     static_cast<unsigned int> (s1::lexer::Warning::InvalidInputSequence));
}

BOOST_AUTO_TEST_CASE(BlockCommentInvalidInput)
{
  std::string inStr ("/* foo" "\xE2\x98" " */\n");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  // Error handler should've been triggered
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code,
                     static_cast<unsigned int> (s1::lexer::Warning::InvalidInputSequence));
}

BOOST_AUTO_TEST_CASE(BlockCommentInvalidInput2)
{
  std::string inStr ("/* foo" "\xE2\x98" " */");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  // Error handler should've been triggered
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code,
                     static_cast<unsigned int> (s1::lexer::Warning::InvalidInputSequence));
}

BOOST_AUTO_TEST_CASE(BlockCommentUnterminated)
{
  std::string inStr ("/* foo ");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  // Error handler should've been triggered
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code,
                     static_cast<unsigned int> (s1::lexer::Warning::UnterminatedBlockComment));
}

BOOST_AUTO_TEST_CASE(BlockCommentUnterminated2)
{
  std::string inStr ("/* foo\n");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  s1::Lexer::Token token;

  // Comment-only stream should report "no tokens"
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
  // Error handler should've been triggered
  BOOST_CHECK_EQUAL (errorHandler.lexerError.code,
                     static_cast<unsigned int> (s1::lexer::Warning::UnterminatedBlockComment));
}

BOOST_AUTO_TEST_SUITE_END()
