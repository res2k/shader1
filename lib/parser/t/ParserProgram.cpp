#include <cxxtest/TestSuite.h>

#include "base/hash_UnicodeString.h"

#include "lexer/LexerErrorHandler.h"
#include "parser/Exception.h"
#include "parser/Parser.h"

#include "TestSemanticsHandler.h"

#include "ParserTestTraits.h"

class ParserPorgramTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler)
     : Parser (inputLexer, semanticsHandler) {}
  };
public:
  void testProgramSimple (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("void main() {}");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.Parse ());
  }
  
  void testProgramSimple2 (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("void main (out float4 pos, out float4 color) {}");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.Parse ());
  }
  
  void testProgramLessSimple (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("void main() { int foo = 1; }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.Parse ());
  }
  
  void testProgramLessSimple2 (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("void main() { int foo = 1, bar; }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.Parse ());
  }
  
  void testProgramGlobalVar (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("int bar; void main() { int foo = bar; }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.Parse ());
  }

  void testProgramGlobalConst (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("const int bar = 1; void main() { int foo = bar; }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.Parse ());
  }
  
  void testProgramFunctionCall (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("void Foo() {} void main() { Foo(); }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.Parse ());
  }
  
  void testProgramFunctionCall2 (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("int Foo() { return 1; } void main() { int x = Foo(); }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.Parse ());
  }
  
  void testProgramFunctionCallCast (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("void main() { int x; x = float (1.2); }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    TestParser parser (lexer, semanticsHandler);
    
    TS_ASSERT_THROWS_NOTHING(parser.Parse ());
  }
  
};
