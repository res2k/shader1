/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


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

#include "parser/ast/Expr.h"
#include "parser/ast/ExprValue.h"
#include "parser/ast/Identifier.h"
#include "parser/ast/Type.h"
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
    ParseProgramStatements (globalScope);
    Expect (lexer::EndOfFile);
  }
  
  void Parser::ParseProgramStatements (const Scope& scope)
  {
    while (true)
    {
      int beyondType;
      bool isType = IsType (scope, beyondType);
      if (currentToken.typeOrID == lexer::kwConst)
      {
        /* constant declaration */
        ParseConstDeclare (scope);
        Expect (lexer::Semicolon);
        NextToken();
      }
      else if ((isType
          && (Peek (beyondType).typeOrID == lexer::Identifier))
        || (currentToken.typeOrID == lexer::kwVoid))
      {
        if ((currentToken.typeOrID == lexer::kwVoid)
            || (Peek (beyondType+1).typeOrID == lexer::ParenL))
        {
          /* Function declaration */
          ParseFuncDeclare (scope);
        }
        else
        {
          /* Variable declaration */
          ParseVarDeclare (scope);
          Expect (lexer::Semicolon);
          NextToken();
        }
      }
      else if (currentToken.typeOrID == lexer::kwTypedef)
      {
        /* Type definition */
        ParseTypedef (scope);
        Expect (lexer::Semicolon);
        NextToken();
      }
      else
        /* Unknown token - let ParseProgram throw error */
        break;
    }
  }
  
  void Parser::ParseBlock (Block block)
  {
    while (true)
    {
      try
      {
        int beyondType;
        Scope blockScope = block->GetInnerScope();
        bool isType = IsType (blockScope, beyondType);
        if (currentToken.typeOrID == lexer::kwConst)
        {
          /* constant declaration */
          ParseConstDeclare (blockScope);
          Expect (lexer::Semicolon);
          NextToken();
        }
        else if (isType && (Peek (beyondType).typeOrID == lexer::Identifier))
        {
          /* Variable declaration */
          ParseVarDeclare (blockScope);
          Expect (lexer::Semicolon);
          NextToken();
        }
        else if (currentToken.typeOrID == lexer::kwTypedef)
        {
          /* Type definition */
          ParseTypedef (blockScope);
          Expect (lexer::Semicolon);
          NextToken();
        }
        else if (IsCommand (blockScope))
        {
          ParseCommand (block);
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
  }
  
  bool Parser::IsCommand (const Scope& scope)
  {
    return IsExpression (scope)
      || (currentToken.typeOrID == lexer::kwReturn)
      || (currentToken.typeOrID == lexer::kwIf)
      || (currentToken.typeOrID == lexer::kwWhile)
      || (currentToken.typeOrID == lexer::kwFor)
      || (currentToken.typeOrID == lexer::BraceL);
  }
  
  void Parser::ParseCommand (Block block)
  {
    Scope blockScope = block->GetInnerScope();
    if (IsExpression (blockScope))
    {
      Expression expr = ParseExpression (blockScope);
      Expect (lexer::Semicolon);
      NextToken();
      block->AddExpressionCommand (expr);
    }
    else if (currentToken.typeOrID == lexer::kwReturn)
    {
      /* 'return' instruction */
      NextToken();
      Expression returnExpr;
      if (IsExpression (blockScope))
      {
        /* Return with some value */
        returnExpr = ParseExpression (blockScope);
      }
      block->AddReturnCommand (returnExpr);
      Expect (lexer::Semicolon);
      NextToken();
    }
    else if (currentToken.typeOrID == lexer::kwIf)
    {
      /* 'if' */
      ParseIf (block);
    }
    else if (currentToken.typeOrID == lexer::kwWhile)
    {
      /* 'while' */
      ParseLoopWhile (block);
    }
    else if (currentToken.typeOrID == lexer::kwFor)
    {
      /* 'for' */
      ParseLoopFor (block);
    }
    else if (currentToken.typeOrID == lexer::BraceL)
    {
      /* nested block */
      NextToken ();
      Block newBlock = semanticsHandler.CreateBlock (blockScope);
      ParseBlock (newBlock);
      Expect (lexer::BraceR);
      NextToken ();
      block->AddNestedBlock (newBlock);
    }
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

  bool Parser::IsExpression (const Scope& scope)
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
    if (IsType (scope, checkForParens))
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
  
  void Parser::ParseTypedef (const Scope& scope)
  {
    // Skip typedef
    NextToken();
    Type aliasedType = ParseType (scope);
    // Add to scope
    Expect (lexer::Identifier);
    scope->AddTypeAlias (aliasedType, currentToken.tokenString);
    NextToken();
  }

  void Parser::ParseFuncDeclare (const Scope& scope)
  {
    // Parse return type
    Type returnType = ParseFuncType (scope);
    // Parse function identifier
    Expect (lexer::Identifier);
    uc::String functionIdentifier (currentToken.tokenString);
    NextToken ();
    // Parse formal parameters
    Expect (lexer::ParenL);
    SemanticsHandler::Scope::FunctionFormalParameters params;
    ParseFuncParamFormal (scope, params);
    // Add function to scope, get block
    Function func (scope->AddFunction (returnType, functionIdentifier, params));
    Block inFunctionBlock = func->GetBody();;
    Expect (lexer::BraceL);
    NextToken();
    // Handle expressions in block
    ParseBlock (inFunctionBlock);
    func->Finish ();
    Expect (lexer::BraceR);
    NextToken();
  }
  
  Parser::Type Parser::ParseFuncType (const Scope& scope)
  {
    if (currentToken.typeOrID == lexer::kwVoid)
    {
      Parser::Type type = semanticsHandler.CreateType (SemanticsHandler::Void);
      NextToken ();
      return type;
    }
    else if (IsType (scope))
    {
      return ParseType (scope);
    }
    else
      UnexpectedToken();
    return Parser::Type ();
  }
  
  void Parser::ParseFuncParamFormal (const Scope& scope, SemanticsHandler::Scope::FunctionFormalParameters& params)
  {
    // Skip '('
    Expect (lexer::ParenL);
    NextToken ();
    while (true)
    {
      if (currentToken.typeOrID == lexer::ParenR)
      {
        // End of list
        NextToken ();
        break;
      }

      // TODO: In case of error, skip to next param
      int paramDirection = 0;
      SemanticsHandler::Scope::FormalParameterFrequency freqQualifier = SemanticsHandler::Scope::freqAuto;
      int numFreqQualifiers = 0;
      bool parseQualifiers = true;
      while (parseQualifiers)
      {
        if (currentToken.typeOrID == lexer::kwIn)
        {
          // 'in' parameter
          NextToken();
          if ((paramDirection & SemanticsHandler::Scope::dirIn) != 0)
          {
            // TODO: Warn about redundancy
          }
          paramDirection |= SemanticsHandler::Scope::dirIn;
        }
        else if (currentToken.typeOrID == lexer::kwOut)
        {
          // 'out' parameter
          NextToken();
          if ((paramDirection & SemanticsHandler::Scope::dirOut) != 0)
          {
            // TODO: Warn about redundancy
          }
          paramDirection |= SemanticsHandler::Scope::dirOut;
        }
        else if (currentToken.typeOrID == lexer::kwUniform)
        {
          // 'uniform' qualifier
          NextToken();
          freqQualifier = SemanticsHandler::Scope::freqUniform;
          numFreqQualifiers++;
        }
        else if (currentToken.typeOrID == lexer::kwAttribute)
        {
          // 'attribute' qualifier
          NextToken();
          freqQualifier = SemanticsHandler::Scope::freqAttribute;
          numFreqQualifiers++;
        }
        else
        {
          parseQualifiers = false;
        }
      }
      // If no explicit direction is given, use 'in'
      if (paramDirection == 0) paramDirection = SemanticsHandler::Scope::dirIn;
      SemanticsHandler::Scope::FunctionFormalParameter newParam;
      newParam.dir = (SemanticsHandler::Scope::FormalParameterDirection)paramDirection;
      newParam.freqQualifier = freqQualifier;
      newParam.type = ParseType (scope);
      Expect (lexer::Identifier);
      newParam.identifier = currentToken.tokenString;
      NextToken ();
      if (currentToken.typeOrID == lexer::Assign)
      {
        // Handle default value
        NextToken ();
        if (paramDirection == SemanticsHandler::Scope::dirOut)
          throw Exception (Error::OutParameterWithDefault);
        newParam.defaultValue = ParseExpression (scope);
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
      params.push_back (newParam);
      if (currentToken.typeOrID == lexer::Separator)
        NextToken ();
      else if (currentToken.typeOrID != lexer::ParenR)
        UnexpectedToken ();
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

  ast::VarsDeclPtr Parser::AstParseVarsDecl ()
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
          vars.emplace_back (ast::VarsDecl::Var{ identifier, std::move (initializer) });
          // ',' - more variables follow
        } while (haveSeparator ());
        return ast::VarsDeclPtr (new ast::VarsDecl (std::move (astType.value()), std::move (vars)));
      });
  }

  void Parser::ParseVarDeclare (const Scope& scope)
  {
    auto astVarsDecl = AstParseVarsDecl ();
    auto type = ParseType (*astVarsDecl->type, scope);
    for (const auto& astVar : astVarsDecl->vars)
    {
      auto varIdentifier = astVar.identifier.GetString();
      Expression initExpr;
      if (astVar.initializer)
        initExpr = ParseExpression (scope, *astVar.initializer);
      scope->AddVariable (type, varIdentifier, initExpr, false);
    }
  }

  void Parser::ParseConstDeclare (const Scope& scope)
  {
    NextToken(); // skip 'const'
    Type type = ParseType (scope);
    ParseConstIdentifierAndInitializerList (scope, type);
  }
  
  void Parser::ParseConstIdentifierAndInitializerList (const Scope& scope, Type type)
  {
    do
    {
      ParseConstIdentifierAndInitializer (scope, type);
      if (currentToken.typeOrID == lexer::Separator)
      {
        // ',' - more constants follow
        NextToken();
        continue;
      }
    }
    while (false);
  }
  
  void Parser::ParseConstIdentifierAndInitializer (const Scope& scope, Type type)
  {
    Expect (lexer::Identifier);
    uc::String varIdentifier = currentToken.tokenString;
    NextToken ();
    // initializer value is required
    Expect (lexer::Assign);
    NextToken ();
    Expression initExpr = ParseExpression (scope);
    scope->AddVariable (type, varIdentifier, initExpr, true);
  }

  void Parser::ParseIf (Block block)
  {
    // Skip 'if'
    NextToken();
    Expect (lexer::ParenL);
    NextToken();
    // Parse condition
    Expression conditionExpr = ParseExpression (block->GetInnerScope());
    Expect (lexer::ParenR);
    NextToken();
    Expect (lexer::BraceL);
    NextToken();
    // Create 'if' block ...
    Block ifBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
    // ... and parse
    ParseBlock (ifBlock);
    Expect (lexer::BraceR);
    NextToken();
    Block elseBlock;
    if (currentToken.typeOrID == lexer::kwElse)
    {
      NextToken();
      Expect (lexer::BraceL);
      NextToken();
      // Create 'else' block ...
      elseBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
      // ... and parse
      ParseBlock (elseBlock);
      Expect (lexer::BraceR);
      NextToken();
    }
    // Add instructions to surrounding block
    block->AddBranching (conditionExpr, ifBlock, elseBlock);
  }

  void Parser::ParseLoopFor (Block block)
  {
    NextToken();
    Expect (lexer::ParenL);
    NextToken();
    Expression initExpr;
    if (currentToken.typeOrID != lexer::Semicolon)
      initExpr = ParseExpression (block->GetInnerScope());
    Expect (lexer::Semicolon);
    NextToken();
    Expression loopTestExpr;
    if (currentToken.typeOrID != lexer::Semicolon)
      loopTestExpr = ParseExpression (block->GetInnerScope());
    Expect (lexer::Semicolon);
    NextToken();
    Expression loopFootExpr;
    if (currentToken.typeOrID != lexer::Semicolon)
      loopFootExpr = ParseExpression (block->GetInnerScope());
    Expect (lexer::ParenR);
    NextToken();
    Expect (lexer::BraceL);
    NextToken();
    Block newBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
    ParseBlock (newBlock);
    Expect (lexer::BraceR);
    NextToken();
    block->AddForLoop (initExpr, loopTestExpr, loopFootExpr, newBlock);
  }
  
  void Parser::ParseLoopWhile (Block block)
  {
    NextToken();
    Expect (lexer::ParenL);
    NextToken();
    Expression loopTestExpr = ParseExpression (block->GetInnerScope());
    Expect (lexer::ParenR);
    NextToken();
    Expect (lexer::BraceL);
    NextToken();
    Block newBlock = semanticsHandler.CreateBlock (block->GetInnerScope());
    ParseBlock (newBlock);
    Expect (lexer::BraceR);
    NextToken();
    block->AddWhileLoop (loopTestExpr, newBlock);
  }
} // namespace s1
