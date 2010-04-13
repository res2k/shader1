#include <cxxtest/TestSuite.h>

#include "base/hash_UnicodeString.h"

#include "lexer/LexerErrorHandler.h"
#include "parser/Exception.h"
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"

class ParserCommonTypeCompatTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler)
     : Parser (inputLexer, semanticsHandler) {}
    
    using s1::Parser::ParseBlock;
  };
  
  class TestSemanticsHandler : public TestSemanticsHandlerTemplated<0>
  {
  public:
    typedef TestSemanticsHandlerTemplated<0> SuperClass;
    
    using SuperClass::GetHigherPrecisionType;
  };
public:
  void testCompatBase (void)
  {
    using namespace s1::parser;
    typedef TestSemanticsHandler::TestType TestType;
    typedef boost::shared_ptr<TestType> TestTypePtr;
    
    TestSemanticsHandler semanticsHandler;
    
    TestTypePtr typeBool;
    TS_ASSERT_THROWS_NOTHING ((typeBool = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateType (CommonSemanticsHandler::Bool))));
    TestTypePtr typeInt;
    TS_ASSERT_THROWS_NOTHING ((typeInt = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateType (CommonSemanticsHandler::Int))));
    TestTypePtr typeUInt;
    TS_ASSERT_THROWS_NOTHING ((typeUInt = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateType (CommonSemanticsHandler::UInt))));
    TestTypePtr typeFloat;
    TS_ASSERT_THROWS_NOTHING ((typeFloat = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateType (CommonSemanticsHandler::Float))));
    
    // Bool - only compatible to itself
    {
      TS_ASSERT_EQUALS (typeBool->CompatibleLossless (*typeBool), true);
      TS_ASSERT_EQUALS (typeBool->CompatibleLossy (*typeBool), true);
      TS_ASSERT_EQUALS (typeBool->IsPrecisionHigherEqual (*typeBool), true);
      
      TS_ASSERT_EQUALS (typeBool->CompatibleLossless (*typeInt), false);
      TS_ASSERT_EQUALS (typeBool->CompatibleLossy (*typeInt), false);
      TS_ASSERT_EQUALS (typeBool->IsPrecisionHigherEqual (*typeInt), false);
      
      TS_ASSERT_EQUALS (typeBool->CompatibleLossless (*typeUInt), false);
      TS_ASSERT_EQUALS (typeBool->CompatibleLossy (*typeUInt), false);
      TS_ASSERT_EQUALS (typeBool->IsPrecisionHigherEqual (*typeUInt), false);
      
      TS_ASSERT_EQUALS (typeBool->CompatibleLossless (*typeFloat), false);
      TS_ASSERT_EQUALS (typeBool->CompatibleLossy (*typeFloat), false);
      TS_ASSERT_EQUALS (typeBool->IsPrecisionHigherEqual (*typeFloat), false);
    }
    // Int - compatible to other numeric types
    {
      TS_ASSERT_EQUALS (typeInt->CompatibleLossless (*typeBool), false);
      TS_ASSERT_EQUALS (typeInt->CompatibleLossy (*typeBool), false);
      TS_ASSERT_EQUALS (typeInt->IsPrecisionHigherEqual (*typeBool), false);
      
      TS_ASSERT_EQUALS (typeInt->CompatibleLossless (*typeInt), true);
      TS_ASSERT_EQUALS (typeInt->CompatibleLossy (*typeInt), true);
      TS_ASSERT_EQUALS (typeInt->IsPrecisionHigherEqual (*typeInt), true);
      
      TS_ASSERT_EQUALS (typeInt->CompatibleLossless (*typeUInt), true);
      TS_ASSERT_EQUALS (typeInt->CompatibleLossy (*typeUInt), true);
      TS_ASSERT_EQUALS (typeInt->IsPrecisionHigherEqual (*typeUInt), true);
      
      TS_ASSERT_EQUALS (typeInt->CompatibleLossless (*typeFloat), true);
      TS_ASSERT_EQUALS (typeInt->CompatibleLossy (*typeFloat), true);
      // int is lower prec than float
      TS_ASSERT_EQUALS (typeInt->IsPrecisionHigherEqual (*typeFloat), false);
    }
    // UInt - compatible to other numeric types
    {
      TS_ASSERT_EQUALS (typeUInt->CompatibleLossless (*typeBool), false);
      TS_ASSERT_EQUALS (typeUInt->CompatibleLossy (*typeBool), false);
      TS_ASSERT_EQUALS (typeUInt->IsPrecisionHigherEqual (*typeBool), false);
      
      TS_ASSERT_EQUALS (typeUInt->CompatibleLossless (*typeInt), true);
      TS_ASSERT_EQUALS (typeUInt->CompatibleLossy (*typeInt), true);
      // uint, despite "lossless" assignable to, is lower prec than int!
      TS_ASSERT_EQUALS (typeUInt->IsPrecisionHigherEqual (*typeInt), false);
      
      TS_ASSERT_EQUALS (typeUInt->CompatibleLossless (*typeUInt), true);
      TS_ASSERT_EQUALS (typeUInt->CompatibleLossy (*typeUInt), true);
      TS_ASSERT_EQUALS (typeUInt->IsPrecisionHigherEqual (*typeUInt), true);
      
      TS_ASSERT_EQUALS (typeUInt->CompatibleLossless (*typeFloat), true);
      TS_ASSERT_EQUALS (typeUInt->CompatibleLossy (*typeFloat), true);
      // uint is lower prec than float
      TS_ASSERT_EQUALS (typeUInt->IsPrecisionHigherEqual (*typeFloat), false);
    }
    // Float - compatible to other numeric types
    {
      TS_ASSERT_EQUALS (typeFloat->CompatibleLossless (*typeBool), false);
      TS_ASSERT_EQUALS (typeFloat->CompatibleLossy (*typeBool), false);
      TS_ASSERT_EQUALS (typeFloat->IsPrecisionHigherEqual (*typeBool), false);
      
      // float -> int not lossless
      TS_ASSERT_EQUALS (typeFloat->CompatibleLossless (*typeInt), false);
      TS_ASSERT_EQUALS (typeFloat->CompatibleLossy (*typeInt), true);
      // float is higher prec than int
      TS_ASSERT_EQUALS (typeFloat->IsPrecisionHigherEqual (*typeInt), true);
      
      // float -> uint not lossless
      TS_ASSERT_EQUALS (typeFloat->CompatibleLossless (*typeUInt), false);
      TS_ASSERT_EQUALS (typeFloat->CompatibleLossy (*typeUInt), true);
      // float is higher prec than uint
      TS_ASSERT_EQUALS (typeFloat->IsPrecisionHigherEqual (*typeUInt), true);
      
      TS_ASSERT_EQUALS (typeFloat->CompatibleLossless (*typeFloat), true);
      TS_ASSERT_EQUALS (typeFloat->CompatibleLossy (*typeFloat), true);
      TS_ASSERT_EQUALS (typeFloat->IsPrecisionHigherEqual (*typeFloat), true);
    }
  }
  
  void testHigherPrecision (void)
  {
    using namespace s1::parser;
    typedef TestSemanticsHandler::TestType TestType;
    typedef boost::shared_ptr<TestType> TestTypePtr;
    
    TestSemanticsHandler semanticsHandler;
    
    TestTypePtr typeBool;
    TS_ASSERT_THROWS_NOTHING ((typeBool = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateType (CommonSemanticsHandler::Bool))));
    TestTypePtr typeInt;
    TS_ASSERT_THROWS_NOTHING ((typeInt = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateType (CommonSemanticsHandler::Int))));
    TestTypePtr typeUInt;
    TS_ASSERT_THROWS_NOTHING ((typeUInt = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateType (CommonSemanticsHandler::UInt))));
    TestTypePtr typeFloat;
    TS_ASSERT_THROWS_NOTHING ((typeFloat = boost::shared_static_cast<TestType> (
      semanticsHandler.CreateType (CommonSemanticsHandler::Float))));
    
    TS_ASSERT_EQUALS (TestSemanticsHandler::GetHigherPrecisionType (typeBool, typeInt),
		      TestTypePtr ());
    TS_ASSERT_EQUALS (TestSemanticsHandler::GetHigherPrecisionType (typeBool, typeUInt),
		      TestTypePtr ());
    TS_ASSERT_EQUALS (TestSemanticsHandler::GetHigherPrecisionType (typeBool, typeFloat),
		      TestTypePtr ());
      
    TS_ASSERT_EQUALS (TestSemanticsHandler::GetHigherPrecisionType (typeInt, typeUInt),
		      typeInt);
    TS_ASSERT_EQUALS (TestSemanticsHandler::GetHigherPrecisionType (typeInt, typeFloat),
		      typeFloat);
      
    TS_ASSERT_EQUALS (TestSemanticsHandler::GetHigherPrecisionType (typeUInt, typeFloat),
		      typeFloat);
  }
};
