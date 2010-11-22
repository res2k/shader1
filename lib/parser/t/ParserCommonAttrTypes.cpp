#include <cxxtest/TestSuite.h>

#include "base/common.h"

#include "lexer/LexerErrorHandler.h"
#include "parser/Exception.h"
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"

class ParserCommonAttrTypesTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler,
		s1::parser::ErrorHandler& errorHandler)
     : Parser (inputLexer, semanticsHandler, errorHandler) {}
    
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
    TS_ASSERT_THROWS_NOTHING ((typeArray = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateArrayType (
	semanticsHandler.CreateType (CommonSemanticsHandler::Int)))));
      
    TestSemanticsHandler::Attribute attrLength =
      TestSemanticsHandler::IdentifyAttribute (UnicodeString ("length"));
    TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
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
    TS_ASSERT_THROWS_NOTHING ((typeInt4 = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateVectorType (
	semanticsHandler.CreateType (CommonSemanticsHandler::Int), 4))));

    {
      TestSemanticsHandler::Attribute attrX =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("x"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4, attrX));
      TS_ASSERT_EQUALS(typeAttr->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrXY =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("xy"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4, attrXY));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Vector);
      TS_ASSERT_EQUALS(typeAttr->vectorDim, 2);
      TestTypePtr typeBase = boost::shared_static_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrGA =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("ga"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4, attrGA));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Vector);
      TS_ASSERT_EQUALS(typeAttr->vectorDim, 2);
      TestTypePtr typeBase = boost::shared_static_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrXB =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("xb"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4, attrXB));
      TS_ASSERT_EQUALS(typeAttr, TestTypePtr ());
    }
    {
      TestSemanticsHandler::Attribute attrRZ =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("rz"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
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
    TS_ASSERT_THROWS_NOTHING ((typeInt4x3 = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateMatrixType (
	semanticsHandler.CreateType (CommonSemanticsHandler::Int), 4, 3))));

    {
      TestSemanticsHandler::Attribute attrCol =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("col"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4x3, attrCol));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Array);
      TestTypePtr typeArrayBase = boost::shared_static_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeArrayBase->typeClass, TestType::Vector);
      TS_ASSERT_EQUALS(typeArrayBase->vectorDim, 3);
      TestTypePtr typeBase = boost::shared_static_cast<TestType> (
	typeArrayBase->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrRow =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("row"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4x3, attrRow));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Array);
      TestTypePtr typeArrayBase = boost::shared_static_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeArrayBase->typeClass, TestType::Vector);
      TS_ASSERT_EQUALS(typeArrayBase->vectorDim, 4);
      TestTypePtr typeBase = boost::shared_static_cast<TestType> (
	typeArrayBase->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrTransp =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("transpose"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt4x3, attrTransp));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Matrix);
      TS_ASSERT_EQUALS(typeAttr->matrixCols, 3);
      TS_ASSERT_EQUALS(typeAttr->matrixRows, 4);
      TestTypePtr typeBase = boost::shared_static_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
    {
      TestSemanticsHandler::Attribute attrInv =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("invert"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
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
    TS_ASSERT_THROWS_NOTHING ((typeInt3x3 = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateMatrixType (
	semanticsHandler.CreateType (CommonSemanticsHandler::Int), 3, 3))));

    {
      TestSemanticsHandler::Attribute attrInv =
	TestSemanticsHandler::IdentifyAttribute (UnicodeString ("invert"));
      TestTypePtr typeAttr = boost::shared_static_cast<TestType> (
	semanticsHandler.GetAttributeType (typeInt3x3, attrInv));
      TS_ASSERT_EQUALS(typeAttr->typeClass, TestType::Matrix);
      TS_ASSERT_EQUALS(typeAttr->matrixCols, 3);
      TS_ASSERT_EQUALS(typeAttr->matrixRows, 3);
      TestTypePtr typeBase = boost::shared_static_cast<TestType> (
	typeAttr->avmBase);
      TS_ASSERT_EQUALS(typeBase->base, TestSemanticsHandler::Int);
    }
  }
};
