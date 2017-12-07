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

class ParserCommonAttrTypesTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler,
                s1::diagnostics::Handler& diagnosticsHandler)
     : Parser (inputLexer, semanticsHandler, diagnosticsHandler) {}
    
    using s1::Parser::ParseBlock;
  };
  
  class TestSemanticsHandler : public ::TestSemanticsHandler
  {
  public:
    typedef ::TestSemanticsHandler Superclass;
    typedef Superclass::Attribute Attribute;
    
    using Superclass::IdentifyAttribute;
    using Superclass::GetAttributeType;
  };
public:
  void testAttrTypeArray (void)
  {
    using namespace s1::parser;
    
    typedef TestSemanticsHandler::TestType TestType;
    typedef boost::shared_ptr<TestType> TestTypePtr;
    
    TestSemanticsHandler semanticsHandler;
    
    TestTypePtr typeArray;
    TS_ASSERT_THROWS_NOTHING ((typeArray = boost::static_pointer_cast<TestType> (
      semanticsHandler.CreateArrayType (
	semanticsHandler.CreateType (CommonSemanticsHandler::Int)))));
      
    TestSemanticsHandler::Attribute attrLength =
      TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("length"));
    TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
      semanticsHandler.GetAttributeType (typeArray, attrLength));
    TS_ASSERT_EQUALS(typeAttr->base, TestSemanticsHandler::UInt);
  }
  
  void testAttrTypeVector (void)
  {
    using namespace s1::parser;
    
    typedef TestSemanticsHandler::TestType TestType;
    typedef boost::shared_ptr<TestType> TestTypePtr;
    
    TestSemanticsHandler semanticsHandler;
    
    TestTypePtr typeInt4;
    TS_ASSERT_THROWS_NOTHING ((typeInt4 = boost::static_pointer_cast<TestType> (
      semanticsHandler.CreateVectorType (
	semanticsHandler.CreateType (CommonSemanticsHandler::Int), 4))));

    {
      TestSemanticsHandler::Attribute attrX =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("x"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4, attrX));
      TS_ASSERT_EQUALS(typeAttr->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrXY =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("xy"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4, attrXY));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Vector);
      TS_ASSERT_EQUALS(typeAttr->vectorDim, 2);
      TestTypePtr typeBase = boost::static_pointer_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrGA =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("ga"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4, attrGA));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Vector);
      TS_ASSERT_EQUALS(typeAttr->vectorDim, 2);
      TestTypePtr typeBase = boost::static_pointer_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrXB =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("xb"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4, attrXB));
      TS_ASSERT_EQUALS(typeAttr, TestTypePtr ());
    }
    {
      TestSemanticsHandler::Attribute attrRZ =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("rz"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4, attrRZ));
      TS_ASSERT_EQUALS(typeAttr, TestTypePtr ());
    }
  }
  
  void testAttrTypeMatrix (void)
  {
    using namespace s1::parser;
    
    typedef TestSemanticsHandler::TestType TestType;
    typedef boost::shared_ptr<TestType> TestTypePtr;
    
    TestSemanticsHandler semanticsHandler;
    
    TestTypePtr typeInt4x3;
    TS_ASSERT_THROWS_NOTHING ((typeInt4x3 = boost::static_pointer_cast<TestType> (
      semanticsHandler.CreateMatrixType (
	semanticsHandler.CreateType (CommonSemanticsHandler::Int), 4, 3))));

    {
      TestSemanticsHandler::Attribute attrCol =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("col"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4x3, attrCol));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Array);
      TestTypePtr typeArrayBase = boost::static_pointer_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeArrayBase->typeClass, TestType::Vector);
      TS_ASSERT_EQUALS(typeArrayBase->vectorDim, 3);
      TestTypePtr typeBase = boost::static_pointer_cast<TestType> (
	typeArrayBase->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrRow =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("row"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4x3, attrRow));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Array);
      TestTypePtr typeArrayBase = boost::static_pointer_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeArrayBase->typeClass, TestType::Vector);
      TS_ASSERT_EQUALS(typeArrayBase->vectorDim, 4);
      TestTypePtr typeBase = boost::static_pointer_cast<TestType> (
	typeArrayBase->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrTransp =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("transpose"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4x3, attrTransp));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Matrix);
      TS_ASSERT_EQUALS(typeAttr->matrixCols, 3);
      TS_ASSERT_EQUALS(typeAttr->matrixRows, 4);
      TestTypePtr typeBase = boost::static_pointer_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrInv =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("invert"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4x3, attrInv));
      TS_ASSERT_EQUALS(typeAttr, TestTypePtr ());
    }
  }

  void testAttrTypeMatrix2 (void)
  {
    using namespace s1::parser;
    
    typedef TestSemanticsHandler::TestType TestType;
    typedef boost::shared_ptr<TestType> TestTypePtr;
    
    TestSemanticsHandler semanticsHandler;
    
    TestTypePtr typeInt3x3;
    TS_ASSERT_THROWS_NOTHING ((typeInt3x3 = boost::static_pointer_cast<TestType> (
      semanticsHandler.CreateMatrixType (
	semanticsHandler.CreateType (CommonSemanticsHandler::Int), 3, 3))));

    {
      TestSemanticsHandler::Attribute attrInv =
	TestSemanticsHandler::IdentifyAttribute (s1::uc::String ("invert"));
      TestTypePtr typeAttr = boost::static_pointer_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt3x3, attrInv));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Matrix);
      TS_ASSERT_EQUALS(typeAttr->matrixCols, 3);
      TS_ASSERT_EQUALS(typeAttr->matrixRows, 3);
      TestTypePtr typeBase = boost::static_pointer_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
  }
};
