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
#include "parser/Diagnostics.h"
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "TestParser.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(ParserProgram)

BOOST_AUTO_TEST_CASE(ProgramSimple)
{
  using namespace s1::parser;

  std::string inStr ("void main() {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramSimple2)
{
  using namespace s1::parser;

  std::string inStr ("void main (out float4 pos, out float4 color) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramLessSimple)
{
  using namespace s1::parser;

  std::string inStr ("void main() { int foo = 1; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramLessSimple2)
{
  using namespace s1::parser;

  std::string inStr ("void main() { int foo = 1, bar; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramGlobalVar)
{
  using namespace s1::parser;

  std::string inStr ("int bar; void main() { int foo = bar; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramGlobalConst)
{
  using namespace s1::parser;

  std::string inStr ("const int bar = 1; void main() { int foo = bar; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionCall)
{
  using namespace s1::parser;

  std::string inStr ("void Foo() {} void main() { Foo(); }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionCall2)
{
  using namespace s1::parser;

  std::string inStr ("int Foo() { return 1; } void main() { int x = Foo(); }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionCallCast)
{
  using namespace s1::parser;

  std::string inStr ("void main() { int x; x = float (1.2); }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramExprNoResult)
{
  using namespace s1::parser;

  std::string inStr ("void main() { int x, y; x + y; }");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK(errorHandler.parseErrors.empty());
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid1)
{
  using namespace s1::parser;

  std::string inStr ("void main(int) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid2)
{
  using namespace s1::parser;

  std::string inStr ("void main(int 12) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid3)
{
  using namespace s1::parser;

  std::string inStr ("void main(int =) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid4)
{
  using namespace s1::parser;

  std::string inStr ("void main(int = 12) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid5)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x =) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedExpression));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid6)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x 12) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid7)
{
  using namespace s1::parser;

  std::string inStr ("void main(x) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(
    parser.ParseProgram ()
  );
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::IdentifierUndeclared));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid8)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x float y) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid9)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x, ) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid10)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x,, ) {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 2u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[1].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));
}

BOOST_AUTO_TEST_CASE(ProgramFunctionDeclInvalid11)
{
  using namespace s1::parser;

  std::string inStr ("void main(int x {}");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  BOOST_CHECK_NO_THROW(parser.ParseProgram ());
  BOOST_CHECK_EQUAL(errorHandler.parseErrors.size (), 1u);
  BOOST_CHECK_EQUAL(errorHandler.parseErrors[0].code,
                    static_cast<unsigned int> (s1::parser::Error::ExpectedSeparatorOrParenthesis));
}

BOOST_AUTO_TEST_SUITE_END()
