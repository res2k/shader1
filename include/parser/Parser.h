#ifndef __PARSER_PARSER_H__
#define __PARSER_PARSER_H__

#include "lexer/Lexer.h"

#include "SemanticsHandler.h"

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
    
    Lexer::Token currentToken;
    void NextToken ();
    
    /// Expect a certain token, throw an "unexpected token" error if some other is encountered
    void Expect (Lexer::TokenType tokenType);
    /// Throw an "unexpected token" error
    void UnexpectedToken ();
    
    // Rough structure
    void ParseProgram ();
    void ParseProgramStatements ();
    void ParseBlock ();
    void ParseCommand ();

    // Expressions
    typedef parser::SemanticsHandler::ExpressionPtr Expression;
    typedef parser::SemanticsHandler::NamePtr Name;
    Expression ParseExpression ();
    Expression ParseExprBase ();
    Expression ParseAttributeOrArrayAccess (Expression baseExpr);
    Expression ParseExprMultiplication ();
    Expression ParseExprAddition ();
    Expression ParseExprUnary ();
    Expression ParseExprTernary ();
    Expression ParseExprCompareEqual ();
    Expression ParseExprComparison ();
    Expression ParseExprLogicOr ();
    Expression ParseExprLogicAnd ();
    Expression ParseExprConstBool ();
    
    // Types
    typedef parser::SemanticsHandler::TypePtr Type;
    Type ParseTypeBase ();
    Type ParseType ();
    Type ParseTypeBool ();
    Type ParseTypeNumeric (bool isUnsigned);
    Type ParseTypeVector (bool isUnsigned);
    Type ParseTypeMatrix (bool isUnsigned);
    Type ParseTypeSampler ();
    Type ParseTypeArray (Type baseType);
    void ParseTypedef ();
    
    // Attributes
    void ParseAttrVector ();
    void ParseAttrMatrix ();
    void ParseAttrArray ();
	
    // Functions
    void ParseFuncDeclare ();
    void ParseFuncType ();
    void ParseFuncParamFormal ();
    void ParseFuncCall ();
    void ParseFuncParamActual ();
    
    // Variables
    void ParseVarDeclare ();
    void ParseVarIdentifierAndInitializerList ();
    void ParseVarIdentifierAndInitializer ();
	
    // Constants
    void ParseConstDeclare ();
    void ParseConstIdentifierAndInitializerList ();
    void ParseConstIdentifierAndInitializer ();

    // Branches, Loops
    void ParseIf ();
    void ParseLoopFor (); 
    void ParseLoopWhile (); 
  };
} // namespace s1

#endif // __PARSER_PARSER_H__
