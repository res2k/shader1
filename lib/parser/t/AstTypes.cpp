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
#include "parser/ast/Type.h"
#include "parser/ast/TypeArray.h"

#include "AstMacros.h"
#include "TestAstBuilder.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

BOOST_AUTO_TEST_SUITE(AstTypes)

BOOST_AUTO_TEST_CASE(TypeBool)
{
  using namespace s1::parser;

  std::string inStr ("bool");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwBool);
}

BOOST_AUTO_TEST_CASE(TypeInt)
{
  using namespace s1::parser;

  std::string inStr ("int");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwInt);
}

BOOST_AUTO_TEST_CASE(TypeUInt)
{
  using namespace s1::parser;

  std::string inStr ("unsigned int");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_UNSIGNED_WELL_KNOWN(*type, kwInt);
}

BOOST_AUTO_TEST_CASE(TypeFloat)
{
  using namespace s1::parser;

  std::string inStr ("float");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwFloat);
}

BOOST_AUTO_TEST_CASE(TypeSampler1D)
{
  using namespace s1::parser;

  std::string inStr ("sampler1D");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwSampler1D);
}

BOOST_AUTO_TEST_CASE(TypeSampler2D)
{
  using namespace s1::parser;

  std::string inStr ("sampler2D");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwSampler2D);
}

BOOST_AUTO_TEST_CASE(TypeSampler3D)
{
  using namespace s1::parser;

  std::string inStr ("sampler3D");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwSampler3D);
}

BOOST_AUTO_TEST_CASE(TypeSamplerCUBE)
{
  using namespace s1::parser;

  std::string inStr ("samplerCUBE");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwSamplerCUBE);
}

BOOST_AUTO_TEST_CASE(TypeVectorBool)
{
  using namespace s1::parser;

  std::string inStr ("bool3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*type, kwBool, 3);
}

BOOST_AUTO_TEST_CASE(TypeVectorInt)
{
  using namespace s1::parser;

  std::string inStr ("int3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*type, kwInt, 3);
}

BOOST_AUTO_TEST_CASE(TypeVectorUInt)
{
  using namespace s1::parser;

  std::string inStr ("unsigned int3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_UNSIGNED_WELL_KNOWN_VEC(*type, kwInt, 3);
}

BOOST_AUTO_TEST_CASE(TypeVectorFloat)
{
  using namespace s1::parser;

  std::string inStr ("float3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*type, kwFloat, 3);
}

BOOST_AUTO_TEST_CASE(TypeMatrixBool)
{
  using namespace s1::parser;

  std::string inStr ("bool3x2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN_MAT(*type, kwBool, 3, 2);
}

BOOST_AUTO_TEST_CASE(TypeMatrixInt)
{
  using namespace s1::parser;

  std::string inStr ("int3x2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN_MAT(*type, kwInt, 3, 2);
}

BOOST_AUTO_TEST_CASE(TypeMatrixUInt)
{
  using namespace s1::parser;

  std::string inStr ("unsigned int3x2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_UNSIGNED_WELL_KNOWN_MAT(*type, kwInt, 3, 2);
}

BOOST_AUTO_TEST_CASE(TypeMatrixFloat)
{
  using namespace s1::parser;

  std::string inStr ("float3x2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  AST_TEST_TYPE_IS_WELL_KNOWN_MAT(*type, kwFloat, 3, 2);
}

BOOST_AUTO_TEST_CASE(TypeArray)
{
  using namespace s1::parser;

  std::string inStr ("int[]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto typeArray = dynamic_cast<const ast::TypeArray*> (type.get());
  AST_TEST_TYPE_IS_WELL_KNOWN(*(typeArray->containedType), kwInt);
}

BOOST_AUTO_TEST_CASE(TypeArrayArray)
{
  using namespace s1::parser;

  std::string inStr ("int[][]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestAstBuilder astBuilder (lexer, errorHandler);

  ast::TypePtr type;
  BOOST_CHECK_NO_THROW((type = astBuilder.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  const auto typeArray = dynamic_cast<const ast::TypeArray*> (type.get());
  const auto typeArrayArray = dynamic_cast<const ast::TypeArray*> (typeArray->containedType.get());
  AST_TEST_TYPE_IS_WELL_KNOWN(*(typeArrayArray->containedType), kwInt);
}

BOOST_AUTO_TEST_SUITE_END()
