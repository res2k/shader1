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

#include "lexer/LexerErrorHandler.h"
#include "parser/Exception.h"
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class ParserProgramTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler,
                s1::diagnostics::Handler& diagnosticsHandler)
     : Parser (inputLexer, semanticsHandler, diagnosticsHandler) {}
     
    using s1::Parser::ParseProgram;
  };
public:
  void testProgramSimple (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }
  
  void testProgramSimple2 (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main (out float4 pos, out float4 color) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }
  
  void testProgramLessSimple (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() { int foo = 1; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }
  
  void testProgramLessSimple2 (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() { int foo = 1, bar; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }
  
  void testProgramGlobalVar (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("int bar; void main() { int foo = bar; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }

  void testProgramGlobalConst (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("const int bar = 1; void main() { int foo = bar; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }
  
  void testProgramFunctionCall (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void Foo() {} void main() { Foo(); }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }
  
  void testProgramFunctionCall2 (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("int Foo() { return 1; } void main() { int x = Foo(); }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }
  
  void testProgramFunctionCallCast (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() { int x; x = float (1.2); }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }
  
  void testProgramExprNoResult (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() { int x, y; x + y; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestDiagnosticsHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS (parserErrorHandler.parseError.code, 0);
  }
  
};
