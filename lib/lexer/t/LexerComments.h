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

#include "base/uc/SimpleBufferStreamSource.h"
#include "base/uc/Stream.h"

#include "lexer/Lexer.h"

#include "LexerTestTraits.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class LexerCommentsTestSuite : public CxxTest::TestSuite 
{
public:
  void testLineComment1(void)
  {
    std::string inStr ("// foo");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    TS_ASSERT_EQUALS (errorHandler.lexerError.code, 0);
  }
  
  void testLineComment2(void)
  {
    std::string inStr ("// foo\n");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    TS_ASSERT_EQUALS (errorHandler.lexerError.code, 0);
  }
  
  void testLineComment3(void)
  {
    std::string inStr ("a\n// foo\nb");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
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
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("b"));
    TS_ASSERT_EQUALS (token.location.line, 2);
    TS_ASSERT_EQUALS (token.location.column, 0);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
    TS_ASSERT_EQUALS (errorHandler.lexerError.code, 0);
  }
  
  void testBlockComment1(void)
  {
    std::string inStr ("/* foo */");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    TS_ASSERT_EQUALS (errorHandler.lexerError.code, 0);
  }
  
  void testBlockComment2(void)
  {
    std::string inStr ("/* foo\n bar */");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    TS_ASSERT_EQUALS (errorHandler.lexerError.code, 0);
  }
  
  void testBlockComment3(void)
  {
    std::string inStr ("a\n/* foo */\nb");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
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
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("b"));
    TS_ASSERT_EQUALS (token.location.line, 2);
    TS_ASSERT_EQUALS (token.location.column, 0);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
    TS_ASSERT_EQUALS (errorHandler.lexerError.code, 0);
  }

  void testBlockComment4(void)
  {
    std::string inStr ("a\n/* foo\nbar */\nb");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
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
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("b"));
    TS_ASSERT_EQUALS (token.location.line, 3);
    TS_ASSERT_EQUALS (token.location.column, 0);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::lexer::EndOfFile);
    TS_ASSERT_EQUALS (errorHandler.lexerError.code, 0);
  }

  void testLineCommentInvalidInput(void)
  {
    std::string inStr ("// foo" "\xE2\x98" "\n");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.lexerError.code,
                      static_cast<unsigned int> (s1::lexer::Warning::InvalidInputSequence));
  }

  void testLineCommentInvalidInput2(void)
  {
    std::string inStr ("// foo" "\xE2\x98");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.lexerError.code,
                      static_cast<unsigned int> (s1::lexer::Warning::InvalidInputSequence));
  }

  void testBlockCommentInvalidInput(void)
  {
    std::string inStr ("/* foo" "\xE2\x98" " */\n");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.lexerError.code,
                      static_cast<unsigned int> (s1::lexer::Warning::InvalidInputSequence));
  }

  void testBlockCommentInvalidInput2(void)
  {
    std::string inStr ("/* foo" "\xE2\x98" " */");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.lexerError.code,
                      static_cast<unsigned int> (s1::lexer::Warning::InvalidInputSequence));
  }

  void testBlockCommentUnterminated(void)
  {
    std::string inStr ("/* foo ");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.lexerError.code,
                      static_cast<unsigned int> (s1::lexer::Warning::UnterminatedBlockComment));
  }

  void testBlockCommentUnterminated2(void)
  {
    std::string inStr ("/* foo\n");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Comment-only stream should report "no tokens"
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
    // Error handler should've been triggered
    TS_ASSERT_EQUALS (errorHandler.lexerError.code,
                      static_cast<unsigned int> (s1::lexer::Warning::UnterminatedBlockComment));
  }
};
