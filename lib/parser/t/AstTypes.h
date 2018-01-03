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

#include <cxxtest/TestSuite.h>

#include "base/common.h"

#include "parser/ast/Type.h"

#include "AstMacros.h"
#include "TestAstBuilder.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class AstTypesTestSuite : public CxxTest::TestSuite
{
public:
  void testTypeBool (void)
  {
    using namespace s1::parser;

    std::string inStr ("bool");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwBool);
  }

  void testTypeInt (void)
  {
    using namespace s1::parser;

    std::string inStr ("int");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwInt);
  }

  void testTypeUInt (void)
  {
    using namespace s1::parser;

    std::string inStr ("unsigned int");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_UNSIGNED_WELL_KNOWN(*type, kwInt);
  }

  void testTypeFloat (void)
  {
    using namespace s1::parser;

    std::string inStr ("float");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwFloat);
  }

  void testTypeSampler1D (void)
  {
    using namespace s1::parser;

    std::string inStr ("sampler1D");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwSampler1D);
  }

  void testTypeSampler2D (void)
  {
    using namespace s1::parser;

    std::string inStr ("sampler2D");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwSampler2D);
  }

  void testTypeSampler3D (void)
  {
    using namespace s1::parser;

    std::string inStr ("sampler3D");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwSampler3D);
  }

  void testTypeSamplerCUBE (void)
  {
    using namespace s1::parser;

    std::string inStr ("samplerCUBE");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN(*type, kwSamplerCUBE);
  }

  void testTypeVectorBool (void)
  {
    using namespace s1::parser;

    std::string inStr ("bool3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*type, kwBool, 3);
  }

  void testTypeVectorInt (void)
  {
    using namespace s1::parser;

    std::string inStr ("int3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*type, kwInt, 3);
  }

  void testTypeVectorUInt (void)
  {
    using namespace s1::parser;

    std::string inStr ("unsigned int3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_UNSIGNED_WELL_KNOWN_VEC(*type, kwInt, 3);
  }
  
  void testTypeVectorFloat (void)
  {
    using namespace s1::parser;

    std::string inStr ("float3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN_VEC(*type, kwFloat, 3);
  }

  void testTypeMatrixBool (void)
  {
    using namespace s1::parser;

    std::string inStr ("bool3x2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN_MAT(*type, kwBool, 3, 2);
  }

  void testTypeMatrixInt (void)
  {
    using namespace s1::parser;

    std::string inStr ("int3x2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN_MAT(*type, kwInt, 3, 2);
  }

  void testTypeMatrixUInt (void)
  {
    using namespace s1::parser;

    std::string inStr ("unsigned int3x2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_UNSIGNED_WELL_KNOWN_MAT(*type, kwInt, 3, 2);
  }

  void testTypeMatrixFloat (void)
  {
    using namespace s1::parser;

    std::string inStr ("float3x2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    AST_TEST_TYPE_IS_WELL_KNOWN_MAT(*type, kwFloat, 3, 2);
  }

  void testTypeArray (void)
  {
    using namespace s1::parser;

    std::string inStr ("int[]");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& typeArray = boost::get<ast::Type::ArrayType> (type->value);
    const auto& typeArrayWellKnown = boost::get<ast::Type::WellKnownType> (typeArray.containedType->value);
    TS_ASSERT_EQUALS(typeArrayWellKnown.size(), 1u);
    TS_ASSERT_EQUALS(typeArrayWellKnown[0].typeOrID, s1::lexer::kwInt);
  }

  void testTypeArrayArray (void)
  {
    using namespace s1::parser;

    std::string inStr ("int[][]");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    ast::TypePtr type;
    TS_ASSERT_THROWS_NOTHING((type = astBuilder.ParseType ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);
    const auto& typeArray = boost::get<ast::Type::ArrayType> (type->value);
    const auto& typeArrayArray = boost::get<ast::Type::ArrayType> (typeArray.containedType->value);
    const auto& typeArrayArrayWellKnown = boost::get<ast::Type::WellKnownType> (typeArrayArray.containedType->value);
    TS_ASSERT_EQUALS(typeArrayArrayWellKnown.size(), 1u);
    TS_ASSERT_EQUALS(typeArrayArrayWellKnown[0].typeOrID, s1::lexer::kwInt);
  }
};
