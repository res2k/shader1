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

#include "parser/AstBuilder.h"

#include "diagnostics/Handler.h"
#include "parser/ast/Block.h"
#include "parser/ast/BlockStatement.h"
#include "parser/ast/BlockStatementExpr.h"
#include "parser/ast/BlockStatementFor.h"
#include "parser/ast/BlockStatementIf.h"
#include "parser/ast/BlockStatementNestedBlock.h"
#include "parser/ast/BlockStatementReturn.h"
#include "parser/ast/BlockStatementTypedef.h"
#include "parser/ast/BlockStatementVarsDecl.h"
#include "parser/ast/BlockStatementWhile.h"
#include "parser/ast/Expr.h"
#include "parser/ast/ExprArrayElement.h"
#include "parser/ast/ExprAttribute.h"
#include "parser/ast/ExprBinary.h"
#include "parser/ast/ExprFunctionCall.h"
#include "parser/ast/ExprUnary.h"
#include "parser/ast/ExprTernary.h"
#include "parser/ast/ExprValue.h"
#include "parser/ast/FunctionDecl.h"
#include "parser/ast/Identifier.h"
#include "parser/ast/Program.h"
#include "parser/ast/ProgramStatementFunctionDecl.h"
#include "parser/ast/ProgramStatementTypedef.h"
#include "parser/ast/ProgramStatementVarsDecl.h"
#include "parser/ast/Type.h"
#include "parser/ast/TypeArray.h"
#include "parser/ast/Typedef.h"
#include "parser/ast/TypeIdentifier.h"
#include "parser/ast/TypeWellKnown.h"
#include "parser/ast/VarsDecl.h"
#include "parser/Diagnostics.h"
#include "parser/Exception.h"

namespace s1
{
  using namespace parser;

  AstBuilder::AstBuilder (Lexer& inputLexer, diagnostics::Handler& diagnosticsHandler)
   : inputLexer (inputLexer), diagnosticsHandler (diagnosticsHandler)
  {
    NextToken ();
  }

  void AstBuilder::NextToken ()
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

  const Lexer::Token& AstBuilder::Peek (size_t lookahead)
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
  typename std::result_of<F()>::type AstBuilder::CommonParseNode (F func)
  {
    auto startLocation = currentToken.location;
    auto result = func ();
    SetParseResultLocation (result, std::move (startLocation), lexer::TokenLocation (previousToken.location));
    return result;
  }

  ast::Identifier AstBuilder::ParseIdentifier ()
  {
    Expect (lexer::Identifier);
    ast::Identifier astIdent = ast::Identifier{ currentToken };
    NextToken ();
    return astIdent;
  }

  void AstBuilder::Expect (Lexer::TokenType tokenType)
  {
    if (currentToken.typeOrID != tokenType)
      throw Exception (Error::UnexpectedToken, currentToken, tokenType);
  }
  
  void AstBuilder::UnexpectedToken ()
  {
    throw Exception (Error::UnexpectedToken, currentToken);
  }

  ast::ProgramPtr AstBuilder::ParseProgram ()
  {
    return CommonParseNode (
      [&]()
      {
        ast::Program::StatementsContainer statements;
        while (true)
        {
          ast::ProgramStatementPtr statement;
          int beyondType;
          bool isType = IsWellKnownType (beyondType);
          if (currentToken.typeOrID == lexer::kwConst)
          {
            /* constant declaration */
            statement = CommonParseNode (
              [&]()
              {
                NextToken ();
                auto decl = ParseVarsDecl (true);
                Expect (lexer::Semicolon);
                NextToken();
                return ast::ProgramStatementPtr (new ast::ProgramStatementVarsDecl (std::move (decl)));
              });
          }
          else if (isType
                  || (currentToken.typeOrID == lexer::kwVoid)
                  || (Peek (beyondType).typeOrID == lexer::Identifier))
          {
            if ((currentToken.typeOrID == lexer::kwVoid)
                || (Peek (beyondType+1).typeOrID == lexer::ParenL))
            {
              /* Function declaration */
              auto decl = ParseFunctionDecl ();
              statement.reset (new ast::ProgramStatementFunctionDecl (std::move (decl)));
            }
            else
            {
              /* Variable declaration */
              statement = CommonParseNode (
                [&]()
                {
                  auto decl = ParseVarsDecl (false);
                  Expect (lexer::Semicolon);
                  NextToken();
                  return ast::ProgramStatementPtr (new ast::ProgramStatementVarsDecl (std::move (decl)));
                });
            }
          }
          else if (currentToken.typeOrID == lexer::kwTypedef)
          {
            /* Type definition */
            statement = CommonParseNode (
              [&]()
              {
                auto typeDef = ParseTypedef ();
                Expect (lexer::Semicolon);
                NextToken();
                return ast::ProgramStatementPtr (new ast::ProgramStatementTypedef (std::move (typeDef)));
              });
          }
          else
            /* Unknown token - throw error below */
            break;
          statements.emplace_back (std::move (statement));
        }
        if (currentToken.typeOrID != lexer::EndOfFile)
        {
          diagnosticsHandler.ParseError (Error::UnexpectedToken, currentToken, lexer::EndOfFile);
        }
        return ast::ProgramPtr (new ast::Program (std::move (statements)));
      });
  }

  ast::BlockPtr AstBuilder::ParseBlock ()
  {
    return CommonParseNode (
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
              auto statement = CommonParseNode (
                [&]()
                {
                  NextToken(); // skip 'const'
                  auto decl = ParseVarsDecl (true);
                  return ast::BlockStatementPtr (new ast::BlockStatementVarsDecl (std::move (decl)));
                });
              statements.emplace_back (std::move (statement));
              Expect (lexer::Semicolon);
              NextToken();
            }
            else if ((isType && (Peek (beyondType).typeOrID == lexer::Identifier))
                || ((currentToken.typeOrID == lexer::Identifier) && (Peek ().typeOrID == lexer::Identifier)))
            {
              /* Variable declaration */
              auto statement = CommonParseNode (
                [&]()
                {
                  auto decl = ParseVarsDecl (false);
                  return ast::BlockStatementPtr (new ast::BlockStatementVarsDecl (std::move (decl)));
                });
              statements.emplace_back (std::move (statement));
              Expect (lexer::Semicolon);
              NextToken();
            }
            else if (currentToken.typeOrID == lexer::kwTypedef)
            {
              /* Type definition */
              auto statement = CommonParseNode (
                [&]()
                {
                  auto astTypedef = ParseTypedef ();
                  return ast::BlockStatementPtr (new ast::BlockStatementTypedef (std::move (astTypedef)));
                });
              statements.emplace_back (std::move (statement));
              Expect (lexer::Semicolon);
              NextToken();
            }
            else if (IsExpression ())
            {
              auto astExpr = ParseExpression ();
              statements.emplace_back (new ast::BlockStatementExpr (std::move (astExpr)));
              Expect (lexer::Semicolon);
              NextToken();
            }
            else if (currentToken.typeOrID == lexer::kwReturn)
            {
              auto astReturn = ParseStatementReturn ();
              statements.emplace_back (std::move (astReturn));
            }
            else if (currentToken.typeOrID == lexer::kwIf)
            {
              /* 'if' */
              auto astIf = ParseIf ();
              statements.emplace_back (std::move (astIf));
            }
            else if (currentToken.typeOrID == lexer::kwWhile)
            {
              /* 'while' */
              auto astWhile = ParseWhile ();
              statements.emplace_back (std::move (astWhile));
            }
            else if (currentToken.typeOrID == lexer::kwFor)
            {
              /* 'for' */
              auto astFor = ParseFor ();
              statements.emplace_back (std::move (astFor));
            }
            else if (currentToken.typeOrID == lexer::BraceL)
            {
              /* nested block */
              NextToken ();
              auto astBlock = ParseBlock ();
              statements.emplace_back (new ast::BlockStatementNestedBlock (std::move (astBlock)));
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

  ast::BlockStatementReturnPtr AstBuilder::ParseStatementReturn ()
  {
    return CommonParseNode (
      [&]()
      {
        /* 'return' instruction */
        NextToken();
        ast::ExprPtr returnExpr;
        if (currentToken.typeOrID != lexer::Semicolon)
        {
          returnExpr = ParseExpression ();
        }
        Expect (lexer::Semicolon);
        NextToken();
        return ast::BlockStatementReturnPtr (new ast::BlockStatementReturn (std::move (returnExpr)));
      });
  }
  
  bool AstBuilder::IsBinaryOperationToken (Lexer::TokenType tokenType)
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
  
  bool AstBuilder::IsUnaryOperationToken (Lexer::TokenType tokenType)
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
  
  ast::ExprPtr AstBuilder::ParseExpression ()
  {
    return CommonParseNode (
      [&]()
      {
        auto expr = ParseExprLogicOr ();
        if (currentToken.typeOrID == lexer::Assign)
        {
          auto opToken = currentToken;
          // Assignment expression
          NextToken();
          auto assignedExpr = ParseExpression ();
          expr.reset (
            new ast::ExprBinary (std::move (expr), opToken, std::move (assignedExpr)));;
        }
        else if (currentToken.typeOrID == lexer::TernaryIf)
        {
          expr = ParseExprTernary (std::move (expr));
        }
        return expr;
      });
  }

  ast::ExprPtr AstBuilder::ParseExprBase ()
  {
    return CommonParseNode (
      [&]()
      {
        ast::ExprPtr expr;
        auto astValue = ParseExprValue ();
        if (astValue)
        {
          /* Need to check if it's identifier + '('.
          * We have a function call in that case */
          if ((astValue->value.typeOrID != lexer::Identifier)
              || (currentToken.typeOrID != lexer::ParenL))
          {
            expr = std::move (astValue);
          }
          else
          {
            // Expression is a function call
            auto params = ParseFuncParamActual ();
            expr.reset (new ast::ExprFunctionCall (
              ast::Identifier{ astValue->value }, std::move (params)));
          }
        }
        if (!expr)
        {
          int beyondType = 0;
          if (IsWellKnownTypeOrArray (beyondType) && (Peek (beyondType).typeOrID == lexer::ParenL))
          {
            // Expression is a type cast or ctor.
            auto astType = ParseType ();
            if (astType.has_error ())
              throw Exception (astType.error ().error, astType.error ().token);
            auto params = ParseFuncParamActual ();
            expr.reset (new ast::ExprFunctionCall (
              std::move (astType.value()), std::move (params)));
          }
          else if (currentToken.typeOrID == lexer::ParenL)
          {
            // '(' - nested expression
            NextToken();
            expr = ParseExpression ();
            Expect (lexer::ParenR);
            NextToken ();
          }
          else
          {
            UnexpectedToken();
          }
        }
        return ParseAttributeOrArrayAccess (std::move (expr));
      });
  }

  ast::ExprValuePtr AstBuilder::ParseExprValue ()
  {
    return CommonParseNode (
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

  ast::ExprPtr AstBuilder::ParseAttributeOrArrayAccess (ast::ExprPtr&& baseExpr)
  {
    return CommonParseNode (
      [&]()
      {
        ast::ExprPtr expr = std::move (baseExpr);
        while (true)
        {
          if (currentToken.typeOrID == lexer::Member)
          {
            NextToken ();
            auto attr = ParseIdentifier ();
            expr.reset (new ast::ExprAttribute (std::move (expr), std::move (attr)));
          }
          else if (currentToken.typeOrID == lexer::BracketL)
          {
            NextToken();
            auto indexExpr = ParseExpression ();
            Expect (lexer::BracketR);
            NextToken();
            expr.reset (new ast::ExprArrayElement (std::move (expr), std::move (indexExpr)));
          }
          else
            break;
        }
        return expr;
      });
  }

  ast::ExprPtr AstBuilder::ParseExprMultiplication ()
  {
    return CommonParseNode (
      [&]()
      {
        auto expr = ParseExprUnary ();
        while ((currentToken.typeOrID == lexer::Mult)
          || (currentToken.typeOrID == lexer::Div)
          || (currentToken.typeOrID == lexer::Mod))
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = ParseExprUnary ();
          expr.reset (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
        }
        return expr;
    });
  }

  ast::ExprPtr AstBuilder::ParseExprAddition ()
  {
    return CommonParseNode (
      [&]()
      {
        auto expr = ParseExprMultiplication ();
        while ((currentToken.typeOrID == lexer::Plus)
          || (currentToken.typeOrID == lexer::Minus))
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = ParseExprMultiplication ();
          expr.reset (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
        }
        return expr;
      });
  }

  ast::ExprPtr AstBuilder::ParseExprUnary ()
  {
    return CommonParseNode (
      [&]()
      {
        ast::ExprPtr expr;
        if ((currentToken.typeOrID == lexer::BitwiseInvert)
          || (currentToken.typeOrID == lexer::LogicInvert)
          || (currentToken.typeOrID == lexer::Minus))
        {
          auto opToken = currentToken;
          NextToken();
          expr = ParseExprBase ();
          expr.reset (new ast::ExprUnary (opToken, std::move (expr)));
        }
        else
          expr = ParseExprBase ();
        return expr;
      });
  }

  ast::ExprPtr AstBuilder::ParseExprTernary (ast::ExprPtr&& prefix)
  {
    return CommonParseNode (
      [&]()
      {
        auto expr (std::move (prefix));
        NextToken();
        auto expr2 = ParseExpression ();
        Expect (lexer::TernaryElse);
        NextToken();
        auto expr3 = ParseExpression ();
        return ast::ExprPtr (new ast::ExprTernary (std::move (expr), std::move (expr2), std::move (expr3)));
      });
  }

  ast::ExprPtr AstBuilder::ParseExprCompareEqual ()
  {
    return CommonParseNode (
      [&]()
      {
        auto expr = ParseExprComparison ();
        while ((currentToken.typeOrID == lexer::Equals)
          || (currentToken.typeOrID == lexer::NotEquals))
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = ParseExprComparison ();
          expr.reset (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
        }
        return expr;
      });
  }

  ast::ExprPtr AstBuilder::ParseExprComparison ()
  {
    return CommonParseNode (
      [&]()
      {
        auto expr = ParseExprAddition ();
        while ((currentToken.typeOrID == lexer::Larger)
          || (currentToken.typeOrID == lexer::LargerEqual)
          || (currentToken.typeOrID == lexer::Smaller)
          || (currentToken.typeOrID == lexer::SmallerEqual))
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = ParseExprAddition ();
          expr.reset (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
        }
        return expr;
      });
  }

  ast::ExprPtr AstBuilder::ParseExprLogicOr ()
  {
    return CommonParseNode (
      [&]()
      {
        auto expr = ParseExprLogicAnd ();
        while (currentToken.typeOrID == lexer::LogicOr)
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = ParseExprLogicAnd ();
          expr.reset (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
        }
        return expr;
      });
  }

  ast::ExprPtr AstBuilder::ParseExprLogicAnd ()
  {
    return CommonParseNode (
      [&]()
      {
        auto expr = ParseExprCompareEqual ();
        while (currentToken.typeOrID == lexer::LogicAnd)
        {
          auto opToken = currentToken;
          NextToken();
          auto expr2 = ParseExprCompareEqual ();
          expr.reset (new ast::ExprBinary (std::move (expr), opToken, std::move (expr2)));
        }
        return expr;
      });
  }

  bool AstBuilder::IsExpression ()
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

  bool AstBuilder::IsWellKnownType (int& peekAfterType)
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

  bool AstBuilder::IsWellKnownTypeOrArray (int& peekAfterType)
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

  AstBuilder::ParseTypeResult AstBuilder::ParseType ()
  {
    return CommonParseNode (
      [&]() -> AstBuilder::ParseTypeResult
      {
        ast::TypePtr type;
        if (currentToken.typeOrID == lexer::Identifier)
        {
          // Assume type alias
          type.reset (new ast::TypeIdentifier (ast::Identifier{ currentToken }));
          NextToken ();
        }
        else
        {
          int wellKnownExtra;
          if (IsWellKnownType (wellKnownExtra))
          {
            ast::TypeWellKnown::Tokens wellKnownTokens;
            for (int i = 0; i <= wellKnownExtra; i++)
            {
              wellKnownTokens.push_back (currentToken);
              NextToken ();
            }
            type.reset (new ast::TypeWellKnown (std::move (wellKnownTokens)));
          }
          else
          {
            return ParseError{ Error::ExpectedTypeName, currentToken };
          }
        }
        while ((currentToken.typeOrID == lexer::BracketL)
          && (Peek (0).typeOrID == lexer::BracketR))
        {
          type.reset (new ast::TypeArray (std::move (type)));
          NextToken ();
          NextToken ();
        }
        return type;
      });
  }

  ast::Typedef AstBuilder::ParseTypedef ()
  {
    // Skip typedef
    NextToken();
    // Aliased type
    auto astType = ParseType ();
    if (astType.has_error ())
      throw Exception (astType.error ().error, astType.error ().token);
    // Type alias
    auto aliasIdentifier = ParseIdentifier ();
    return ast::Typedef (std::move (astType.value()), std::move (aliasIdentifier));
  }

  ast::FunctionDeclPtr AstBuilder::ParseFunctionDecl ()
  {
    return CommonParseNode (
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
          auto astType = ParseType ();
          if (astType.has_error ())
            throw Exception (astType.error ().error, astType.error ().token);
          returnType = std::move (astType.value());
        }
        // Parse function identifier
        auto funcIdentifier = ParseIdentifier ();
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
            auto astType = ParseType ();
            if (astType.has_error ())
              throw Exception (astType.error ().error, astType.error ().token);
            param.type = std::move (astType.value ());
          }
          param.identifier = ParseIdentifier ();
          if (currentToken.typeOrID == lexer::Assign)
          {
            // Handle default value
            NextToken ();
            param.defaultValue = ParseExpression ();
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
        auto body = ParseBlock ();
        Expect (lexer::BraceR);
        NextToken();
        return ast::FunctionDeclPtr (new ast::FunctionDecl (std::move (returnType),
                                                            std::move (funcIdentifier),
                                                            std::move (params),
                                                            std::move (body)));
      });
  }

  std::vector<ast::ExprPtr> AstBuilder::ParseFuncParamActual ()
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

      auto expr = ParseExpression ();
      params.emplace_back (std::move (expr));

      if (currentToken.typeOrID == lexer::Separator)
        NextToken ();
      else if (currentToken.typeOrID != lexer::ParenR)
        UnexpectedToken ();
    }
    return params;
  }

  ast::VarsDecl AstBuilder::ParseVarsDecl (bool isConst)
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

    auto astType = ParseType ();
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
        initializer = ParseExpression ();
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
    return ast::VarsDecl (isConst, std::move (astType.value()), std::move (vars));
  }

  ast::BlockStatementForPtr AstBuilder::ParseFor ()
  {
    return CommonParseNode (
      [&]()
      {
        NextToken();
        Expect (lexer::ParenL);
        NextToken();
        ast::ExprPtr initExpr;
        if (currentToken.typeOrID != lexer::Semicolon)
          initExpr = ParseExpression ();
        Expect (lexer::Semicolon);
        NextToken();
        ast::ExprPtr loopTestExpr;
        if (currentToken.typeOrID != lexer::Semicolon)
          loopTestExpr = ParseExpression ();
        Expect (lexer::Semicolon);
        NextToken();
        ast::ExprPtr loopFootExpr;
        if (currentToken.typeOrID != lexer::Semicolon)
          loopFootExpr = ParseExpression ();
        Expect (lexer::ParenR);
        NextToken();
        Expect (lexer::BraceL);
        NextToken();
        auto body = ParseBlock ();
        Expect (lexer::BraceR);
        NextToken();
        return ast::BlockStatementForPtr (new ast::BlockStatementFor (std::move (initExpr),
                                                                      std::move (loopTestExpr),
                                                                      std::move (loopFootExpr),
                                                                      std::move (body)));
      });
  }

  ast::BlockStatementIfPtr AstBuilder::ParseIf ()
  {
    return CommonParseNode (
      [&]()
      {
        // Skip 'if'
        NextToken();
        Expect (lexer::ParenL);
        NextToken();
        // Parse condition
        auto conditionExpr = ParseExpression ();
        Expect (lexer::ParenR);
        NextToken();
        Expect (lexer::BraceL);
        NextToken();
        // Parse 'if' block
        auto ifBlock = ParseBlock ();
        Expect (lexer::BraceR);
        NextToken();
        ast::BlockPtr elseBlock;
        if (currentToken.typeOrID == lexer::kwElse)
        {
          NextToken();
          Expect (lexer::BraceL);
          NextToken();
          // Parse 'else' block
          elseBlock = ParseBlock ();
          Expect (lexer::BraceR);
          NextToken();
        }
        return ast::BlockStatementIfPtr (new ast::BlockStatementIf (std::move (conditionExpr),
                                                                    std::move (ifBlock),
                                                                    std::move (elseBlock)));
      });
  }

  ast::BlockStatementWhilePtr AstBuilder::ParseWhile ()
  {
    return CommonParseNode (
      [&]()
      {
        NextToken();
        Expect (lexer::ParenL);
        NextToken();
        auto loopTestExpr = ParseExpression ();
        Expect (lexer::ParenR);
        NextToken();
        Expect (lexer::BraceL);
        NextToken();
        auto body = ParseBlock ();
        Expect (lexer::BraceR);
        NextToken();
        return ast::BlockStatementWhilePtr (new ast::BlockStatementWhile (std::move (loopTestExpr),
                                                                          std::move (body)));
      });
  }
} // namespace s1
