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

#include <assert.h>

#include "base/uc/SimpleBufferStreamSource.h"
#include "base/uc/Stream.h"

#include "lexer/Lexer.h"
#include "lexer/LexerErrorHandler.h"

#include "LexerTestTraits.h"

class LexerNumericTestSuite : public CxxTest::TestSuite 
{
public:
  void testNumeric(void)
  {
    std::string inStr ("0 0xabc 0x12AB 0.0 .0 0.0 .1 1e2 1e-2 .1E3 1.E-4");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // All of the above are valid numbers
    while (lexer)
    {
      TS_ASSERT_THROWS_NOTHING ((token = *lexer));
      // Token should be a "numeric"
      TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Numeric);
      TS_ASSERT_DIFFERS (token.tokenString, s1::uc::String (""));
      // Trying to forward never throws
      TS_ASSERT_THROWS_NOTHING (++lexer);
    }

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid1(void)
  {
    std::string inStr ("0.0.0");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("0.0"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String (".0"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid2(void)
  {
    std::string inStr ("0.-0");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("0."));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be a minus
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Minus);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be a "numeric"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("0"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid3(void)
  {
    std::string inStr ("1e2.3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("1e2"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String (".3"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid4(void)
  {
    std::string inStr ("1e2e3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("1e2"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("e3"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid5(void)
  {
    std::string inStr ("0xa.e2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "numeric"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Numeric);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("0xa"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "member"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Member);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("e2"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid6(void)
  {
    std::string inStr (".e2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "member"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Member);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("e2"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testNumericInvalid7(void)
  {
    std::string inStr (".x");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "member"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Member);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("x"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }

  void testNumericInvalid8(void)
  {
    std::string inStr ("0x");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be invalid
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Invalid);
    TS_ASSERT_EQUALS (token.tokenString, s1::uc::String ("0x"));
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
};
