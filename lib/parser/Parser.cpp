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
#include "parser/AstBuilder.h"
#include "parser/Diagnostics.h"
#include "parser/Exception.h"

namespace s1
{
  using namespace parser;
  
  Parser::Parser (Lexer& inputLexer, SemanticsHandler& semanticsHandler,
                  diagnostics::Handler& diagnosticsHandler)
   : inputLexer (inputLexer), semanticsHandler (semanticsHandler),
     diagnosticsHandler (diagnosticsHandler)
  {
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

  void Parser::ParseProgram ()
  {
    Scope globalScope (semanticsHandler.CreateScope (builtinScope, SemanticsHandler::Global));
    auto astProgram = AstBuilder (inputLexer, diagnosticsHandler).ParseProgram ();
    ParseProgramStatements (globalScope, *astProgram);
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

  void Parser::ParseTypedef (const Scope& scope, const parser::ast::Typedef& astTypedef)
  {
    Type aliasedType = ParseType (*astTypedef.type, scope);
    // Add to scope
    scope->AddTypeAlias (aliasedType, astTypedef.alias.GetString());
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
} // namespace s1
