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

class LexerKeywordsTestSuite : public CxxTest::TestSuite 
{
public:
  void testKeywords(void)
  {
    std::string inStr ("return true false bool unsigned int float sampler1D sampler2D sampler3D samplerCUBE "
			   "typedef void in out const uniform attribute if else while");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    s1::Lexer::TokenType expectedToken = s1::Lexer::kwReturn;
    // All of the above are valid tokens
    while (lexer)
    {
      TS_ASSERT_THROWS_NOTHING ((token = *lexer));
      // Token should be a certain token
      TS_ASSERT_EQUALS (token.typeOrID, expectedToken);
      TS_ASSERT_DIFFERS (token.tokenString, s1::uc::String (""));
      expectedToken = (s1::Lexer::TokenType)(expectedToken+1);
      // Trying to forward never throws
      TS_ASSERT_THROWS_NOTHING (++lexer);
    }

    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
  void testVectorMatrixKeywords(void)
  {
    std::string inStr ("int2 int3x2 float5 float32 bool3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    s1::Lexer::Token token;

    // Should report token available
    TS_ASSERT_EQUALS ((bool)lexer, true);
    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::kwInt);
    TS_ASSERT_EQUALS (token.typeClass, s1::Lexer::Vector);
    TS_ASSERT_EQUALS (token.dimension1, 2);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::kwInt);
    TS_ASSERT_EQUALS (token.typeClass, s1::Lexer::Matrix);
    TS_ASSERT_EQUALS (token.dimension1, 3);
    TS_ASSERT_EQUALS (token.dimension2, 2);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::Identifier);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);

    // Any attempt to get current token should never throw anything
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    // Token should be an "identifier"
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::kwBool);
    TS_ASSERT_EQUALS (token.typeClass, s1::Lexer::Vector);
    TS_ASSERT_EQUALS (token.dimension1, 3);
    // Trying to forward never throws
    TS_ASSERT_THROWS_NOTHING (++lexer);
    
    // Still at end
    TS_ASSERT_THROWS_NOTHING ((token = *lexer));
    TS_ASSERT_EQUALS (token.typeOrID, s1::Lexer::EndOfFile);
  }
  
};
