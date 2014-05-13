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
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"

class ParserTypesTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler,
		s1::parser::ErrorHandler& errorHandler)
     : Parser (inputLexer, semanticsHandler, errorHandler) {}
    
    using s1::Parser::Type;
    Type ParseType() { return s1::Parser::ParseType (TestSemanticsHandler::ScopePtr()); }
  };
public:
  void testTypeBool (void)
  {
    std::istringstream in ("bool");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Bool);
  }
  
  void testTypeInt (void)
  {
    std::istringstream in ("int");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Int);
  }
  
  void testTypeUInt (void)
  {
    std::istringstream in ("unsigned int");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::UInt);
  }
  
  void testTypeFloat (void)
  {
    std::istringstream in ("float");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (testType->base, s1::parser::SemanticsHandler::Float);
  }
  
  void testTypeSampler1D (void)
  {
    std::istringstream in ("sampler1D");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Sampler);
    TS_ASSERT_EQUALS (testType->sampler, s1::parser::SemanticsHandler::_1D);
  }
  
  void testTypeSampler2D (void)
  {
    std::istringstream in ("sampler2D");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Sampler);
    TS_ASSERT_EQUALS (testType->sampler, s1::parser::SemanticsHandler::_2D);
  }
  
  void testTypeSampler3D (void)
  {
    std::istringstream in ("sampler3D");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Sampler);
    TS_ASSERT_EQUALS (testType->sampler, s1::parser::SemanticsHandler::_3D);
  }
  
  void testTypeSamplerCUBE (void)
  {
    std::istringstream in ("samplerCUBE");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Sampler);
    TS_ASSERT_EQUALS (testType->sampler, s1::parser::SemanticsHandler::CUBE);
  }
  
  void testTypeVectorBool (void)
  {
    std::istringstream in ("bool3");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Vector);
    TS_ASSERT_EQUALS (testType->vectorDim, 3);
    TestSemanticsHandler::TestType* vecType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (vecType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (vecType->base, s1::parser::SemanticsHandler::Bool);
  }
  
  void testTypeVectorInt (void)
  {
    std::istringstream in ("int3");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Vector);
    TS_ASSERT_EQUALS (testType->vectorDim, 3);
    TestSemanticsHandler::TestType* vecType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (vecType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (vecType->base, s1::parser::SemanticsHandler::Int);
  }
  
  void testTypeVectorUInt (void)
  {
    std::istringstream in ("unsigned int3");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Vector);
    TS_ASSERT_EQUALS (testType->vectorDim, 3);
    TestSemanticsHandler::TestType* vecType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (vecType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (vecType->base, s1::parser::SemanticsHandler::UInt);
  }
  
  void testTypeVectorFloat (void)
  {
    std::istringstream in ("float3");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Vector);
    TS_ASSERT_EQUALS (testType->vectorDim, 3);
    TestSemanticsHandler::TestType* vecType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (vecType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (vecType->base, s1::parser::SemanticsHandler::Float);
  }
  
  void testTypeMatrixBool (void)
  {
    std::istringstream in ("bool3x2");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Matrix);
    TS_ASSERT_EQUALS (testType->matrixCols, 3);
    TS_ASSERT_EQUALS (testType->matrixRows, 2);
    TestSemanticsHandler::TestType* matType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (matType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (matType->base, s1::parser::SemanticsHandler::Bool);
  }
  
  void testTypeMatrixInt (void)
  {
    std::istringstream in ("int3x2");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Matrix);
    TS_ASSERT_EQUALS (testType->matrixCols, 3);
    TS_ASSERT_EQUALS (testType->matrixRows, 2);
    TestSemanticsHandler::TestType* matType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (matType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (matType->base, s1::parser::SemanticsHandler::Int);
  }
  
  void testTypeMatrixUInt (void)
  {
    std::istringstream in ("unsigned int3x2");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Matrix);
    TS_ASSERT_EQUALS (testType->matrixCols, 3);
    TS_ASSERT_EQUALS (testType->matrixRows, 2);
    TestSemanticsHandler::TestType* matType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (matType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (matType->base, s1::parser::SemanticsHandler::UInt);
  }
  
  void testTypeMatrixFloat (void)
  {
    std::istringstream in ("float3x2");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Matrix);
    TS_ASSERT_EQUALS (testType->matrixCols, 3);
    TS_ASSERT_EQUALS (testType->matrixRows, 2);
    TestSemanticsHandler::TestType* matType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (matType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (matType->base, s1::parser::SemanticsHandler::Float);
  }
  
  void testTypeArray (void)
  {
    std::istringstream in ("int[]");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Array);
    TestSemanticsHandler::TestType* arrType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (arrType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (arrType->base, s1::parser::SemanticsHandler::Int);
  }
  
  void testTypeArrayArray (void)
  {
    std::istringstream in ("int[][]");
    s1::uc::Stream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    TestSemanticsHandler::TypePtr type;
    TS_ASSERT_THROWS_NOTHING ((type = parser.ParseType ()));
    TestSemanticsHandler::TestType* testType = 
      static_cast<TestSemanticsHandler::TestType*> (type.get());
    TS_ASSERT_EQUALS (testType->typeClass, TestSemanticsHandler::TestType::Array);
    TestSemanticsHandler::TestType* arrArrType = 
      static_cast<TestSemanticsHandler::TestType*> (testType->avmBase.get());
    TS_ASSERT_EQUALS (arrArrType->typeClass, TestSemanticsHandler::TestType::Array);
    TestSemanticsHandler::TestType* arrType = 
      static_cast<TestSemanticsHandler::TestType*> (arrArrType->avmBase.get());
    TS_ASSERT_EQUALS (arrType->typeClass, TestSemanticsHandler::TestType::Base);
    TS_ASSERT_EQUALS (arrType->base, s1::parser::SemanticsHandler::Int);
  }
};
