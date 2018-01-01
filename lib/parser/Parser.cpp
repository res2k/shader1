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

#include "base/common.h"

#include "parser/Parser.h"

#include "parser/ast/Block.h"
#include "parser/ast/BlockStatement.h"
#include "parser/ast/BlockStatementReturn.h"
#include "parser/ast/Expr.h"
#include "parser/ast/ExprValue.h"
#include "parser/ast/FunctionDecl.h"
#include "parser/ast/Identifier.h"
#include "parser/ast/Program.h"
#include "parser/ast/Type.h"
#include "parser/ast/Typedef.h"
#include "parser/ast/VarsDecl.h"
#include "parser/Diagnostics.h"
#include "parser/Exception.h"

#include <assert.h>
#include <bitset>

#include <boost/optional.hpp>

namespace s1
{
  using namespace parser;
  
  Parser::Parser (Lexer& inputLexer, SemanticsHandler& semanticsHandler,
                  diagnostics::Handler& diagnosticsHandler)
   : inputLexer (inputLexer), semanticsHandler (semanticsHandler),
    diagnosticsHandler (diagnosticsHandler)
  {
    NextToken ();
    
    builtinScope = semanticsHandler.CreateScope (Scope (), SemanticsHandler::Builtin);
  }

  void Parser::Parse ()
  {
    try
    {
      ParseProgram();
    }
    catch (const Exception& e)
    {
      /* emit error */
      diagnosticsHandler.ParseError (e.GetCode(), e.GetEncounteredToken(), e.GetExpectedToken());
    }
  }
  
  void Parser::NextToken ()
  {
    previousToken = currentToken;
    if (nextTokens.size() == 0)
    {
      currentToken = *inputLexer;
      ++inputLexer;
    }
    else
    {
      currentToken = nextTokens.front ();
      nextTokens.pop_front ();
    }
  }
  
  const Lexer::Token& Parser::Peek (size_t lookahead)
  {
    while (lookahead >= nextTokens.size())
    {
      nextTokens.push_back (*inputLexer);
      ++inputLexer;
    }
    return nextTokens[lookahead];
  }

  namespace
  {
    // Helper: Set end and start location for a AST parse result
    template<typename T>
    static inline void SetParseResultLocation (T& value,
                                               lexer::TokenLocation&& startLocation,
                                               lexer::TokenLocation&& endLocation)
    {
      // Assume T derives from ast::Node
      if (value)
      {
        value->startLocation = std::move (startLocation);
        value->endLocation = std::move (endLocation);
      }
    }

    template<typename T, typename U, typename V>
    static inline void SetParseResultLocation (OUTCOME_V2_NAMESPACE::result<T, U, V>& value,
                                               lexer::TokenLocation&& startLocation,
                                               lexer::TokenLocation&& endLocation)
    {
      // Assume T derives from ast::Node
      if (value.has_value())
      {
        SetParseResultLocation (value.value (), std::move (startLocation), std::move (endLocation));
      }
    }
  } // anonymous namespace

  template<typename F>
  typename std::result_of<F()>::type Parser::CommonAstParseNode (F func)
  {
    auto startLocation = currentToken.location;
    auto result = func ();
    SetParseResultLocation (result, std::move (startLocation), lexer::TokenLocation (previousToken.location));
    return result;
  }

  ast::Identifier Parser::AstParseIdentifier ()
  {
    Expect (lexer::Identifier);
    ast::Identifier astIdent = ast::Identifier{ currentToken };
    NextToken ();
    return astIdent;
  }

  void Parser::Expect (Lexer::TokenType tokenType)
  {
    if (currentToken.typeOrID != tokenType)
      throw Exception (parser::Error::UnexpectedToken, currentToken, tokenType);
  }
  
  void Parser::UnexpectedToken ()
  {
    throw Exception (parser::Error::UnexpectedToken, currentToken);
  }

  void Parser::ParseProgram ()
  {
    Scope globalScope (semanticsHandler.CreateScope (builtinScope, SemanticsHandler::Global));
    auto astProgram = AstParseProgram ();
    if (currentToken.typeOrID != lexer::EndOfFile)
    {
      diagnosticsHandler.ParseError (Error::UnexpectedToken, currentToken, lexer::Identifier);
    }
    ParseProgramStatements (globalScope, *astProgram);
  }

  ast::ProgramPtr Parser::AstParseProgram ()
  {
    return CommonAstParseNode (
      [&]()
      {
        ast::Program::StatementsContainer statements;
        while (true)
        {
          ast::Program::Statement statement;
          int beyondType;
          bool isType = IsWellKnownType (beyondType);
          if (currentToken.typeOrID == lexer::kwConst)
          {
            /* constant declaration */
            NextToken ();
            auto decl = AstParseVarsDecl (true);
            Expect (lexer::Semicolon);
            NextToken();
            statement = std::move (decl);
          }
          else if (isType
                  || (currentToken.typeOrID == lexer::kwVoid)
                  || (Peek (beyondType).typeOrID == lexer::Identifier))
          {
            if ((currentToken.typeOrID == lexer::kwVoid)
                || (Peek (beyondType+1).typeOrID == lexer::ParenL))
            {
              /* Function declaration */
              auto decl = AstParseFunctionDecl ();
              statement = std::move (decl);
            }
            else
            {
              /* Variable declaration */
              auto decl = AstParseVarsDecl (false);
              Expect (lexer::Semicolon);
              NextToken();
              statement = std::move (decl);
            }
          }
          else if (currentToken.typeOrID == lexer::kwTypedef)
          {
            /* Type definition */
            auto typeDef = AstParseTypedef ();
            Expect (lexer::Semicolon);
            NextToken();
            statement = std::move (typeDef);
          }
          else
            /* Unknown token - let caller throw error */
            break;
          statements.emplace_back (std::move (statement));
        }
        return ast::ProgramPtr (new ast::Program (std::move (statements)));
      });
  }

  void Parser::ParseProgramStatements (const Scope& scope, const ast::Program& astProgram)
  {
    for (const auto& statement : astProgram.statements)
    {
      if (auto astTypedef = boost::get<ast::TypedefPtr> (&statement))
      {
        ParseTypedef (scope, **astTypedef);
      }
      else if (auto astVarsDecl = boost::get<ast::VarsDeclPtr> (&statement))
      {
        ParseVarDeclare (scope, **astVarsDecl);
      }
      else if (auto astFuncDecl = boost::get<ast::FunctionDeclPtr> (&statement))
      {
        ParseFuncDeclare (scope, **astFuncDecl);
      }
      else
      {
        S1_ASSERT_NOT_REACHED_MSG("unhandled program statement", S1_ASSERT_RET_VOID);
      }
    }
  }

  void Parser::ParseBlock (Block block, const parser::ast::Block& astBlock)
  {
    Scope blockScope = block->GetInnerScope();
    for (const auto& statement : astBlock.statements)
    {
      if (auto astVarsDecl = boost::get<ast::VarsDeclPtr> (&statement->value))
      {
        ParseVarDeclare (blockScope, **astVarsDecl);
      }
      else if (auto astTypedef = boost::get<ast::TypedefPtr> (&statement->value))
      {
        ParseTypedef (blockScope, **astTypedef);
      }
      else if (auto astExpr = boost::get<ast::ExprPtr> (&statement->value))
      {
        auto expr = ParseExpression (blockScope, **astExpr);
        block->AddExpressionCommand (expr);
      }
      else if (auto astStatementReturn = boost::get<ast::BlockStatementReturnPtr> (&statement->value))
      {
        Expression returnExpr;
        if ((*astStatementReturn)->expr)
        {
          /* Return with some value */
          returnExpr = ParseExpression (blockScope, *(*astStatementReturn)->expr);
        }
        block->AddReturnCommand (returnExpr);
      }
      else if (auto astStatementFor = boost::get<ast::BlockStatementForPtr> (&statement->value))
      {
        Expression initExpr, loopTestExpr, loopFootExpr;
        if ((*astStatementFor)->initExpr)
        {
          initExpr = ParseExpression (blockScope, *(*astStatementFor)->initExpr);
        }
        if ((*astStatementFor)->condition)
        {
          loopTestExpr = ParseExpression (blockScope, *(*astStatementFor)->condition);
        }
        if ((*astStatementFor)->footExpr)
        {
          loopFootExpr = ParseExpression (blockScope, *(*astStatementFor)->footExpr);
        }
        Block newBlock = semanticsHandler.CreateBlock (blockScope);
        ParseBlock (newBlock, *(*astStatementFor)->bodyBlock);
        block->AddForLoop (initExpr, loopTestExpr, loopFootExpr, newBlock);
      }
      else if (auto astStatementIf = boost::get<ast::BlockStatementIfPtr> (&statement->value))
      {
        auto conditionExpr = ParseExpression (blockScope, *(*astStatementIf)->condition);
        Block ifBlock = semanticsHandler.CreateBlock (blockScope);
        ParseBlock (ifBlock, *(*astStatementIf)->ifBlock);
        Block elseBlock;
        if ((*astStatementIf)->elseBlock)
        {
          elseBlock = semanticsHandler.CreateBlock (blockScope);
          ParseBlock (elseBlock, *(*astStatementIf)->elseBlock);
        }
        block->AddBranching (conditionExpr, ifBlock, elseBlock);
      }
      else if (auto astStatementWhile = boost::get<ast::BlockStatementWhilePtr> (&statement->value))
      {
        auto loopTestExpr = ParseExpression (blockScope, *(*astStatementWhile)->condition);
        Block newBlock = semanticsHandler.CreateBlock (blockScope);
        ParseBlock (newBlock, *(*astStatementWhile)->bodyBlock);
        block->AddWhileLoop (loopTestExpr, newBlock);
      }
      else if (auto astBlock = boost::get<ast::BlockPtr> (&statement->value))
      {
        /* nested block */
        Block newBlock = semanticsHandler.CreateBlock (blockScope);
        ParseBlock (newBlock, **astBlock);
        block->AddNestedBlock (newBlock);
      }
      else
      {
        S1_ASSERT_NOT_REACHED(S1_ASSERT_RET_VOID);
      }
    }
  }
  
  bool Parser::IsCommand ()
  {
    return IsExpression ()
      || (currentToken.typeOrID == lexer::kwReturn)
      || (currentToken.typeOrID == lexer::kwIf)
      || (currentToken.typeOrID == lexer::kwWhile)
      || (currentToken.typeOrID == lexer::kwFor)
      || (currentToken.typeOrID == lexer::BraceL);
  }
  
  ast::BlockPtr Parser::AstParseBlock ()
  {
    return CommonAstParseNode (
      [&]()
      {
        ast::Block::StatementsContainer statements;
        while (true)
        {
          try
          {
            int beyondType;
            bool isType = IsWellKnownTypeOrArray (beyondType);
            if (currentToken.typeOrID == lexer::kwConst)
            {
              /* constant declaration */
              NextToken(); // skip 'const'
              auto astConsts = AstParseVarsDecl (true);
              statements.emplace_back (new ast::BlockStatement (std::move (astConsts)));
              Expect (lexer::Semicolon);
              NextToken();
            }
            else if ((isType && (Peek (beyondType).typeOrID == lexer::Identifier))
                || ((currentToken.typeOrID == lexer::Identifier) && (Peek ().typeOrID == lexer::Identifier)))
            {
              /* Variable declaration */
              auto astVars = AstParseVarsDecl ();
              statements.emplace_back (new ast::BlockStatement (std::move (astVars)));
              Expect (lexer::Semicolon);
              NextToken();
            }
            else if (currentToken.typeOrID == lexer::kwTypedef)
            {
              /* Type definition */
              auto astTypedef = AstParseTypedef ();
              statements.emplace_back (new ast::BlockStatement (std::move (astTypedef)));
              Expect (lexer::Semicolon);
              NextToken();
            }
            else if (IsExpression ())
            {
              auto astExpr = AstParseExpression ();
              statements.emplace_back (new ast::BlockStatement (std::move (astExpr)));
              Expect (lexer::Semicolon);
              NextToken();
            }
            else if (currentToken.typeOrID == lexer::kwReturn)
            {
              auto astReturn = AstParseStatementReturn ();
              statements.emplace_back (new ast::BlockStatement (std::move (astReturn)));
            }
            else if (currentToken.typeOrID == lexer::kwIf)
            {
              /* 'if' */
              auto astIf = AstParseIf ();
              statements.emplace_back (new ast::BlockStatement (std::move (astIf)));
            }
            else if (currentToken.typeOrID == lexer::kwWhile)
            {
              /* 'while' */
              auto astWhile = AstParseWhile ();
              statements.emplace_back (new ast::BlockStatement (std::move (astWhile)));
            }
            else if (currentToken.typeOrID == lexer::kwFor)
            {
              /* 'for' */
              auto astFor = AstParseFor ();
              statements.emplace_back (new ast::BlockStatement (std::move (astFor)));
            }
            else if (currentToken.typeOrID == lexer::BraceL)
            {
              /* nested block */
              NextToken ();
              auto astBlock = AstParseBlock ();
              statements.emplace_back (new ast::BlockStatement (std::move (astBlock)));
              Expect (lexer::BraceR);
              NextToken ();
            }
            else
              break;
            /* TODO: could improve behaviour in case unexpected tokens are encountered */
          }
          catch (const Exception& e)
          {
            /* emit error */
            diagnosticsHandler.ParseError (e.GetCode(), e.GetEncounteredToken(), e.GetExpectedToken());
            // Seek next ';' (end of statement) or '}' (end of block)
            while ((currentToken.typeOrID != lexer::Semicolon)
              && (currentToken.typeOrID != lexer::BraceR)
              && (currentToken.typeOrID != lexer::EndOfFile))
            {
              NextToken();
            }
            if (currentToken.typeOrID == lexer::Semicolon)
            {
              NextToken ();
              continue;
            }
            else
              // '}', exit block
              break;
          }
        }
        return ast::BlockPtr (new ast::Block (std::move (statements)));
      });
  }

  ast::BlockStatementReturnPtr Parser::AstParseStatementReturn ()
  {
    return CommonAstParseNode (
      [&]()
      {
        /* 'return' instruction */
        NextToken();
        ast::ExprPtr returnExpr;
        if (currentToken.typeOrID != lexer::Semicolon)
        {
          returnExpr = AstParseExpression ();
        }
        Expect (lexer::Semicolon);
        NextToken();
        return ast::BlockStatementReturnPtr (new ast::BlockStatementReturn (std::move (returnExpr)));
      });
  }
  
  bool Parser::IsBinaryOperationToken (Lexer::TokenType tokenType)
  {
    switch (tokenType)
    {
    case lexer::Equals:
    case lexer::NotEquals:
    case lexer::Larger:
    case lexer::LargerEqual:
    case lexer::Smaller:
    case lexer::SmallerEqual:
    case lexer::Plus:
    case lexer::Minus:
    case lexer::Mult:
    case lexer::Div:
    case lexer::Mod:
    case lexer::LogicOr:
    case lexer::LogicAnd:
      return true;
    default:
      break;
    }
    return false;
  }
  
  bool Parser::IsUnaryOperationToken (Lexer::TokenType tokenType)
  {
    switch (tokenType)
    {
    case lexer::Minus:
    case lexer::BitwiseInvert:
    case lexer::LogicInvert:
      return true;
    default:
      break;
    }
    return false;
  }
  
  ast::ExprPtr Parser::AstParseExpression ()
  {
    return CommonAstParseNode (
      [&]()
      {
        auto expr = AstParseExprLogicOr ();
        if (currentToken.typeOrID == lexer::Assign)
        {
          auto opToken = currentToken;
          // Assignment expression
          NextToken();
          auto assignedExpr = AstParseExpression ();
          ast::ExprBinaryPtr assignmentExpr (
            new ast::ExprBinary (std::move (expr), opToken, std::move (assignedExpr)));;
          expr.reset (new ast::Expr (std::move (assignmentExpr)));
        }
        else if (currentToken.typeOrID == lexer::TernaryIf)
        {
          expr = AstParseExprTernary (std::move (expr));
        }
        return expr;
      });
  }

  ast::ExprPtr Parser::AstParseExprBase ()
  {
    return CommonAstParseNode (
      [&]()
      {
        ast::ExprPtr expr;
        auto astValue = AstParseExprValue ();
        if (astValue)
        {
          /* Need to check if it's identifier + '('.
          * We have a function call in that case */
          if ((astValue->value.typeOrID != lexer::Identifier)
              || (currentToken.typeOrID != lexer::ParenL))
          {
            expr.reset (new ast::Expr (std::move (astValue)));
          }
          else
          {
            // Expression is a function call
            auto params = AstParseFuncParamActual ();
            ast::ExprFunctionCallPtr funcCallExpr (new ast::ExprFunctionCall (
              ast::Identifier{ astValue->value }, std::move (params)));
            expr.reset (new ast::Expr (std::move (funcCallExpr)));
          }
        }
        if (!expr)
        {
          int beyondType = 0;
          if (IsWellKnownTypeOrArray (beyondType) && (Peek (beyondType).typeOrID == lexer::ParenL))
          {
            // Expression is a type cast or ctor.
            auto astType = AstParseType ();
            if (astType.has_error ())
              throw Exception (astType.error ().error, astType.error ().token);
            auto params = AstParseFuncParamActual ();
            ast::ExprFunctionCallPtr funcCallExpr (new ast::ExprFunctionCall (
              std::move (astType.value()), std::move (params)));
            expr.reset (new ast::Expr (std::move (funcCallExpr)));
          }
          else if (currentToken.typeOrID == lexer::ParenL)
          {
            // '(' - nested expression
            NextToken();
            expr = AstParseExpression ();
            Expect (lexer::ParenR);
            NextToken ();
          }
          else
          {
            UnexpectedToken();
          }
        }
        return AstParseAttributeOrArrayAccess (std::move (expr));
      });
  }

  ast::ExprValuePtr Parser::AstParseExprValue ()
  {
    return CommonAstParseNode (
      [&]()
      {
        ast::ExprValuePtr node;
        // Variable, boolean constant, or numeric constant
        if ((currentToken.typeOrID == lexer::Identifier)
            || (currentToken.typeOrID == lexer::kwTrue) || (currentToken.typeOrID == lexer::kwFalse)
            || (currentToken.typeOrID == lexer::Numeric))
        {
          node.reset (new ast::ExprValue (currentToken));
          NextToken ();
        }
        // else: None of the above
        return node;
      });
  }

  ast::ExprPtr Parser::AstParseAttributeOrArrayAccess (ast::ExprPtr&& baseExpr)
  {
    return CommonAstParseNode (
      [&]()
      {
        ast::ExprPtr expr = std::move (baseExpr);
        while (true)
        {
          if (currentToken.typeOrID == lexer::Member)
          {
            NextToken ();
            auto attr = AstParseIdentifier ();
            auto attributeExpr = ast::ExprAttributePtr (new ast::ExprAttribute (std::move (expr), std::move (attr)));
            expr.reset (new ast::Expr (std::move (attributeExpr)));
          }
          else if (currentToken.typeOrID == lexer::BracketL)
          {
            NextToken();
            auto indexExpr = AstParseExpression ();
            Expect (lexer::BracketR);
            NextToken();
            auto arrayElementExpr = ast::ExprArrayElementPtr (new ast::ExprArrayElement (std::move (expr), std::move (indexExpr)));
            expr.reset (new ast::Expr (std::move (arrayElementExpr)));
          }
          else
            break;
        }
        return expr;
      });
  }

  ast::ExprPtr Parser::AstParseExprMultiplication ()
  {
    return CommonAstParseNode (
      [&]()
      {
        auto expr = AstParseExprUnary ();
        while ((currentToken.typeOrID == lexer::Mult)
          || (currentToken.typeOrID == lexer::Div)
          || (currentToken.typeOrID == lexer::Mod))
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = AstParseExprUnary ();
          ast::ExprBinaryPtr binExpr (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
          expr.reset (new ast::Expr (std::move (binExpr)));
        }
        return expr;
    });
  }

  ast::ExprPtr Parser::AstParseExprAddition ()
  {
    return CommonAstParseNode (
      [&]()
      {
        auto expr = AstParseExprMultiplication ();
        while ((currentToken.typeOrID == lexer::Plus)
          || (currentToken.typeOrID == lexer::Minus))
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = AstParseExprMultiplication ();
          ast::ExprBinaryPtr binExpr (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
          expr.reset (new ast::Expr (std::move (binExpr)));
        }
        return expr;
      });
  }

  ast::ExprPtr Parser::AstParseExprUnary ()
  {
    return CommonAstParseNode (
      [&]()
      {
        ast::ExprPtr expr;
        if ((currentToken.typeOrID == lexer::BitwiseInvert)
          || (currentToken.typeOrID == lexer::LogicInvert)
          || (currentToken.typeOrID == lexer::Minus))
        {
          auto opToken = currentToken;
          NextToken();
          expr = AstParseExprBase ();
          ast::ExprUnaryPtr unaryExpr (new ast::ExprUnary (opToken, std::move (expr)));
          expr.reset (new ast::Expr (std::move (unaryExpr)));
        }
        else
          expr = AstParseExprBase ();
        return expr;
      });
  }

  ast::ExprPtr Parser::AstParseExprTernary (ast::ExprPtr&& prefix)
  {
    return CommonAstParseNode (
      [&]()
      {
        auto expr (std::move (prefix));
        NextToken();
        auto expr2 = AstParseExpression ();
        Expect (lexer::TernaryElse);
        NextToken();
        auto expr3 = AstParseExpression ();
        ast::ExprTernaryPtr ternaryExpr (new ast::ExprTernary (std::move (expr), std::move (expr2), std::move (expr3)));
        return ast::ExprPtr (new ast::Expr (std::move (ternaryExpr)));
      });
  }

  ast::ExprPtr Parser::AstParseExprCompareEqual ()
  {
    return CommonAstParseNode (
      [&]()
      {
        auto expr = AstParseExprComparison ();
        while ((currentToken.typeOrID == lexer::Equals)
          || (currentToken.typeOrID == lexer::NotEquals))
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = AstParseExprComparison ();
          ast::ExprBinaryPtr binExpr (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
          expr.reset (new ast::Expr (std::move (binExpr)));
        }
        return expr;
      });
  }

  ast::ExprPtr Parser::AstParseExprComparison ()
  {
    return CommonAstParseNode (
      [&]()
      {
        auto expr = AstParseExprAddition ();
        while ((currentToken.typeOrID == lexer::Larger)
          || (currentToken.typeOrID == lexer::LargerEqual)
          || (currentToken.typeOrID == lexer::Smaller)
          || (currentToken.typeOrID == lexer::SmallerEqual))
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = AstParseExprAddition ();
          ast::ExprBinaryPtr binExpr (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
          expr.reset (new ast::Expr (std::move (binExpr)));
        }
        return expr;
      });
  }

  ast::ExprPtr Parser::AstParseExprLogicOr ()
  {
    return CommonAstParseNode (
      [&]()
      {
        auto expr = AstParseExprLogicAnd ();
        while (currentToken.typeOrID == lexer::LogicOr)
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = AstParseExprLogicAnd ();
          ast::ExprBinaryPtr binExpr (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
          expr.reset (new ast::Expr (std::move (binExpr)));
        }
        return expr;
      });
  }

  ast::ExprPtr Parser::AstParseExprLogicAnd ()
  {
    return CommonAstParseNode (
      [&]()
      {
        auto expr = AstParseExprCompareEqual ();
        while (currentToken.typeOrID == lexer::LogicAnd)
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = AstParseExprCompareEqual ();
          ast::ExprBinaryPtr binExpr (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
          expr.reset (new ast::Expr (std::move (binExpr)));
        }
        return expr;
      });
  }

  bool Parser::IsExpression ()
  {
    if ((currentToken.typeOrID == lexer::ParenL)
        || (currentToken.typeOrID == lexer::kwTrue)
        || (currentToken.typeOrID == lexer::kwFalse)
        || (currentToken.typeOrID == lexer::Numeric))
      return true;
    // Check if assignment
    if ((currentToken.typeOrID == lexer::Identifier)
          && (Peek ().typeOrID == lexer::Assign))
      return true;
    // Check if attribute
    if ((currentToken.typeOrID == lexer::Identifier)
          && (Peek ().typeOrID == lexer::Member))
      return true;
    // Check if array access
    if ((currentToken.typeOrID == lexer::Identifier)
          && (Peek ().typeOrID == lexer::BracketL))
      return true;
    // Check if 'stand-alone' expression
    if (((currentToken.typeOrID == lexer::Identifier)
            && (IsBinaryOperationToken (Peek ().typeOrID) 	// X op Y
              || (Peek ().typeOrID == lexer::TernaryIf)))	// X ? ...
          || IsUnaryOperationToken (currentToken.typeOrID)	// !X, ~X ...
          || Peek ().typeOrID == lexer::Semicolon)		// X; (expression is just identifier)
      return true;
    /* Check if function call - can be <Identifier> '('
       or <type> '(', where <type> can take up multiple tokens,
       especially for arrays */
    if ((currentToken.typeOrID == lexer::Identifier)
          && (Peek ().typeOrID == lexer::ParenL))
      return true;
    int checkForParens = 0;
    if (IsWellKnownTypeOrArray (checkForParens))
    {
      if (Peek (checkForParens).typeOrID == lexer::ParenL)
        return true;
    }
    return false;
  }
  
  Parser::Expression Parser::ParseExpression (const Scope& scope)
  {
    auto astExpr = AstParseExpression ();
    S1_ASSERT(astExpr, Parser::Expression());

    return ParseExpression (scope, *astExpr);
  }

  Parser::Expression Parser::ParseExpression (const Scope& scope, const ast::Expr& astExpr)
  {
    if (auto exprValue = boost::get<ast::ExprValuePtr> (&astExpr.value))
    {
      return ParseExprValue (scope, **exprValue);
    }
    else if (auto exprBinary = boost::get<ast::ExprBinaryPtr> (&astExpr.value))
    {
      return ParseExprBinary (scope, **exprBinary);
    }
    else if (auto exprUnary = boost::get<ast::ExprUnaryPtr> (&astExpr.value))
    {
      return ParseExprUnary (scope, **exprUnary);
    }
    else if (auto exprTernary = boost::get<ast::ExprTernaryPtr> (&astExpr.value))
    {
      return ParseExprTernary (scope, **exprTernary);
    }
    else if (auto exprFunctionCall = boost::get<ast::ExprFunctionCallPtr> (&astExpr.value))
    {
      return ParseExprFunctionCall (scope, **exprFunctionCall);
    }
    else if (auto exprAttribute = boost::get<ast::ExprAttributePtr> (&astExpr.value))
    {
      return ParseExprAttribute (scope, **exprAttribute);
    }
    else if (auto exprArrayElement = boost::get<ast::ExprArrayElementPtr> (&astExpr.value))
    {
      return ParseExprArrayElement (scope, **exprArrayElement);
    }
    // else
    S1_ASSERT_NOT_REACHED(Expression ());
  }

  Parser::Expression Parser::ParseExprValue (const Scope& scope, const ast::ExprValue& astExprValue)
  {
    const auto& token = astExprValue.value;
    if (token.typeOrID == lexer::Identifier)
    {
      auto idName = scope->ResolveIdentifier (token.tokenString);
      if (idName->GetType() != SemanticsHandler::Name::Variable)
      {
        // TODO: Report error?
      }
      return semanticsHandler.CreateVariableExpression (idName);
    }
    else if (token.typeOrID == lexer::Numeric)
    {
      return semanticsHandler.CreateConstNumericExpression (token.tokenString);
    }
    else if ((token.typeOrID == lexer::kwTrue) || (token.typeOrID == lexer::kwFalse))
    {
      return semanticsHandler.CreateConstBoolExpression ((token.typeOrID == lexer::kwTrue));
    }
    else
    {
      S1_ASSERT_NOT_REACHED(Parser::Expression ());
    }
  }

  Parser::Expression Parser::ParseExprArrayElement (const Scope& scope, const ast::ExprArrayElement& astExprArrayElement)
  {
    return semanticsHandler.CreateArrayElementAccess (ParseExpression (scope, *astExprArrayElement.value),
                                                      ParseExpression (scope, *astExprArrayElement.index));
  }

  Parser::Expression Parser::ParseExprAttribute (const Scope& scope, const ast::ExprAttribute& astExprAttribute)
  {
    return semanticsHandler.CreateAttributeAccess (ParseExpression (scope, *astExprAttribute.value),
                                                   astExprAttribute.attr.GetString());
  }

  Parser::Expression Parser::ParseExprFunctionCall (const Scope& scope, const ast::ExprFunctionCall& astExprFunctionCall)
  {
    SemanticsHandler::ExpressionVector paramExprs;
    paramExprs.reserve (astExprFunctionCall.args.size());
    for (const auto& paramAstExpr : astExprFunctionCall.args)
    {
      paramExprs.push_back (ParseExpression (scope, *paramAstExpr));
    }
    if (auto astType = boost::get<ast::TypePtr> (&astExprFunctionCall.identifierOrType))
    {
      return semanticsHandler.CreateTypeConstructorExpression (ParseType (**astType, scope), paramExprs);
    }
    else
    {
      const auto& identifier = boost::get<ast::Identifier> (astExprFunctionCall.identifierOrType).GetString();
      auto idName = scope->ResolveIdentifier (identifier);
      return semanticsHandler.CreateFunctionCallExpression (idName, paramExprs);
    }
  }

  Parser::Expression Parser::ParseExprBinary (const Scope& scope, const ast::ExprBinary& astExprBinary)
  {
    switch (astExprBinary.op.typeOrID)
    {
    case lexer::Assign:
      return semanticsHandler.CreateAssignExpression (ParseExpression (scope, *astExprBinary.left),
                                                      ParseExpression (scope, *astExprBinary.right));
    case lexer::Mult:
    case lexer::Div:
    case lexer::Mod:
    case lexer::Plus:
    case lexer::Minus:
      return ParseExprArithmetic (scope, astExprBinary);
    case lexer::Equals:
    case lexer::NotEquals:
    case lexer::Larger:
    case lexer::LargerEqual:
    case lexer::Smaller:
    case lexer::SmallerEqual:
      return ParseExprComparison (scope, astExprBinary);
    case lexer::LogicOr:
    case lexer::LogicAnd:
      return ParseExprLogic (scope, astExprBinary);
    }
    S1_ASSERT_NOT_REACHED (Parser::Expression ());
  }

  Parser::Expression Parser::ParseExprArithmetic (const Scope& scope, const parser::ast::ExprBinary& astExprBinary)
  {
    SemanticsHandler::ArithmeticOp op;
    switch (astExprBinary.op.typeOrID)
    {
    case lexer::Mult:
      op = SemanticsHandler::Mul;
      break;
    case lexer::Div:
      op = SemanticsHandler::Div;
      break;
    case lexer::Mod:
      op = SemanticsHandler::Mod;
      break;
    case lexer::Plus:
      op = SemanticsHandler::Add;
      break;
    case lexer::Minus:
      op = SemanticsHandler::Sub;
      break;
    default:
      S1_ASSERT_NOT_REACHED (Parser::Expression ());
    }
    return semanticsHandler.CreateArithmeticExpression (op,
                                                        ParseExpression (scope, *astExprBinary.left),
                                                        ParseExpression (scope, *astExprBinary.right));
  }
  
  Parser::Expression Parser::ParseExprUnary (const Scope& scope, const parser::ast::ExprUnary& astExprUnary)
  {
    SemanticsHandler::UnaryOp op;
    switch (astExprUnary.op.typeOrID)
    {
    default:
      S1_ASSERT_NOT_REACHED (Parser::Expression ());
    case lexer::BitwiseInvert:
      op = SemanticsHandler::Inv;
      break;
    case lexer::LogicInvert:
      op = SemanticsHandler::Not;
      break;
    case lexer::Minus:
      op = SemanticsHandler::Neg;
      break;
    }
    return semanticsHandler.CreateUnaryExpression (op, ParseExpression (scope, *astExprUnary.right));
  }
  
  Parser::Expression Parser::ParseExprTernary (const Scope& scope, const parser::ast::ExprTernary& astExprTernary)
  {
    return semanticsHandler.CreateTernaryExpression (ParseExpression (scope, *astExprTernary.cond),
                                                     ParseExpression (scope, *astExprTernary.trueExpr),
                                                     ParseExpression (scope, *astExprTernary.falseExpr));
  }
  
  Parser::Expression Parser::ParseExprComparison (const Scope& scope, const parser::ast::ExprBinary& astExprBinary)
  {
    SemanticsHandler::CompareOp op;
    switch (astExprBinary.op.typeOrID)
    {
    case lexer::Equals:
      op = SemanticsHandler::Equals;
      break;
    case lexer::NotEquals:
      op = SemanticsHandler::NotEquals;
      break;
    case lexer::Larger:
      op = SemanticsHandler::Larger;
      break;
    case lexer::LargerEqual:
      op = SemanticsHandler::LargerEqual;
      break;
    case lexer::Smaller:
      op = SemanticsHandler::Smaller;
      break;
    case lexer::SmallerEqual:
      op = SemanticsHandler::SmallerEqual;
      break;
    default:
      S1_ASSERT_NOT_REACHED (Parser::Expression ());
    }
    return semanticsHandler.CreateComparisonExpression (op,
                                                        ParseExpression (scope, *astExprBinary.left),
                                                        ParseExpression (scope, *astExprBinary.right));
  }
  
  Parser::Expression Parser::ParseExprLogic (const Scope& scope, const parser::ast::ExprBinary& astExprBinary)
  {
    switch (astExprBinary.op.typeOrID)
    {
    case lexer::LogicOr:
      return semanticsHandler.CreateLogicExpression (SemanticsHandler::Or,
                                                     ParseExpression (scope, *astExprBinary.left),
                                                     ParseExpression (scope, *astExprBinary.right));
    case lexer::LogicAnd:
      return semanticsHandler.CreateLogicExpression (SemanticsHandler::And,
                                                     ParseExpression (scope, *astExprBinary.left),
                                                     ParseExpression (scope, *astExprBinary.right));
    }
    S1_ASSERT_NOT_REACHED (Parser::Expression ());
  }
  
  bool Parser::IsWellKnownType (int& peekAfterType)
  {
    Lexer::TokenType tokenID = static_cast<lexer::TokenType> (currentToken.typeOrID & ~lexer::TypeFlagMask);
    peekAfterType = 0;
    if (currentToken.typeOrID == lexer::kwUnsigned)
    {
      peekAfterType++;
      tokenID = static_cast<lexer::TokenType> (Peek().typeOrID & ~lexer::TypeFlagMask);
    }
    bool isType = false;
    switch (tokenID)
    {
    case lexer::kwBool:
    case lexer::kwInt:
    case lexer::kwFloat:
    case lexer::kwSampler1D:
    case lexer::kwSampler2D:
    case lexer::kwSampler3D:
    case lexer::kwSamplerCUBE:
      isType = true;
      break;
    default:
      break;
    }
    return isType;
  }

  bool Parser::IsWellKnownTypeOrArray (int& peekAfterType)
  {
    if (IsWellKnownType (peekAfterType))
    {
      while ((Peek (peekAfterType).typeOrID == lexer::BracketL)
        && (Peek (peekAfterType+1).typeOrID == lexer::BracketR))
      {
        peekAfterType += 2;
      }
      return true;
    }
    return false;
  }

  bool Parser::IsType (const Scope& scope, int& peekAfterType)
  {
    Lexer::TokenType tokenID = static_cast<lexer::TokenType> (currentToken.typeOrID & ~lexer::TypeFlagMask);
    peekAfterType = 0;
    bool isType = false;
    if (currentToken.typeOrID == lexer::Identifier)
    {
      /* Might be a type alias */
      Name typeName = scope->ResolveIdentifier (currentToken.tokenString);
      isType = (typeName->GetType() == SemanticsHandler::Name::TypeAlias);
    }
    else
    {
      int peekWellKnown;
      if ((isType = IsWellKnownType (peekWellKnown)))
        peekAfterType += peekWellKnown;
    }
    if (isType)
    {
      while ((Peek (peekAfterType).typeOrID == lexer::BracketL)
        && (Peek (peekAfterType+1).typeOrID == lexer::BracketR))
      {
        peekAfterType += 2;
      }
    }
    return isType;
  }

  Parser::AstParseTypeResult Parser::AstParseType ()
  {
    return CommonAstParseNode (
      [&]() -> Parser::AstParseTypeResult
      {
        ast::TypePtr type;
        if (currentToken.typeOrID == lexer::Identifier)
        {
          // Assume type alias
          type.reset (new ast::Type (ast::Identifier{ currentToken }));
          NextToken ();
        }
        else
        {
          int wellKnownExtra;
          if (IsWellKnownType (wellKnownExtra))
          {
            ast::Type::WellKnownType wellKnownTokens;
            for (int i = 0; i <= wellKnownExtra; i++)
            {
              wellKnownTokens.push_back (currentToken);
              NextToken ();
            }
            type.reset (new ast::Type (std::move (wellKnownTokens)));
          }
          else
          {
            return ParseError{ parser::Error::ExpectedTypeName, currentToken };
          }
        }
        while ((currentToken.typeOrID == lexer::BracketL)
          && (Peek (0).typeOrID == lexer::BracketR))
        {
          type.reset (new ast::Type (ast::Type::ArrayType{ std::move (type) }));
          NextToken ();
          NextToken ();
        }
        return type;
      });
  }

  Parser::Type Parser::ParseTypeBase (parser::ast::Type& astType, const Scope& scope)
  {
    if (auto typeIdentifier = boost::get<ast::Identifier> (&astType.value))
    {
      Name typeName = scope->ResolveIdentifier (typeIdentifier->GetString());
      Type type;
      if (typeName->GetType() == SemanticsHandler::Name::TypeAlias)
      {
        return typeName->GetAliasedType();
      }
      else
      {
        throw Exception (parser::Error::ExpectedTypeName, typeIdentifier->token);
      }
    }
    auto typeTokens = boost::get<ast::Type::WellKnownType> (&astType.value);
    S1_ASSERT(typeTokens, Type ());

    auto currentTypeToken = (*typeTokens).at (0);
    bool isUnsigned = false;
    auto baseToken = static_cast<lexer::TokenType> (currentTypeToken.typeOrID & ~lexer::TypeFlagMask);
    if (baseToken == lexer::kwUnsigned)
    {
      isUnsigned = true;
      currentTypeToken = (*typeTokens).at (1);
      baseToken = static_cast<lexer::TokenType> (currentTypeToken.typeOrID & ~lexer::TypeFlagMask);
    }
    switch (baseToken)
    {
    case lexer::kwBool:
    case lexer::kwInt:
    case lexer::kwFloat:
      {
        switch (static_cast<lexer::TokenType> (currentTypeToken.typeOrID & lexer::TypeFlagMask))
        {
        case 0:
          if (baseToken == lexer::kwBool)
            return ParseTypeBool (currentTypeToken);
          else
            return ParseTypeNumeric (isUnsigned, currentTypeToken);
        case lexer::VecFlag:
          return ParseTypeVector (isUnsigned, currentTypeToken);
        case lexer::MatFlag:
          return ParseTypeMatrix (isUnsigned, currentTypeToken);
        default:
          break;
        }
      }
      break;
    case lexer::kwSampler1D:
    case lexer::kwSampler2D:
    case lexer::kwSampler3D:
    case lexer::kwSamplerCUBE:
      return ParseTypeSampler (currentTypeToken);
    }
    S1_ASSERT_NOT_REACHED_MSG("unhandled type", Type ());
    return Type ();
  }
  
  Parser::Type Parser::ParseType (const Scope& scope)
  {
    auto astType = AstParseType ();
    if (astType.has_error ())
      throw Exception (astType.error ().error, astType.error ().token);
    S1_ASSERT(astType.value(), Type ());
    return ParseType (*astType.value(), scope);
  }

  Parser::Type Parser::ParseType (ast::Type& astType, const Scope& scope)
  {
    if (auto arrayType = boost::get<ast::Type::ArrayType> (&astType.value))
    {
      auto type = ParseType (*arrayType->containedType, scope);
      return semanticsHandler.CreateArrayType (type);
    }
    else
    {
      return ParseTypeBase (astType, scope);
    }
  }
  
  Parser::Type Parser::ParseTypeBool (const Lexer::Token& /*token*/)
  {
    return semanticsHandler.CreateType (SemanticsHandler::Bool);
  }
  
  Parser::Type Parser::ParseTypeNumeric (bool isUnsigned, const Lexer::Token& token)
  {
    Type type;
    switch (token.typeOrID)
    {
    case lexer::kwInt:
      /* int */
      type = semanticsHandler.CreateType (
        isUnsigned ? SemanticsHandler::UInt : SemanticsHandler::Int);
      break;
    case lexer::kwFloat:
      /* float */
      type = semanticsHandler.CreateType (SemanticsHandler::Float);
      // TODO: Err if unsigned
      break;
    default:
      S1_ASSERT_NOT_REACHED(Type());
    }
    return type;
  }
  
  Parser::Type Parser::ParseTypeVector (bool isUnsigned, const Lexer::Token& token)
  {
    Type baseType;
    switch (static_cast<lexer::TokenType> (token.typeOrID & ~lexer::TypeFlagMask))
    {
    case lexer::kwBool:
      baseType = semanticsHandler.CreateType (SemanticsHandler::Bool);
      break;
    case lexer::kwInt:
      /* int */
      baseType = semanticsHandler.CreateType (
        isUnsigned ? SemanticsHandler::UInt : SemanticsHandler::Int);
      break;
    case lexer::kwFloat:
      /* float */
      baseType = semanticsHandler.CreateType (SemanticsHandler::Float);
      break;
    default:
      S1_ASSERT_NOT_REACHED(Type());
    }
    Type vecType = semanticsHandler.CreateVectorType (baseType, token.dimension1);
    return vecType;
  }
  
  Parser::Type Parser::ParseTypeMatrix (bool isUnsigned, const Lexer::Token& token)
  {
    Type baseType;
    switch (static_cast<lexer::TokenType> (token.typeOrID & ~lexer::TypeFlagMask))
    {
    case lexer::kwBool:
      baseType = semanticsHandler.CreateType (SemanticsHandler::Bool);
      break;
    case lexer::kwInt:
      /* int */
      baseType = semanticsHandler.CreateType (
        isUnsigned ? SemanticsHandler::UInt : SemanticsHandler::Int);
      break;
    case lexer::kwFloat:
      /* float */
      baseType = semanticsHandler.CreateType (SemanticsHandler::Float);
      break;
    default:
      S1_ASSERT_NOT_REACHED(Type());
    }
    Type matType = semanticsHandler.CreateMatrixType (baseType, token.dimension1, token.dimension2);
    return matType;
  }
  
  Parser::Type Parser::ParseTypeSampler (const Lexer::Token& token)
  {
    Type type;
    switch (token.typeOrID)
    {
    case lexer::kwSampler1D:
      /* 1D sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::_1D);
      break;
    case lexer::kwSampler2D:
      /* 2D sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::_2D);
      break;
    case lexer::kwSampler3D:
      /* 3D sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::_3D);
      break;
    case lexer::kwSamplerCUBE:
      /* CUBE sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::CUBE);
      break;
    default:
      S1_ASSERT_NOT_REACHED(Type());
    }
    return type;
  }

  ast::TypedefPtr Parser::AstParseTypedef ()
  {
    return CommonAstParseNode (
      [&]()
      {
        // Skip typedef
        NextToken();
        // Aliased type
        auto astType = AstParseType ();
        if (astType.has_error ())
          throw Exception (astType.error ().error, astType.error ().token);
        // Type alias
        auto aliasIdentifier = AstParseIdentifier ();
        return ast::TypedefPtr (new ast::Typedef (std::move (astType.value()), std::move (aliasIdentifier)));
      });
  }

  void Parser::ParseTypedef (const Scope& scope)
  {
    auto astTypedef = AstParseTypedef ();
    ParseTypedef (scope, *astTypedef);
  }

  void Parser::ParseTypedef (const Scope& scope, const parser::ast::Typedef& astTypedef)
  {
    Type aliasedType = ParseType (*astTypedef.type, scope);
    // Add to scope
    scope->AddTypeAlias (aliasedType, astTypedef.alias.GetString());
  }

  void Parser::ParseFuncDeclare (const Scope& scope)
  {
    auto astFunctionDecl = AstParseFunctionDecl ();
    ParseFuncDeclare (scope, *astFunctionDecl);
  }

  void Parser::ParseFuncDeclare (const Scope& scope, const ast::FunctionDecl& astFunctionDecl)
  {
    Type returnType;
    if (auto voidResult = boost::get<ast::FunctionDecl::Void> (&astFunctionDecl.resultType))
    {
      returnType = semanticsHandler.CreateType (SemanticsHandler::Void);
    }
    else
    {
      const auto& astType = boost::get<ast::TypePtr> (astFunctionDecl.resultType);
      returnType = ParseType (*astType, scope);
    }
    uc::String functionIdentifier = astFunctionDecl.identifier.GetString();
    // Parse formal parameters
    SemanticsHandler::Scope::FunctionFormalParameters params;
    ParseFuncParamFormal (scope, params, astFunctionDecl);
    // Add function to scope, get block
    Function func (scope->AddFunction (returnType, functionIdentifier, params));
    Block inFunctionBlock = func->GetBody();;
    // Handle expressions in block
    ParseBlock (inFunctionBlock, *astFunctionDecl.body);
    func->Finish ();
  }

  ast::FunctionDeclPtr Parser::AstParseFunctionDecl ()
  {
    return CommonAstParseNode (
      [&]()
      {
        // Parse return type
        ast::FunctionDecl::VoidOrType returnType;
        if (currentToken.typeOrID == lexer::kwVoid)
        {
          returnType = ast::FunctionDecl::Void ();
          NextToken ();
        }
        else
        {
          auto astType = AstParseType ();
          if (astType.has_error ())
            throw Exception (astType.error ().error, astType.error ().token);
          returnType = std::move (astType.value());
        }
        // Parse function identifier
        auto funcIdentifier = AstParseIdentifier ();
        // Parse formal parameters
        Expect (lexer::ParenL);
        NextToken ();
        ast::FunctionDecl::ParamsContainer params;
        while (true)
        {
          if (currentToken.typeOrID == lexer::ParenR)
          {
            // End of list
            NextToken ();
            break;
          }

          // TODO: In case of error, skip to next param
          ast::FunctionDecl::Param param;
          while ((currentToken.typeOrID == lexer::kwIn)
            || (currentToken.typeOrID == lexer::kwOut)
            || (currentToken.typeOrID == lexer::kwUniform)
            || (currentToken.typeOrID == lexer::kwAttribute))
          {
            param.qualifiers.push_back (currentToken);
            NextToken ();
          }
          {
            auto astType = AstParseType ();
            if (astType.has_error ())
              throw Exception (astType.error ().error, astType.error ().token);
            param.type = std::move (astType.value ());
          }
          param.identifier = AstParseIdentifier ();
          if (currentToken.typeOrID == lexer::Assign)
          {
            // Handle default value
            NextToken ();
            param.defaultValue = AstParseExpression ();
          }
          params.emplace_back (std::move (param));
          if (currentToken.typeOrID == lexer::Separator)
            NextToken ();
          else if (currentToken.typeOrID != lexer::ParenR)
            UnexpectedToken ();
        }
        // Parse function body
        Expect (lexer::BraceL);
        NextToken();
        auto body = AstParseBlock ();
        Expect (lexer::BraceR);
        NextToken();
        return ast::FunctionDeclPtr (new ast::FunctionDecl (std::move (returnType),
                                                            std::move (funcIdentifier),
                                                            std::move (params),
                                                            std::move (body)));
      });
  }

  void Parser::ParseFuncParamFormal (const Scope& scope,
                                     parser::SemanticsHandler::Scope::FunctionFormalParameters& params,
                                     const parser::ast::FunctionDecl& astFunctionDecl)
  {
    params.reserve (astFunctionDecl.params.size());
    for (const auto& param : astFunctionDecl.params)
    {
      int paramDirection = 0;
      SemanticsHandler::Scope::FormalParameterFrequency freqQualifier = SemanticsHandler::Scope::freqAuto;
      int numFreqQualifiers = 0;
      for (const auto& qualifierToken : param.qualifiers)
      {
        if (qualifierToken.typeOrID == lexer::kwIn)
        {
          // 'in' parameter
          if ((paramDirection & SemanticsHandler::Scope::dirIn) != 0)
          {
            // TODO: Warn about redundancy
          }
          paramDirection |= SemanticsHandler::Scope::dirIn;
        }
        else if (qualifierToken.typeOrID == lexer::kwOut)
        {
          // 'out' parameter
          if ((paramDirection & SemanticsHandler::Scope::dirOut) != 0)
          {
            // TODO: Warn about redundancy
          }
          paramDirection |= SemanticsHandler::Scope::dirOut;
        }
        else if (qualifierToken.typeOrID == lexer::kwUniform)
        {
          // 'uniform' qualifier
          freqQualifier = SemanticsHandler::Scope::freqUniform;
          numFreqQualifiers++;
        }
        else if (qualifierToken.typeOrID == lexer::kwAttribute)
        {
          // 'attribute' qualifier
          freqQualifier = SemanticsHandler::Scope::freqAttribute;
          numFreqQualifiers++;
        }
        else
        {
          S1_ASSERT_NOT_REACHED_MSG("unhandled qualifier token", S1_ASSERT_RET_VOID);
        }
      }
      // If no explicit direction is given, use 'in'
      if (paramDirection == 0) paramDirection = SemanticsHandler::Scope::dirIn;
      SemanticsHandler::Scope::FunctionFormalParameter newParam;
      newParam.dir = (SemanticsHandler::Scope::FormalParameterDirection)paramDirection;
      newParam.freqQualifier = freqQualifier;
      newParam.type = ParseType (*param.type, scope);
      newParam.identifier = param.identifier.GetString();
      if (param.defaultValue)
      {
        // Handle default value
        if (paramDirection == SemanticsHandler::Scope::dirOut)
          throw Exception (Error::OutParameterWithDefault);
        newParam.defaultValue = ParseExpression (scope, *param.defaultValue);
      }
      if ((((paramDirection & SemanticsHandler::Scope::dirIn) == 0)
          || ((paramDirection & SemanticsHandler::Scope::dirOut) != 0))
        && (numFreqQualifiers > 0))
      {
        throw Exception (Error::QualifiersNotAllowed);
      }
      else if (numFreqQualifiers > 1)
      {
        throw Exception (Error::ConflictingQualifiersForInParam);
      }
      params.push_back (std::move (newParam));
    }
  }
  
  //void ParseFuncCall ();

  std::vector<ast::ExprPtr> Parser::AstParseFuncParamActual ()
  {
    std::vector<ast::ExprPtr> params;
    // Skip '('
    NextToken();
    while (true)
    {
      if (currentToken.typeOrID == lexer::ParenR)
      {
        // End of list
        NextToken ();
        break;
      }

      auto expr = AstParseExpression ();
      params.emplace_back (std::move (expr));

      if (currentToken.typeOrID == lexer::Separator)
        NextToken ();
      else if (currentToken.typeOrID != lexer::ParenR)
        UnexpectedToken ();
    }
    return params;
  }

  ast::VarsDeclPtr Parser::AstParseVarsDecl (bool isConst)
  {
    return CommonAstParseNode (
      [&]()
      {
        // Helper to check for a token and to also consume it
        auto haveSeparator =
          [&]()
          {
            if (currentToken.typeOrID == lexer::Separator)
            {
              NextToken ();
              return true;
            }
            else
              return false;
          };

        auto astType = AstParseType ();
        if (astType.has_error ())
          throw Exception (astType.error ().error, astType.error ().token);
        ast::VarsDecl::VarsContainer vars;
        do
        {
          if (currentToken.typeOrID != lexer::Identifier)
          {
            diagnosticsHandler.ParseError (Error::UnexpectedToken, currentToken, lexer::Identifier);
            break;
          }
          auto identifier = ast::Identifier{ currentToken };
          NextToken ();
          ast::ExprPtr initializer;
          if (currentToken.typeOrID == lexer::Assign)
          {
            // Variable has an initializer value
            NextToken ();
            initializer = AstParseExpression ();
          }
          else if (isConst)
          {
            // TODO: Give it's own error
            diagnosticsHandler.ParseError (Error::UnexpectedToken, currentToken, lexer::Assign);
            break;
          }
          vars.emplace_back (ast::VarsDecl::Var{ identifier, std::move (initializer) });
          // ',' - more variables follow
        } while (haveSeparator ());
        return ast::VarsDeclPtr (new ast::VarsDecl (isConst, std::move (astType.value()), std::move (vars)));
      });
  }

  void Parser::ParseVarDeclare (const Scope& scope)
  {
    auto astVarsDecl = AstParseVarsDecl ();
    ParseVarDeclare (scope, *astVarsDecl);
  }

  void Parser::ParseVarDeclare (const Scope& scope, const parser::ast::VarsDecl& astVarsDecl)
  {
    auto type = ParseType (*astVarsDecl.type, scope);
    for (const auto& astVar : astVarsDecl.vars)
    {
      auto varIdentifier = astVar.identifier.GetString();
      Expression initExpr;
      if (astVar.initializer)
        initExpr = ParseExpression (scope, *astVar.initializer);
      scope->AddVariable (type, varIdentifier, initExpr, astVarsDecl.isConst);
    }
  }

  void Parser::ParseConstDeclare (const Scope& scope)
  {
    NextToken(); // skip 'const'
    auto astConstsDecl = AstParseVarsDecl (true);
    ParseVarDeclare (scope, *astConstsDecl);
  }

  ast::BlockStatementForPtr Parser::AstParseFor ()
  {
    return CommonAstParseNode (
      [&]()
      {
        NextToken();
        Expect (lexer::ParenL);
        NextToken();
        ast::ExprPtr initExpr;
        if (currentToken.typeOrID != lexer::Semicolon)
          initExpr = AstParseExpression ();
        Expect (lexer::Semicolon);
        NextToken();
        ast::ExprPtr loopTestExpr;
        if (currentToken.typeOrID != lexer::Semicolon)
          loopTestExpr = AstParseExpression ();
        Expect (lexer::Semicolon);
        NextToken();
        ast::ExprPtr loopFootExpr;
        if (currentToken.typeOrID != lexer::Semicolon)
          loopFootExpr = AstParseExpression ();
        Expect (lexer::ParenR);
        NextToken();
        Expect (lexer::BraceL);
        NextToken();
        auto body = AstParseBlock ();
        Expect (lexer::BraceR);
        NextToken();
        return ast::BlockStatementForPtr (new ast::BlockStatementFor (std::move (initExpr),
                                                                      std::move (loopTestExpr),
                                                                      std::move (loopFootExpr),
                                                                      std::move (body)));
      });
  }

  ast::BlockStatementIfPtr Parser::AstParseIf ()
  {
    return CommonAstParseNode (
      [&]()
      {
        // Skip 'if'
        NextToken();
        Expect (lexer::ParenL);
        NextToken();
        // Parse condition
        auto conditionExpr = AstParseExpression ();
        Expect (lexer::ParenR);
        NextToken();
        Expect (lexer::BraceL);
        NextToken();
        // Parse 'if' block
        auto ifBlock = AstParseBlock ();
        Expect (lexer::BraceR);
        NextToken();
        ast::BlockPtr elseBlock;
        if (currentToken.typeOrID == lexer::kwElse)
        {
          NextToken();
          Expect (lexer::BraceL);
          NextToken();
          // Parse 'else' block
          elseBlock = AstParseBlock ();
          Expect (lexer::BraceR);
          NextToken();
        }
        return ast::BlockStatementIfPtr (new ast::BlockStatementIf (std::move (conditionExpr),
                                                                    std::move (ifBlock),
                                                                    std::move (elseBlock)));
      });
  }

  ast::BlockStatementWhilePtr Parser::AstParseWhile ()
  {
    return CommonAstParseNode (
      [&]()
      {
        NextToken();
        Expect (lexer::ParenL);
        NextToken();
        auto loopTestExpr = AstParseExpression ();
        Expect (lexer::ParenR);
        NextToken();
        Expect (lexer::BraceL);
        NextToken();
        auto body = AstParseBlock ();
        Expect (lexer::BraceR);
        NextToken();
        return ast::BlockStatementWhilePtr (new ast::BlockStatementWhile (std::move (loopTestExpr),
                                                                          std::move (body)));
      });
  }
} // namespace s1
