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

#include "TestSemanticsHandlerSloppyIdentifiers.h"

#include "ParserTestTraits.h"

class ParserBlockTestSuite : public CxxTest::TestSuite 
{
  class TestParser : public s1::Parser
  {
  public:
    TestParser (s1::Lexer& inputLexer, s1::parser::SemanticsHandler& semanticsHandler,
		s1::parser::ErrorHandler& errorHandler)
     : Parser (inputLexer, semanticsHandler, errorHandler) {}
    
    using s1::Parser::ParseBlock;
  };
  
  typedef TestSemanticsHandlerSloppyIdentifiers TestSemanticsHandler;
public:
  void testBlockExpr (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("a = b;");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestBlock* testBlock = 
      static_cast<TestSemanticsHandler::TestBlock*> (block.get());
    TS_ASSERT_EQUALS(testBlock->GetBlockString(),
		     "  (a = b);\n");
  }
  
  void testBlockStatementIncomplete (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("a = b");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestBlock* testBlock = 
      static_cast<TestSemanticsHandler::TestBlock*> (block.get());
    TS_ASSERT_EQUALS(testBlock->GetBlockString(),
		     "");
  }  
  
  void testBlockBranch (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("if (a) { c = d; }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestBlock* testBlock = 
      static_cast<TestSemanticsHandler::TestBlock*> (block.get());
    TS_ASSERT_EQUALS(testBlock->GetBlockString(),
		     "  if (a)\n"
		     "  {\n"
		     "    (c = d);\n"
		     "  }\n");
  }
  
  void testBlockBranch2 (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("if (a) { c = d; } else { c = e; }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestBlock* testBlock = 
      static_cast<TestSemanticsHandler::TestBlock*> (block.get());
    TS_ASSERT_EQUALS(testBlock->GetBlockString(),
		     "  if (a)\n"
		     "  {\n"
		     "    (c = d);\n"
		     "  }\n"
		     "  else\n"
		     "  {\n"
		     "    (c = e);\n"
		     "  }\n");
  }
  
  void testBlockWhile (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("while (a) { c = d; }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestBlock* testBlock = 
      static_cast<TestSemanticsHandler::TestBlock*> (block.get());
    TS_ASSERT_EQUALS(testBlock->GetBlockString(),
		     "  while (a)\n"
		     "  {\n"
		     "    (c = d);\n"
		     "  }\n");
  }
  
  void testBlockFor (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("for (a; b; b = b+1) { c = d; }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestBlock* testBlock = 
      static_cast<TestSemanticsHandler::TestBlock*> (block.get());
    TS_ASSERT_EQUALS(testBlock->GetBlockString(),
		     "  for (a; b; (b = (b + 1)))\n"
		     "  {\n"
		     "    (c = d);\n"
		     "  }\n");
  }
  
  void testBlockNested (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("{ a = b; }");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestBlock* testBlock = 
      static_cast<TestSemanticsHandler::TestBlock*> (block.get());
    TS_ASSERT_EQUALS(testBlock->GetBlockString(),
		     "  {\n"
		     "    (a = b);\n"
		     "  }\n");
  }
  
  void testBlockErrorRecovery (void)
  {
    using namespace s1::parser;
    
    std::istringstream in ("a = b error; c=d;");
    s1::UnicodeStream ustream (in, "utf-8");
    s1::LexerErrorHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler;
    s1::parser::ErrorHandler parserErrorHandler;
    TestParser parser (lexer, semanticsHandler, parserErrorHandler);
    
    SemanticsHandler::BlockPtr block (
      semanticsHandler.CreateBlock (SemanticsHandler::ScopePtr()));
    TS_ASSERT_THROWS_NOTHING(parser.ParseBlock (block));
    TestSemanticsHandler::TestBlock* testBlock = 
      static_cast<TestSemanticsHandler::TestBlock*> (block.get());
    TS_ASSERT_EQUALS(testBlock->GetBlockString(),
		     "  (c = d);\n");
  }  
};
