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
#include "parser/ast/Typedef.h"
#include "parser/ast/VarsDecl.h"
#include "parser/ast/VisitorExpr.h"
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

  class Parser::VisitorProgramStatementImpl : public ast::VisitorProgramStatement
  {
    Parser& parent;
    const Scope& scope;
  public:
    VisitorProgramStatementImpl (Parser& parent, const Scope& scope) : parent (parent), scope (scope) {}

    void operator() (const ast::ProgramStatementFunctionDecl& statement) override
    {
      parent.ParseFuncDeclare (scope, *statement.functionDecl);
    }
    void operator() (const ast::ProgramStatementTypedef& statement) override
    {
      parent.ParseTypedef (scope, *statement.typeDef);
    }
    void operator() (const ast::ProgramStatementVarsDecl& statement) override
    {
      parent.ParseVarDeclare (scope, *statement.varsDecl);
    }
  };

  void Parser::ParseProgram ()
  {
    Scope globalScope (semanticsHandler.CreateScope (builtinScope, SemanticsHandler::Global));
    auto astProgram = AstBuilder (inputLexer, diagnosticsHandler).ParseProgram ();
    ParseProgramStatements (globalScope, *astProgram);
  }

  void Parser::ParseProgramStatements (const Scope& scope, const ast::Program& astProgram)
  {
    VisitorProgramStatementImpl visitor (*this, scope);
    for (const auto& statement : astProgram.statements)
    {
      statement->Visit (visitor);
    }
  }

  class Parser::VisitorBlockStatementImpl : public ast::VisitorBlockStatement
  {
    Parser& parent;
    Block block;
    Scope scope;
  public:
    VisitorBlockStatementImpl (Parser& parent, Block block)
      : parent (parent), block (block), scope (block->GetInnerScope()) {}

    void operator() (const ast::BlockStatementExpr& statement) override
    {
      auto expr = parent.ParseExpression (scope, *statement.expr);
      block->AddExpressionCommand (expr);
    }
    void operator() (const ast::BlockStatementFor& statement) override
    {
      Expression initExpr, loopTestExpr, loopFootExpr;
      if (statement.initExpr)
      {
        initExpr = parent.ParseExpression (scope, *statement.initExpr);
      }
      if (statement.condition)
      {
        loopTestExpr = parent.ParseExpression (scope, *statement.condition);
      }
      if (statement.footExpr)
      {
        loopFootExpr = parent.ParseExpression (scope, *statement.footExpr);
      }
      Block newBlock = parent.semanticsHandler.CreateBlock (scope);
      parent.ParseBlock (newBlock, *statement.bodyBlock);
      block->AddForLoop (initExpr, loopTestExpr, loopFootExpr, newBlock);
    }
    void operator() (const ast::BlockStatementIf& statement) override
    {
      auto conditionExpr = parent.ParseExpression (scope, *statement.condition);
      Block ifBlock = parent.semanticsHandler.CreateBlock (scope);
      parent.ParseBlock (ifBlock, *statement.ifBlock);
      Block elseBlock;
      if (statement.elseBlock)
      {
        elseBlock = parent.semanticsHandler.CreateBlock (scope);
        parent.ParseBlock (elseBlock, *statement.elseBlock);
      }
      block->AddBranching (conditionExpr, ifBlock, elseBlock);
    }
    void operator() (const ast::BlockStatementNestedBlock& statement) override
    {
      /* nested block */
      Block newBlock = parent.semanticsHandler.CreateBlock (scope);
      parent.ParseBlock (newBlock, *statement.block);
      block->AddNestedBlock (newBlock);
    }
    void operator() (const ast::BlockStatementReturn& statement) override
    {
      Expression returnExpr;
      if (statement.expr)
      {
        /* Return with some value */
        returnExpr = parent.ParseExpression (scope, *statement.expr);
      }
      block->AddReturnCommand (returnExpr);
    }
    void operator() (const ast::BlockStatementTypedef& statement) override
    {
      parent.ParseTypedef (scope, *statement.typeDef);
    }
    void operator() (const ast::BlockStatementVarsDecl& statement) override
    {
      parent.ParseVarDeclare (scope, *statement.varsDecl);
    }
    void operator() (const ast::BlockStatementWhile& statement) override
    {
      auto loopTestExpr = parent.ParseExpression (scope, *statement.condition);
      Block newBlock = parent.semanticsHandler.CreateBlock (scope);
      parent.ParseBlock (newBlock, *statement.bodyBlock);
      block->AddWhileLoop (loopTestExpr, newBlock);
    }
  };

  void Parser::ParseBlock (Block block, const parser::ast::Block& astBlock)
  {
    VisitorBlockStatementImpl visitor (*this, block);
    for (const auto& statement : astBlock.statements)
    {
      statement->Visit (visitor);
    }
  }

  class Parser::VisitorExprImpl : public ast::VisitorExpr
  {
    Parser& parent;
    const Scope& scope;
  public:
    Parser::Expression parsedExpr;

    VisitorExprImpl (Parser& parent, const Scope& scope) : parent (parent), scope (scope) {}

    void operator() (const ast::ExprArrayElement& expr) override
    {
      parsedExpr = parent.ParseExprArrayElement (scope, expr);
    }
    void operator() (const ast::ExprAttribute& expr) override
    {
      parsedExpr = parent.ParseExprAttribute (scope, expr);
    }
    void operator() (const ast::ExprBinary& expr) override
    {
      parsedExpr = parent.ParseExprBinary (scope, expr);
    }
    void operator() (const ast::ExprFunctionCall& expr) override
    {
      parsedExpr = parent.ParseExprFunctionCall (scope, expr);
    }
    void operator() (const ast::ExprTernary& expr) override
    {
      parsedExpr = parent.ParseExprTernary (scope, expr);
    }
    void operator() (const ast::ExprUnary& expr) override
    {
      parsedExpr = parent.ParseExprUnary (scope, expr);
    }
    void operator() (const ast::ExprValue& expr) override
    {
      parsedExpr = parent.ParseExprValue (scope, expr);
    }
  };

  Parser::Expression Parser::ParseExpression (const Scope& scope, const ast::Expr& astExpr)
  {
    VisitorExprImpl visitor (*this, scope);
    astExpr.Visit (visitor);
    S1_ASSERT(visitor.parsedExpr, Expression ());
    return std::move (visitor.parsedExpr);
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
