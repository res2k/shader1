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

#include "base/uc/SimpleBufferStreamSource.h"
#include "parser/ast/Type.h"
#include "parser/ast/Typedef.h"
#include "parser/ast/VarsDecl.h"
#include "parser/Exception.h"
#include "parser/Parser.h"

#include "AstMacros.h"
#include "TestParser.h"
#include "TestSemanticsHandler.h"

#include "../../diagnostics/t/TestDiagnosticsHandler.h"

class AstBlockStrictTestSuite : public CxxTest::TestSuite
{
public:
  void testBlockVarDecl (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[0]->value);
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");
    TS_ASSERT(!varsDecl->vars[0].initializer);
  }

  void testBlockVarDeclMulti (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a, b;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[0]->value);
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 2u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");
    TS_ASSERT(!varsDecl->vars[0].initializer);
    TS_ASSERT_EQUALS(varsDecl->vars[1].identifier.GetString(), "b");
    TS_ASSERT(!varsDecl->vars[1].initializer);
  }

  void testBlockVarDeclMulti2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a; int b;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 2u);
    {
      const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[0]->value);
      TS_ASSERT(!varsDecl->isConst);
      AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

      TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
      TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");
      TS_ASSERT(!varsDecl->vars[0].initializer);
    }
    {
      const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[1]->value);
      TS_ASSERT(!varsDecl->isConst);
      AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

      TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
      TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "b");
      TS_ASSERT(!varsDecl->vars[0].initializer);
    }
  }

  void testBlockVarDeclInit (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a = 1+2;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[0]->value);
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");

    const auto& varsDeclInitExprBinary = boost::get<ast::ExprBinaryPtr> (varsDecl->vars[0].initializer->value);
    AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->left), int, 1);
    TS_ASSERT_EQUALS(varsDeclInitExprBinary->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->right), int, 2);
  }

  void testBlockVarDeclInitMulti (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a = 1+2, b;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[0]->value);
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 2u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");

    const auto& varsDeclInitExprBinary = boost::get<ast::ExprBinaryPtr> (varsDecl->vars[0].initializer->value);
    AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->left), int, 1);
    TS_ASSERT_EQUALS(varsDeclInitExprBinary->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->right), int, 2);

    TS_ASSERT_EQUALS(varsDecl->vars[1].identifier.GetString(), "b");
    TS_ASSERT(!varsDecl->vars[1].initializer);
  }

  void testBlockVarDeclInitMultiError (void)
  {
    using namespace s1::parser;

    std::string inStr ("int a = 1+2, ;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
        TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS (errorHandler.parseError.code,
                      static_cast<unsigned int> (s1::parser::Error::UnexpectedToken));

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[0]->value);
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");

    const auto& varsDeclInitExprBinary = boost::get<ast::ExprBinaryPtr> (varsDecl->vars[0].initializer->value);
    AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->left), int, 1);
    TS_ASSERT_EQUALS(varsDeclInitExprBinary->op.typeOrID, s1::lexer::Plus);
    AST_TEST_EXPR_IS_NUMERIC(*(varsDeclInitExprBinary->right), int, 2);
  }

  void testBlockConstDecl (void)
  {
    using namespace s1::parser;

    std::string inStr ("const int a = 0;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[0]->value);
    TS_ASSERT(varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");

    AST_TEST_EXPR_IS_NUMERIC(*(varsDecl->vars[0].initializer), int, 0);
  }

  void testBlockConstDeclIncomplete (void)
  {
    using namespace s1::parser;

    std::string inStr ("const int a;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS (errorHandler.parseError.code,
                      static_cast<unsigned int> (s1::parser::Error::UnexpectedToken));

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[0]->value);
    TS_ASSERT(varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT(varsDecl->vars.empty());
  }

  void testBlockTypedef (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int MyInt;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto& typeDef = boost::get<ast::TypedefPtr> (block->statements[0]->value);
    AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
    TS_ASSERT_EQUALS(typeDef->alias.GetString(), "MyInt");
  }

  void testBlockTypedefIndirect (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int MyInt1; typedef MyInt1 MyInt;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 2u);
    {
      const auto& typeDef = boost::get<ast::TypedefPtr> (block->statements[0]->value);
      AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
      TS_ASSERT_EQUALS(typeDef->alias.GetString(), "MyInt1");
    }
    {
      const auto& typeDef = boost::get<ast::TypedefPtr> (block->statements[1]->value);
      AST_TEST_TYPE_IS_ALIAS(*typeDef->type, "MyInt1");
      TS_ASSERT_EQUALS(typeDef->alias.GetString(), "MyInt");
    }
  }

  void testBlockVarTypedefDecl (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int MyInt; MyInt a;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestSemanticsHandler semanticsHandler; // FIXME: Remove
    TestParser parser (lexer, semanticsHandler, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = parser.AstParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 2u);
    {
      const auto& typeDef = boost::get<ast::TypedefPtr> (block->statements[0]->value);
      AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
      TS_ASSERT_EQUALS(typeDef->alias.GetString(), "MyInt");
    }
    {
      const auto& varsDecl = boost::get<ast::VarsDeclPtr> (block->statements[1]->value);
      TS_ASSERT(!varsDecl->isConst);
      AST_TEST_TYPE_IS_ALIAS(*varsDecl->type, "MyInt");
      TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
      TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");
      TS_ASSERT(!varsDecl->vars[0].initializer);
    }
  }
};
