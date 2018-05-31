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

#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"
#include "TestParser.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class ParserTypesTestSuite : public CxxTest::TestSuite 
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
public:
  void testTypeBool (void)
  {
    std::string inStr ("bool");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::BaseType::Bool);
  }
  
  void testTypeInt (void)
  {
    std::string inStr ("int");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::BaseType::Int);
  }
  
  void testTypeUInt (void)
  {
    std::string inStr ("unsigned int");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::BaseType::UInt);
  }
  
  void testTypeFloat (void)
  {
    std::string inStr ("float");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::semantics::BaseType::Float);
  }
  
  void testTypeSampler1D (void)
  {
    std::string inStr ("sampler1D");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Sampler);
    TS_ASSERT_EQUALS (testType->sampler, s1::semantics::SamplerType::_1D);
  }
  
  void testTypeSampler2D (void)
  {
    std::string inStr ("sampler2D");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Sampler);
    TS_ASSERT_EQUALS (testType->sampler, s1::semantics::SamplerType::_2D);
  }
  
  void testTypeSampler3D (void)
  {
    std::string inStr ("sampler3D");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Sampler);
    TS_ASSERT_EQUALS (testType->sampler, s1::semantics::SamplerType::_3D);
  }
  
  void testTypeSamplerCUBE (void)
  {
    std::string inStr ("samplerCUBE");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Sampler);
    TS_ASSERT_EQUALS (testType->sampler, s1::semantics::SamplerType::CUBE);
  }
  
  void testTypeVectorBool (void)
  {
    std::string inStr ("bool3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Vector);
    TS_ASSERT_EQUALS (testType->vectorDim, 3);
    TestSemanticsHandler::TestType* vecType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (vecType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (vecType->base, s1::semantics::BaseType::Bool);
  }
  
  void testTypeVectorInt (void)
  {
    std::string inStr ("int3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Vector);
    TS_ASSERT_EQUALS (testType->vectorDim, 3);
    TestSemanticsHandler::TestType* vecType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (vecType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (vecType->base, s1::semantics::BaseType::Int);
  }
  
  void testTypeVectorUInt (void)
  {
    std::string inStr ("unsigned int3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Vector);
    TS_ASSERT_EQUALS (testType->vectorDim, 3);
    TestSemanticsHandler::TestType* vecType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (vecType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (vecType->base, s1::semantics::BaseType::UInt);
  }
  
  void testTypeVectorFloat (void)
  {
    std::string inStr ("float3");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Vector);
    TS_ASSERT_EQUALS (testType->vectorDim, 3);
    TestSemanticsHandler::TestType* vecType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (vecType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (vecType->base, s1::semantics::BaseType::Float);
  }
  
  void testTypeMatrixBool (void)
  {
    std::string inStr ("bool3x2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Matrix);
    TS_ASSERT_EQUALS (testType->matrixCols, 3);
    TS_ASSERT_EQUALS (testType->matrixRows, 2);
    TestSemanticsHandler::TestType* matType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (matType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (matType->base, s1::semantics::BaseType::Bool);
  }
  
  void testTypeMatrixInt (void)
  {
    std::string inStr ("int3x2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Matrix);
    TS_ASSERT_EQUALS (testType->matrixCols, 3);
    TS_ASSERT_EQUALS (testType->matrixRows, 2);
    TestSemanticsHandler::TestType* matType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (matType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (matType->base, s1::semantics::BaseType::Int);
  }
  
  void testTypeMatrixUInt (void)
  {
    std::string inStr ("unsigned int3x2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Matrix);
    TS_ASSERT_EQUALS (testType->matrixCols, 3);
    TS_ASSERT_EQUALS (testType->matrixRows, 2);
    TestSemanticsHandler::TestType* matType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (matType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (matType->base, s1::semantics::BaseType::UInt);
  }
  
  void testTypeMatrixFloat (void)
  {
    std::string inStr ("float3x2");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Matrix);
    TS_ASSERT_EQUALS (testType->matrixCols, 3);
    TS_ASSERT_EQUALS (testType->matrixRows, 2);
    TestSemanticsHandler::TestType* matType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (matType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (matType->base, s1::semantics::BaseType::Float);
  }
  
  void testTypeArray (void)
  {
    std::string inStr ("int[]");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Array);
    TestSemanticsHandler::TestType* arrType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (arrType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (arrType->base, s1::semantics::BaseType::Int);
  }
  
  void testTypeArrayArray (void)
  {
    std::string inStr ("int[][]");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler, errorHandler);
    
    s1::semantics::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TS_ASSERT(errorHandler.parseErrors.empty());
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Array);
    TestSemanticsHandler::TestType* arrArrType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (arrArrType->typeClass, TestSemanticsHandler::TestType::Array);
    TestSemanticsHandler::TestType* arrType = 
      static_cast<TestSemanticsHandler::TestType*> (arrArrType->avmBase.get());
    TS_ASSERT_EQUALS (arrType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (arrType->base, s1::semantics::BaseType::Int);
  }
};
