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
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "TestParser.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"
#include "../../semantics/t/print_semantics_BaseType.h"
#include "../../semantics/t/print_semantics_SamplerType.h"

BOOST_AUTO_TEST_SUITE(ParserTypes)

namespace
{
  class TestParser : public ::TestParser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::semantics::Handler& semanticsHandler,
                s1::diagnostics::Handler& diagnosticsHandler)
     : ::TestParser (inputLexer, semanticsHandler, diagnosticsHandler) {}
  
    using s1::Parser::Type;
    Type ParseType() { return ::TestParser::ParseType (s1::semantics::ScopePtr()); }
  };
} // anonymous namespace

BOOST_AUTO_TEST_CASE(TypeBool)
{
  std::string inStr ("bool");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (type->GetBaseType(), s1::semantics::BaseType::Bool);
}

BOOST_AUTO_TEST_CASE(TypeInt)
{
  std::string inStr ("int");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (type->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(TypeUInt)
{
  std::string inStr ("unsigned int");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (type->GetBaseType(), s1::semantics::BaseType::UInt);
}

BOOST_AUTO_TEST_CASE(TypeFloat)
{
  std::string inStr ("float");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (type->GetBaseType(), s1::semantics::BaseType::Float);
}

BOOST_AUTO_TEST_CASE(TypeSampler1D)
{
  std::string inStr ("sampler1D");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Sampler);
  BOOST_CHECK_EQUAL (type->GetSamplerType(), s1::semantics::SamplerType::_1D);
}

BOOST_AUTO_TEST_CASE(TypeSampler2D)
{
  std::string inStr ("sampler2D");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Sampler);
  BOOST_CHECK_EQUAL (type->GetSamplerType(), s1::semantics::SamplerType::_2D);
}

BOOST_AUTO_TEST_CASE(TypeSampler3D)
{
  std::string inStr ("sampler3D");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Sampler);
  BOOST_CHECK_EQUAL (type->GetSamplerType(), s1::semantics::SamplerType::_3D);
}

BOOST_AUTO_TEST_CASE(TypeSamplerCUBE)
{
  std::string inStr ("samplerCUBE");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Sampler);
  BOOST_CHECK_EQUAL (type->GetSamplerType(), s1::semantics::SamplerType::CUBE);
}

BOOST_AUTO_TEST_CASE(TypeVectorBool)
{
  std::string inStr ("bool3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Vector);
  BOOST_CHECK_EQUAL (type->GetVectorTypeComponents(), 3u);
  auto vecType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (vecType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (vecType->GetBaseType(), s1::semantics::BaseType::Bool);
}

BOOST_AUTO_TEST_CASE(TypeVectorInt)
{
  std::string inStr ("int3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Vector);
  BOOST_CHECK_EQUAL (type->GetVectorTypeComponents(), 3u);
  auto vecType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (vecType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (vecType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(TypeVectorUInt)
{
  std::string inStr ("unsigned int3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Vector);
  BOOST_CHECK_EQUAL (type->GetVectorTypeComponents(), 3u);
  auto vecType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (vecType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (vecType->GetBaseType(), s1::semantics::BaseType::UInt);
}

BOOST_AUTO_TEST_CASE(TypeVectorFloat)
{
  std::string inStr ("float3");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Vector);
  BOOST_CHECK_EQUAL (type->GetVectorTypeComponents(), 3u);
  auto vecType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (vecType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (vecType->GetBaseType(), s1::semantics::BaseType::Float);
}

BOOST_AUTO_TEST_CASE(TypeMatrixBool)
{
  std::string inStr ("bool3x2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Matrix);
  BOOST_CHECK_EQUAL (type->GetMatrixTypeCols(), 3u);
  BOOST_CHECK_EQUAL (type->GetMatrixTypeRows(), 2u);
  auto matType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (matType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (matType->GetBaseType(), s1::semantics::BaseType::Bool);
}

BOOST_AUTO_TEST_CASE(TypeMatrixInt)
{
  std::string inStr ("int3x2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Matrix);
  BOOST_CHECK_EQUAL (type->GetMatrixTypeCols(), 3u);
  BOOST_CHECK_EQUAL (type->GetMatrixTypeRows(), 2u);
  auto matType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (matType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (matType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(TypeMatrixUInt)
{
  std::string inStr ("unsigned int3x2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Matrix);
  BOOST_CHECK_EQUAL (type->GetMatrixTypeCols(), 3u);
  BOOST_CHECK_EQUAL (type->GetMatrixTypeRows(), 2u);
  auto matType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (matType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (matType->GetBaseType(), s1::semantics::BaseType::UInt);
}

BOOST_AUTO_TEST_CASE(TypeMatrixFloat)
{
  std::string inStr ("float3x2");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Matrix);
  BOOST_CHECK_EQUAL (type->GetMatrixTypeCols(), 3u);
  BOOST_CHECK_EQUAL (type->GetMatrixTypeRows(), 2u);
  auto matType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (matType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (matType->GetBaseType(), s1::semantics::BaseType::Float);
}

BOOST_AUTO_TEST_CASE(TypeArray)
{
  std::string inStr ("int[]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Array);
  auto arrType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (arrType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (arrType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_CASE(TypeArrayArray)
{
  std::string inStr ("int[][]");
  s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
  s1::uc::Stream ustream (in);
  TestDiagnosticsHandler errorHandler;
  s1::Lexer lexer (ustream, errorHandler);
  TestSemanticsHandler semanticsHandler;
  TestParser parser (lexer, semanticsHandler, errorHandler);

  s1::semantics::TypePtr type;
  BOOST_CHECK_NO_THROW ((type = parser.ParseType ()));
  BOOST_CHECK(errorHandler.parseErrors.empty());
  BOOST_CHECK_EQUAL (type->GetTypeClass(), s1::semantics::Type::Array);
  auto arrArrType = type->GetAVMBase();
  BOOST_CHECK_EQUAL (arrArrType->GetTypeClass(), s1::semantics::Type::Array);
  auto arrType = arrArrType->GetAVMBase();
  BOOST_CHECK_EQUAL (arrType->GetTypeClass(), s1::semantics::Type::Base);
  BOOST_CHECK_EQUAL (arrType->GetBaseType(), s1::semantics::BaseType::Int);
}

BOOST_AUTO_TEST_SUITE_END()
