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
#include "parser/ast/BlockStatementTypedef.h"
#include "parser/ast/BlockStatementVarsDecl.h"
#include "parser/ast/Type.h"
#include "parser/ast/Typedef.h"
#include "parser/ast/VarsDecl.h"

#include "AstMacros.h"
#include "TestAstBuilder.h"

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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::VarsDecl*> (block->statements[0].get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 2u);
    {
      const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
      TS_ASSERT(!varsDecl->isConst);
      AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

      TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
      TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");
      TS_ASSERT(!varsDecl->vars[0].initializer);
    }
    {
      const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[1].get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");

    const auto varsDeclInitExprBinary = dynamic_cast<const ast::ExprBinary*> (varsDecl->vars[0].initializer.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 2u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");

    const auto varsDeclInitExprBinary = dynamic_cast<const ast::ExprBinary*> (varsDecl->vars[0].initializer.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS (errorHandler.parseError.code,
                      static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
    TS_ASSERT(!varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");

    const auto varsDeclInitExprBinary = dynamic_cast<const ast::ExprBinary*> (varsDecl->vars[0].initializer.get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS (errorHandler.parseError.code,
                      static_cast<unsigned int> (s1::parser::Error::ExpectedAssign));

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[0].get());
    TS_ASSERT(varsDecl->isConst);
    AST_TEST_TYPE_IS_WELL_KNOWN(*varsDecl->type, kwInt);

    // Parsing will produce variable w/o initializer
    TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
    TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");
    TS_ASSERT(!varsDecl->vars[0].initializer);
  }

  void testBlockTypedef (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int MyInt;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 2u);
    {
      const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
      AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
      TS_ASSERT_EQUALS(typeDef->alias.GetString(), "MyInt1");
    }
    {
      const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[1].get());
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
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code, 0);

    TS_ASSERT_EQUALS(block->statements.size(), 2u);
    {
      const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
      AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
      TS_ASSERT_EQUALS(typeDef->alias.GetString(), "MyInt");
    }
    {
      const auto varsDecl = dynamic_cast<const ast::BlockStatementVarsDecl*> (block->statements[1].get());
      TS_ASSERT(!varsDecl->isConst);
      AST_TEST_TYPE_IS_ALIAS(*varsDecl->type, "MyInt");
      TS_ASSERT_EQUALS(varsDecl->vars.size(), 1u);
      TS_ASSERT_EQUALS(varsDecl->vars[0].identifier.GetString(), "a");
      TS_ASSERT(!varsDecl->vars[0].initializer);
    }
  }

  void testBlockTypedefInvalid1 (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef 0 MyInt;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedTypeOrIdentifier));

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
    TS_ASSERT(!typeDef->type);
    TS_ASSERT_EQUALS(typeDef->alias.GetString(), "MyInt");
  }

  void testBlockTypedefInvalid2 (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int float;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
    AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
    TS_ASSERT_EQUALS(typeDef->alias.GetString(), "float");
  }

  void testBlockTypedefInvalid3 (void)
  {
    using namespace s1::parser;

    std::string inStr ("typedef int;");
    s1::uc::SimpleBufferStreamSource in (inStr.data(), inStr.size());
    s1::uc::Stream ustream (in);
    TestDiagnosticsHandler errorHandler;
    s1::Lexer lexer (ustream, errorHandler);
    TestAstBuilder astBuilder (lexer, errorHandler);

    s1::parser::ast::BlockPtr block;
    TS_ASSERT_THROWS_NOTHING((block = astBuilder.ParseBlock ()));
    TS_ASSERT_EQUALS(errorHandler.parseError.code,
                     static_cast<unsigned int> (s1::parser::Error::ExpectedIdentifier));

    TS_ASSERT_EQUALS(block->statements.size(), 1u);
    const auto typeDef = dynamic_cast<const ast::BlockStatementTypedef*> (block->statements[0].get());
    AST_TEST_TYPE_IS_WELL_KNOWN(*typeDef->type, kwInt);
    TS_ASSERT_EQUALS(typeDef->alias.GetString(), ";");
  }
};
