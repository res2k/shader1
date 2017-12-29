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

#ifndef __PARSER_PARSER_H__
#define __PARSER_PARSER_H__

#include "lexer/Lexer.h"

#include "ast/forwarddecl.h"
#include "parser/Diagnostics.h"
#include "SemanticsHandler.h"

#include "base/uc/SimpleBufferStreamSource.h"
#include "diagnostics/Handler.h"

#include <boost/container/deque.hpp>

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
    Parser (Lexer& inputLexer, parser::SemanticsHandler& semanticsHandler,
            diagnostics::Handler& diagnosticsHandler);
    
    void Parse ();
  protected:
    Lexer& inputLexer;
    parser::SemanticsHandler& semanticsHandler;
    diagnostics::Handler& diagnosticsHandler;
    /// Scope with builtin definitions
    parser::SemanticsHandler::ScopePtr builtinScope;
    
    Lexer::Token currentToken;
    Lexer::Token previousToken;
    void NextToken ();
    boost::container::deque<Lexer::Token> nextTokens;
    const Lexer::Token& Peek (size_t lookahead = 0);

    /// Parse error info
    struct ParseError
    {
      /// Error code
      parser::Error error;
      /// Offending token
      lexer::Token token;
    };
    /**
     * AST node parsing helper.
     * Calls func(), recording the starting and ending location
     */
    template<typename F>
    typename std::result_of<F()>::type CommonAstParseNode (F func);
    /// AST parsing helper: guaranteed return of an identifier
    parser::ast::Identifier AstParseIdentifier ();

    /// Expect a certain token, throw an "unexpected token" error if some other is encountered
    void Expect (Lexer::TokenType tokenType);
    /// Throw an "unexpected token" error
    void UnexpectedToken ();
    
    // Rough structure
    typedef parser::SemanticsHandler::BlockPtr Block;
    typedef parser::SemanticsHandler::ScopePtr Scope;
    void ParseProgram ();
    void ParseProgramStatements (const Scope& scope);
    void ParseBlock (Block block);
    bool IsCommand (const Scope& scope);
    void ParseCommand (Block block);

    // Expressions
    bool IsBinaryOperationToken (Lexer::TokenType tokenType);
    bool IsUnaryOperationToken (Lexer::TokenType tokenType);
    /// Returns whether the current token is the start of an expression.
    bool IsExpression (const Scope& scope);
    typedef parser::SemanticsHandler::ExpressionPtr Expression;
    typedef parser::SemanticsHandler::NamePtr Name;
    Expression ParseExpression (const Scope& scope);
    Expression ParseExprBase (const Scope& scope);
    parser::ast::ExprValuePtr AstParseExprValue ();
    Expression ParseAttributeOrArrayAccess (const Scope& scope, Expression baseExpr);
    Expression ParseExprMultiplication (const Scope& scope);
    Expression ParseExprAddition (const Scope& scope);
    Expression ParseExprUnary (const Scope& scope);
    Expression ParseExprTernary (const Expression& prefix, const Scope& scope);
    Expression ParseExprCompareEqual (const Scope& scope);
    Expression ParseExprComparison (const Scope& scope);
    Expression ParseExprLogicOr (const Scope& scope);
    Expression ParseExprLogicAnd (const Scope& scope);
    
    // Types
    bool IsWellKnownType (int& peekAfterType);
    bool IsType (const Scope& scope, int& peekAfterType);
    bool IsType (const Scope& scope) { int dummy; return IsType (scope, dummy); }
    typedef OUTCOME_V2_NAMESPACE::result<parser::ast::TypePtr, ParseError> AstParseTypeResult;
    AstParseTypeResult AstParseType ();
    typedef parser::SemanticsHandler::TypePtr Type;
    Type ParseTypeBase (parser::ast::Type& astType, const Scope& scope);
    Type ParseType (const Scope& scope);
    Type ParseType (parser::ast::Type& astType, const Scope& scope);
    Type ParseTypeBool (const Lexer::Token& token);
    Type ParseTypeNumeric (bool isUnsigned, const Lexer::Token& token);
    Type ParseTypeVector (bool isUnsigned, const Lexer::Token& token);
    Type ParseTypeMatrix (bool isUnsigned, const Lexer::Token& token);
    Type ParseTypeSampler (const Lexer::Token& token);
    void ParseTypedef (const Scope& scope);
    
    // Functions
    typedef parser::SemanticsHandler::FunctionPtr Function;
    void ParseFuncDeclare (const Scope& scope);
    Parser::Type ParseFuncType (const Scope& scope);
    void ParseFuncParamFormal (const Scope& scope,
			       parser::SemanticsHandler::Scope::FunctionFormalParameters& params);
    void ParseFuncParamActual (const Scope& scope, parser::SemanticsHandler::ExpressionVector& params);
    
    // Variables
    void ParseVarDeclare (const Scope& scope);
    void ParseVarIdentifierAndInitializerList (const Scope& scope, Type type);
    void ParseVarIdentifierAndInitializer (const Scope& scope, Type type);
	
    // Constants
    void ParseConstDeclare (const Scope& scope);
    void ParseConstIdentifierAndInitializerList (const Scope& scope, Type type);
    void ParseConstIdentifierAndInitializer (const Scope& scope, Type type);

    // Branches, Loops
    void ParseIf (Block block);
    void ParseLoopFor (Block block); 
    void ParseLoopWhile (Block block); 
  };
} // namespace s1

#endif // __PARSER_PARSER_H__
