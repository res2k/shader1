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

#include "parser/Diagnostics.h"
#include "parser/Exception.h"

#include <assert.h>
#include <bitset>

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
    if (nextTokens.size() == 0)
    {
      currentToken = *inputLexer;
      ++inputLexer;
    }
    else
    {
      currentToken = nextTokens[0];
      nextTokens.erase (nextTokens.begin());
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
    Expression expr = ParseExprLogicOr (scope);
    if (currentToken.typeOrID == lexer::Assign)
    {
      // Assignment expression
      NextToken();
      Expression assignedExpr = ParseExpression (scope);
      expr = semanticsHandler.CreateAssignExpression (expr, assignedExpr);
    }
    else if (currentToken.typeOrID == lexer::TernaryIf)
    {
      expr = ParseExprTernary (expr, scope);
    }
    return expr;
  }

  Parser::Expression Parser::ParseExprBase (const Scope& scope)
  {
    Expression expr;
    int beyondType = 0;
    bool isType (false);
    if ((currentToken.typeOrID == lexer::Identifier)
        || ((isType = IsType (scope, beyondType))
          && (Peek (beyondType).typeOrID == lexer::ParenL)))
    {
      /* Expression could be: 
         * a variable/attribute value,
         * function call,
         * type constructor
       */
      Type type;
      Name idName;
      if (isType)
        type = ParseType (scope);
      else
      {
        idName = scope->ResolveIdentifier (currentToken.tokenString);
        NextToken();
      }
      /* if identifier is function name ... */
      if (isType || (idName->GetType() == SemanticsHandler::Name::Function))
      {
        // ... parse function call
        SemanticsHandler::ExpressionVector params;
        ParseFuncParamActual (scope, params);
        if (isType)
          expr = semanticsHandler.CreateTypeConstructorExpression (type, params);
        else
          expr = semanticsHandler.CreateFunctionCallExpression (idName, params);
      }
      else
      {
        expr = semanticsHandler.CreateVariableExpression (idName);
      }
    }
    else if (currentToken.typeOrID == lexer::ParenL)
    {
      // '(' - nested expression
      NextToken();
      expr = ParseExpression (scope);
      Expect (lexer::ParenR);
      NextToken ();
    }
    else if ((currentToken.typeOrID == lexer::kwTrue) || (currentToken.typeOrID == lexer::kwFalse))
    {
      // boolean constant
      expr = ParseExprConstBool();
    }
    else if (currentToken.typeOrID == lexer::Numeric)
    {
      // numeric constant
      expr = semanticsHandler.CreateConstNumericExpression (currentToken.tokenString);
      NextToken();
    }
    else
    {
      UnexpectedToken();
    }
    return ParseAttributeOrArrayAccess (scope, expr);
  }
  
  Parser::Expression Parser::ParseAttributeOrArrayAccess (const Scope& scope,
                                                          Expression baseExpr)
  {
    Parser::Expression expr = baseExpr;
    while (true)
    {
      if (currentToken.typeOrID == lexer::Member)
      {
        NextToken ();
        Expect (lexer::Identifier);
        expr = semanticsHandler.CreateAttributeAccess (expr, currentToken.tokenString);
        NextToken();
      }
      else if (currentToken.typeOrID == lexer::BracketL)
      {
        NextToken();
        Expression indexExpr = ParseExpression (scope);
        Expect (lexer::BracketR);
        NextToken();
        expr = semanticsHandler.CreateArrayElementAccess (expr, indexExpr);
      }
      else
        break;
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprMultiplication (const Scope& scope)
  {
    Expression expr = ParseExprUnary (scope);
    while ((currentToken.typeOrID == lexer::Mult)
      || (currentToken.typeOrID == lexer::Div)
      || (currentToken.typeOrID == lexer::Mod))
    {
      SemanticsHandler::ArithmeticOp op;
      switch (currentToken.typeOrID)
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
      default:
        S1_ASSERT_NOT_REACHED (expr);
      }
      NextToken();
      Expression expr2 = ParseExprUnary (scope);
      expr = semanticsHandler.CreateArithmeticExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprAddition (const Scope& scope)
  {
    Expression expr = ParseExprMultiplication (scope);
    while ((currentToken.typeOrID == lexer::Plus)
      || (currentToken.typeOrID == lexer::Minus))
    {
      SemanticsHandler::ArithmeticOp op;
      switch (currentToken.typeOrID)
      {
      case lexer::Plus:
        op = SemanticsHandler::Add;
        break;
      case lexer::Minus:
        op = SemanticsHandler::Sub;
        break;
      default:
        S1_ASSERT_NOT_REACHED (expr);
      }
      NextToken();
      Expression expr2 = ParseExprMultiplication (scope);
      expr = semanticsHandler.CreateArithmeticExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprUnary (const Scope& scope)
  {
    Expression expr;
    if ((currentToken.typeOrID == lexer::BitwiseInvert)
      || (currentToken.typeOrID == lexer::LogicInvert)
      || (currentToken.typeOrID == lexer::Minus))
    {
      SemanticsHandler::UnaryOp op;
      switch (currentToken.typeOrID)
      {
      default:
        S1_ASSERT_NOT_REACHED (expr);
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
      NextToken();
      expr = ParseExprBase (scope);
      expr = semanticsHandler.CreateUnaryExpression (op, expr);
    }
    else
      expr = ParseExprBase (scope);
    return expr;
  }
  
  Parser::Expression Parser::ParseExprTernary (const Expression& prefix, const Scope& scope)
  {
    Expression expr (prefix);
    NextToken();
    Expression expr2 = ParseExpression (scope);
    Expect (lexer::TernaryElse);
    NextToken();
    Expression expr3 = ParseExpression (scope);
    return semanticsHandler.CreateTernaryExpression (expr, expr2, expr3);
  }
  
  Parser::Expression Parser::ParseExprCompareEqual (const Scope& scope)
  {
    Expression expr = ParseExprComparison (scope);
    while ((currentToken.typeOrID == lexer::Equals)
      || (currentToken.typeOrID == lexer::NotEquals))
    {
      SemanticsHandler::CompareOp op;
      switch (currentToken.typeOrID)
      {
      case lexer::Equals:
        op = SemanticsHandler::Equals;
        break;
      case lexer::NotEquals:
        op = SemanticsHandler::NotEquals;
        break;
      default:
        S1_ASSERT_NOT_REACHED (expr);
      }
      NextToken();
      Expression expr2 = ParseExprComparison (scope);
      expr = semanticsHandler.CreateComparisonExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprComparison (const Scope& scope)
  {
    Expression expr = ParseExprAddition (scope);
    while ((currentToken.typeOrID == lexer::Larger)
      || (currentToken.typeOrID == lexer::LargerEqual)
      || (currentToken.typeOrID == lexer::Smaller)
      || (currentToken.typeOrID == lexer::SmallerEqual))
    {
      SemanticsHandler::CompareOp op;
      switch (currentToken.typeOrID)
      {
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
        S1_ASSERT_NOT_REACHED (expr);
      }
      NextToken();
      Expression expr2 = ParseExprAddition (scope);
      expr = semanticsHandler.CreateComparisonExpression (op, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprLogicOr (const Scope& scope)
  {
    Expression expr = ParseExprLogicAnd (scope);
    while (currentToken.typeOrID == lexer::LogicOr)
    {
      NextToken();
      Expression expr2 = ParseExprLogicAnd (scope);
      expr = semanticsHandler.CreateLogicExpression (SemanticsHandler::Or, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprLogicAnd (const Scope& scope)
  {
    Expression expr = ParseExprCompareEqual (scope);
    while (currentToken.typeOrID == lexer::LogicAnd)
    {
      NextToken();
      Expression expr2 = ParseExprCompareEqual (scope);
      expr = semanticsHandler.CreateLogicExpression (SemanticsHandler::And, expr, expr2);
    }
    return expr;
  }
  
  Parser::Expression Parser::ParseExprConstBool ()
  {
    Expression expr = semanticsHandler.CreateConstBoolExpression (
      currentToken.typeOrID == lexer::kwTrue);
    NextToken();
    return expr;
  }
  
  bool Parser::IsType (const Scope& scope, int& peekAfterType)
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
    case lexer::Identifier:
      /* Might be a type alias */
      {
        Name typeName = scope->ResolveIdentifier (currentToken.tokenString);
        isType = (typeName->GetType() == SemanticsHandler::Name::TypeAlias);
      }
      break;
    default:
      break;
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
  
  Parser::Type Parser::ParseTypeBase (const Scope& scope)
  {
    bool isUnsigned = false;
    auto baseToken = static_cast<lexer::TokenType> (currentToken.typeOrID & ~lexer::TypeFlagMask);
    if (baseToken == lexer::kwUnsigned)
    {
      isUnsigned = true;
      NextToken();
      baseToken = static_cast<lexer::TokenType> (currentToken.typeOrID & ~lexer::TypeFlagMask);
    }
    switch (baseToken)
    {
    case lexer::kwBool:
    case lexer::kwInt:
    case lexer::kwFloat:
      {
        switch (static_cast<lexer::TokenType> (currentToken.typeOrID & lexer::TypeFlagMask))
        {
        case 0:
          if (baseToken == lexer::kwBool)
            return ParseTypeBool ();
          else
            return ParseTypeNumeric (isUnsigned);
        case lexer::VecFlag:
          return ParseTypeVector (isUnsigned);
        case lexer::MatFlag:
          return ParseTypeMatrix (isUnsigned);
        default:
          break;
        }
      }
      break;
    case lexer::kwSampler1D:
    case lexer::kwSampler2D:
    case lexer::kwSampler3D:
    case lexer::kwSamplerCUBE:
      return ParseTypeSampler ();
    case lexer::Identifier:
      {
        Name typeName = scope->ResolveIdentifier (currentToken.tokenString);
        Type type;
        if (typeName->GetType() == SemanticsHandler::Name::TypeAlias)
        {
          type = typeName->GetAliasedType();
        }
        else
        {
          throw Exception (parser::Error::ExpectedTypeName, currentToken);
        }
        NextToken ();
        return type;
      }
      break;
    default:
      UnexpectedToken ();
    }
    return Type ();
  }
  
  Parser::Type Parser::ParseType (const Scope& scope)
  {
    Type type = ParseTypeBase (scope);
    if (currentToken.typeOrID == lexer::BracketL)
    {
      type = ParseTypeArray (type);
    }
    return type;
  }
  
  Parser::Type Parser::ParseTypeBool ()
  {
    NextToken();
    return semanticsHandler.CreateType (SemanticsHandler::Bool);
  }
  
  Parser::Type Parser::ParseTypeNumeric (bool isUnsigned)
  {
    Type type;
    switch (currentToken.typeOrID)
    {
    case lexer::kwInt:
      /* int */
      type = semanticsHandler.CreateType (
        isUnsigned ? SemanticsHandler::UInt : SemanticsHandler::Int);
      NextToken();
      break;
    case lexer::kwFloat:
      /* float */
      type = semanticsHandler.CreateType (SemanticsHandler::Float);
      NextToken();
      break;
    default:
      UnexpectedToken ();
    }
    return type;
  }
  
  Parser::Type Parser::ParseTypeVector (bool isUnsigned)
  {
    Type baseType;
    switch (static_cast<lexer::TokenType> (currentToken.typeOrID & ~lexer::TypeFlagMask))
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
      UnexpectedToken ();
    }
    Type vecType = semanticsHandler.CreateVectorType (baseType, currentToken.dimension1);
    NextToken();
    return vecType;
  }
  
  Parser::Type Parser::ParseTypeMatrix (bool isUnsigned)
  {
    Type baseType;
    switch (static_cast<lexer::TokenType> (currentToken.typeOrID & ~lexer::TypeFlagMask))
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
      UnexpectedToken ();
    }
    Type matType = semanticsHandler.CreateMatrixType (baseType, currentToken.dimension1,
                                                   currentToken.dimension2);
    NextToken();
    return matType;
  }
  
  Parser::Type Parser::ParseTypeSampler ()
  {
    Type type;
    switch (currentToken.typeOrID)
    {
    case lexer::kwSampler1D:
      /* 1D sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::_1D);
      NextToken();
      break;
    case lexer::kwSampler2D:
      /* 2D sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::_2D);
      NextToken();
      break;
    case lexer::kwSampler3D:
      /* 3D sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::_3D);
      NextToken();
      break;
    case lexer::kwSamplerCUBE:
      /* CUBE sampler */
      type = semanticsHandler.CreateSamplerType (SemanticsHandler::CUBE);
      NextToken();
      break;
    default:
      UnexpectedToken ();
    }
    return type;
  }
  
  Parser::Type Parser::ParseTypeArray (Type baseType)
  {
    NextToken();
    Expect (lexer::BracketR);
    Type type = semanticsHandler.CreateArrayType (baseType);
    NextToken ();
    if (currentToken.typeOrID == lexer::BracketL)
    {
      type = ParseTypeArray (type);
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
    NextToken();
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

  void Parser::ParseFuncParamActual (const Scope& scope, parser::SemanticsHandler::ExpressionVector& params)
  {
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
      
      Expression expr = ParseExpression (scope);
      params.push_back (expr);

      if (currentToken.typeOrID == lexer::Separator)
        NextToken ();
      else if (currentToken.typeOrID != lexer::ParenR)
        UnexpectedToken ();
    }
  }

  void Parser::ParseVarDeclare (const Scope& scope)
  {
    Type type = ParseType (scope);
    ParseVarIdentifierAndInitializerList (scope, type);
  }
  
  void Parser::ParseVarIdentifierAndInitializerList (const Scope& scope, Type type)
  {
    while (true)
    {
      ParseVarIdentifierAndInitializer (scope, type);
      if (currentToken.typeOrID == lexer::Separator)
      {
        // ',' - more variables follow
        NextToken();
        continue;
      }
      break;
    }
  }
  
  void Parser::ParseVarIdentifierAndInitializer (const Scope& scope, Type type)
  {
    Expect (lexer::Identifier);
    uc::String varIdentifier = currentToken.tokenString;
    NextToken ();
    Expression initExpr;
    if (currentToken.typeOrID == lexer::Assign)
    {
      // Variable has an initializer value
      NextToken ();
      initExpr = ParseExpression (scope);
    }
    scope->AddVariable (type, varIdentifier, initExpr, false);
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
