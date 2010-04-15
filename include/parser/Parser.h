#ifndef __PARSER_PARSER_H__
#define __PARSER_PARSER_H__

#include "lexer/Lexer.h"

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
     */
    Parser (Lexer& inputLexer, parser::SemanticsHandler& semanticsHandler);
    
    void Parse ();
  protected:
    Lexer& inputLexer;
    parser::SemanticsHandler& semanticsHandler;
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
    void ParseProgramStatements (Scope scope);
    void ParseBlock (Block block);
    bool IsCommand (Scope scope);
    void ParseCommand (Block block);

    // Expressions
    /// Returns whether the current token is the start of an expression.
    bool IsExpression (Scope scope);
    typedef parser::SemanticsHandler::ExpressionPtr Expression;
    typedef parser::SemanticsHandler::NamePtr Name;
    Expression ParseExpression (Scope scope);
    Expression ParseExprBase (Scope scope);
    Expression ParseAttributeOrArrayAccess (Scope scope, Expression baseExpr);
    Expression ParseExprMultiplication (Scope scope);
    Expression ParseExprAddition (Scope scope);
    Expression ParseExprUnary (Scope scope);
    Expression ParseExprTernary (Scope scope);
    Expression ParseExprCompareEqual (Scope scope);
    Expression ParseExprComparison (Scope scope);
    Expression ParseExprLogicOr (Scope scope);
    Expression ParseExprLogicAnd (Scope scope);
    Expression ParseExprConstBool ();
    
    // Types
    bool IsType (Scope scope, int& peekAfterType);
    bool IsType (Scope scope) { int dummy; return IsType (scope, dummy); }
    typedef parser::SemanticsHandler::TypePtr Type;
    Type ParseTypeBase (Scope scope);
    Type ParseType (Scope scope);
    Type ParseTypeBool ();
    Type ParseTypeNumeric (bool isUnsigned);
    Type ParseTypeVector (bool isUnsigned);
    Type ParseTypeMatrix (bool isUnsigned);
    Type ParseTypeSampler ();
    Type ParseTypeArray (Type baseType);
    void ParseTypedef (Scope scope);
    
    // Attributes
    void ParseAttrVector ();
    void ParseAttrMatrix ();
    void ParseAttrArray ();
	
    // Functions
    void ParseFuncDeclare (Scope scope);
    Parser::Type ParseFuncType (Scope scope);
    void ParseFuncParamFormal (Scope scope,
			       parser::SemanticsHandler::Scope::FunctionFormalParameters& params);
    void ParseFuncCall ();
    void ParseFuncParamActual ();
    
    // Variables
    void ParseVarDeclare (Scope scope);
    void ParseVarIdentifierAndInitializerList (Scope scope, Type type);
    void ParseVarIdentifierAndInitializer (Scope scope, Type type);
	
    // Constants
    void ParseConstDeclare (Scope scope);
    void ParseConstIdentifierAndInitializerList (Scope scope, Type type);
    void ParseConstIdentifierAndInitializer (Scope scope, Type type);

    // Branches, Loops
    void ParseIf (Block block);
    void ParseLoopFor (Block block); 
    void ParseLoopWhile (Block block); 
  };
} // namespace s1

#endif // __PARSER_PARSER_H__
