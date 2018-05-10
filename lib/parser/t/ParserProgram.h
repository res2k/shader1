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

#include "parser/Diagnostics.h"
#include "parser/Exception.h"
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"
#include "TestParser.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class ParserProgramTestSuite : public CxxTest::TestSuite 
{
public:
  void testProgramSimple (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }
  
  void testProgramSimple2 (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main (out float4 pos, out float4 color) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }
  
  void testProgramLessSimple (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() { int foo = 1; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }
  
  void testProgramLessSimple2 (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() { int foo = 1, bar; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }
  
  void testProgramGlobalVar (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("int bar; void main() { int foo = bar; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }

  void testProgramGlobalConst (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("const int bar = 1; void main() { int foo = bar; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }
  
  void testProgramFunctionCall (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void Foo() {} void main() { Foo(); }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }
  
  void testProgramFunctionCall2 (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("int Foo() { return 1; } void main() { int x = Foo(); }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }
  
  void testProgramFunctionCallCast (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() { int x; x = float (1.2); }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }
  
  void testProgramExprNoResult (void)
  {
    using namespace s1::parser;
    
    std::string inStr ("void main() { int x, y; x + y; }");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT(errorHandler.parseErrors.empty());
  }

  void testProgramFunctionDeclInvalid1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
  }

  void testProgramFunctionDeclInvalid2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int 12) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
  }

  void testProgramFunctionDeclInvalid3 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int =) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  }

  void testProgramFunctionDeclInvalid4 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int = 12) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
  }

  void testProgramFunctionDeclInvalid5 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x =) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
  }

  void testProgramFunctionDeclInvalid6 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x 12) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
  }

  void testProgramFunctionDeclInvalid7 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(x) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_ASSERT(
      parser.ParseProgram (),
      const s1::parser::Exception& e,
      TS_ASSERT_EQUALS(e.GetCode(), s1::parser::Error::IdentifierUndeclared)
    );
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
  }

  void testProgramFunctionDeclInvalid8 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x float y) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
  }

  void testProgramFunctionDeclInvalid9 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x, ) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));
  }

  void testProgramFunctionDeclInvalid10 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x,, ) {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 2u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));
    TS_ASSERT_EQUALS(errorHandler.parseErrors[1].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));
  }

  void testProgramFunctionDeclInvalid11 (void)
  {
    using namespace s1::parser;

    std::string inStr ("void main(int x {}");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);

    TS_ASSERT_THROWS_NOTHING(parser.ParseProgram ());
    TS_ASSERT_EQUALS(errorHandler.parseErrors.size (), 1u);
    TS_ASSERT_EQUALS(errorHandler.parseErrors[0].code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
  }
};
