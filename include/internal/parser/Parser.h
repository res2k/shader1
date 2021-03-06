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

#ifndef __PARSER_PARSER_H__
#define __PARSER_PARSER_H__

#include "ast/forwarddecl.h"

#include "diagnostics/Handler.h"
#include "lexer/Lexer.h"
#include "parser/Diagnostics_fwd.h"
#include "semantics/Diagnostics_fwd.h"
#include "semantics/Handler.h"
#include "semantics/Scope.h"

namespace s1
{
  /// Parser implementation
  class Parser
  {
  public:    
    /**
     * Construct parser.
     * \param inputLexer Input token stream
     * \param semanticsHandler Handler for semantics
     * \param diagnosticsHandler Handler for errors
     */
    Parser (Lexer& inputLexer, semantics::Handler& semanticsHandler,
            diagnostics::Handler& diagnosticsHandler);
    
    void Parse ();
  protected:
    Lexer& inputLexer;
    semantics::Handler& semanticsHandler;
    diagnostics::Handler& diagnosticsHandler;
    /// Scope with builtin definitions
    semantics::ScopePtr builtinScope;

    // Error handling
    struct ErrorInfo;
    class SimpleDiagnosticsImpl;
    void ParseError (const ErrorInfo& error);
    void ParseError (parser::Error code,
                     const Lexer::Token& encountered = lexer::Invalid,
                     const Lexer::TokenType& expected = lexer::Invalid);

    // Rough structure
    class VisitorProgramStatementImpl;
    typedef semantics::BlockPtr Block;
    typedef semantics::ScopePtr Scope;
    void ParseProgram ();
    void ParseProgramStatements (const Scope& scope, const parser::ast::Program& astProgram);
    class VisitorBlockStatementImpl;
    void ParseBlock (Block block, const parser::ast::Block& astBlock);

    class VisitorExprImpl;
    typedef semantics::ExpressionPtr Expression;
    Expression ParseExpression (const Scope& scope, const parser::ast::Expr* astExpr);
    Expression ParseExprValue (const Scope& scope, const parser::ast::ExprValue& astExprValue);
    Expression ParseExprArrayElement (const Scope& scope, const parser::ast::ExprArrayElement& astExprArrayElement);
    Expression ParseExprAttribute (const Scope& scope, const parser::ast::ExprAttribute& astExprAttribute);
    Expression ParseExprFunctionCall (const Scope& scope, const parser::ast::ExprFunctionCall& astExprFunctionCall);
    Expression ParseExprBinary (const Scope& scope, const parser::ast::ExprBinary& astExprBinary);
    Expression ParseExprArithmetic (const Scope& scope, const parser::ast::ExprBinary& astExprBinary);
    Expression ParseExprUnary (const Scope& scope, const parser::ast::ExprUnary& astExprUnary);
    Expression ParseExprTernary (const Scope& scope, const parser::ast::ExprTernary& astExprTernary);
    Expression ParseExprComparison (const Scope& scope, const parser::ast::ExprBinary& astExprBinary);
    Expression ParseExprLogic (const Scope& scope, const parser::ast::ExprBinary& astExprBinary);

    // Types
    class VisitorTypeImpl;
    typedef semantics::TypePtr Type;
    Type ParseType (parser::ast::Type* astType, const Scope& scope);
    Type ParseTypeBool (const Lexer::Token& token);
    Type ParseTypeNumeric (bool isUnsigned, const Lexer::Token& token);
    Type ParseTypeVector (bool isUnsigned, const Lexer::Token& token);
    Type ParseTypeMatrix (bool isUnsigned, const Lexer::Token& token);
    Type ParseTypeSampler (const Lexer::Token& token);
    void ParseTypedef (const Scope& scope, const parser::ast::Typedef& astTypedef);

    // Functions
    typedef semantics::FunctionPtr Function;
    void ParseFuncDeclare (const Scope& scope, const parser::ast::FunctionDecl& astFunctionDecl);
    void ParseFuncParamFormal (const Scope& scope,
                               semantics::FunctionFormalParameters& params,
                               const parser::ast::FunctionDecl& astFunctionDecl);
    
    // Variables
    void ParseVarDeclare (const Scope& scope, const parser::ast::VarsDecl& astVarsDecl);
  };
} // namespace s1

#endif // __PARSER_PARSER_H__
