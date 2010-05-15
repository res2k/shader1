#ifndef __PARSER_PARSER_H__
#define __PARSER_PARSER_H__

#include "lexer/Lexer.h"

#include "ErrorHandler.h"
#include "SemanticsHandler.h"

#include <vector>

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
     * \param semanticsHandler Handler for errors
     */
    Parser (Lexer& inputLexer, parser::SemanticsHandler& semanticsHandler,
	    parser::ErrorHandler& errorHandler);
    
    void Parse ();
  protected:
    Lexer& inputLexer;
    parser::SemanticsHandler& semanticsHandler;
    parser::ErrorHandler& errorHandler;
    /// Scope with builtin definitions
    parser::SemanticsHandler::ScopePtr builtinScope;
    
    Lexer::Token currentToken;
    void NextToken ();
    std::vector<Lexer::Token> nextTokens;
    const Lexer::Token& Peek (size_t lookahead = 0);
    
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
    Expression ParseAttributeOrArrayAccess (const Scope& scope, Expression baseExpr);
    Expression ParseExprMultiplication (const Scope& scope);
    Expression ParseExprAddition (const Scope& scope);
    Expression ParseExprUnary (const Scope& scope);
    Expression ParseExprTernary (const Scope& scope);
    Expression ParseExprCompareEqual (const Scope& scope);
    Expression ParseExprComparison (const Scope& scope);
    Expression ParseExprLogicOr (const Scope& scope);
    Expression ParseExprLogicAnd (const Scope& scope);
    Expression ParseExprConstBool ();
    
    // Types
    bool IsType (const Scope& scope, int& peekAfterType);
    bool IsType (const Scope& scope) { int dummy; return IsType (scope, dummy); }
    typedef parser::SemanticsHandler::TypePtr Type;
    Type ParseTypeBase (const Scope& scope);
    Type ParseType (const Scope& scope);
    Type ParseTypeBool ();
    Type ParseTypeNumeric (bool isUnsigned);
    Type ParseTypeVector (bool isUnsigned);
    Type ParseTypeMatrix (bool isUnsigned);
    Type ParseTypeSampler ();
    Type ParseTypeArray (Type baseType);
    void ParseTypedef (const Scope& scope);
    
    // Functions
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
