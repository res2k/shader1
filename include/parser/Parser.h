#ifndef __PARSER_PARSER_H__
#define __PARSER_PARSER_H__

#include "lexer/Lexer.h"

#include "ParserSemanticsHandler.h"

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
    Parser (Lexer& inputLexer, ParserSemanticsHandler& semanticsHandler);
    
    void Parse ();
  protected:
    Lexer& inputLexer;
    ParserSemanticsHandler& semanticsHandler;
    
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
    void ParseValueIdentifier ();
    void ParseAssignment ();
    void ParseExpression ();
    void ParseValue ();
    void ParseExprBase ();
    void ParseAttributeList ();
    void ParseExprMultiplication ();
    void ParseExprAddition ();
    void ParseExprUnary ();
    void ParseExprTernary ();
    void ParseExprCompareEqual ();
    void ParseExprComparison ();
    void ParseExprLogicOr ();
    void ParseExprLogicAnd ();
    void ParseExprConstBool ();
    void ParseAttribute ();
    
    // Types
    typedef ParserSemanticsHandler::TypePtr Type;
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
