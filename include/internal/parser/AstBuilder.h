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

/**\file
 * AST builder (parser) implementation
 */
#ifndef PARSER_ASTBUILDER_H_
#define PARSER_ASTBUILDER_H_

#include "ast/forwarddecl.h"
#include "lexer/Lexer.h"
#include "parser/Diagnostics.h"

#include <vector>

#include <boost/container/deque.hpp>
#include <boost/optional.hpp>

namespace s1
{
  namespace diagnostics
  {
    struct Handler;
  } // namespace diagnostics

  namespace parser
  {
    /// AST builder (parser) implementation
    class AstBuilder
    {
    public:
      /**
       * Construct AST builder.
       * \param inputLexer Input token stream
       * \param diagnosticsHandler Handler for errors
       */
      AstBuilder (Lexer& inputLexer, diagnostics::Handler& diagnosticsHandler);

      /**
       * Parse a program into an AST.
       * This will consume all tokens from the input lexer.
       */
      ast::ProgramPtr ParseProgram ();
    protected:
      Lexer& inputLexer;
      diagnostics::Handler& diagnosticsHandler;
      
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
      typename std::result_of<F()>::type CommonParseNode (F func);
      typedef std::pair<ast::Identifier, boost::optional<ParseError>> ParseIdentifierResult;
      /**
       * AST parsing helper: guaranteed return of an identifier.
       * If an error is returned the identifier may not be valid.
       */
      ParseIdentifierResult ParseIdentifier ();
      /// Parse identifier. Emits error if occured. Returned identifier may not actually be valid.
      ParseIdentifierResult ParseIdentifierAndReport ();

      //@{
      /// Return value in an outcome result or throw Exception
      template<typename T>
      const T& CheckResult (const OUTCOME_V2_NAMESPACE::result<T, ParseError>& result);
      template<typename T>
      T CheckResult (OUTCOME_V2_NAMESPACE::result<T, ParseError>&& result);
      template<typename T>
      const T& CheckResult (const std::pair<T, boost::optional<ParseError>>& result);
      template<typename T>
      T CheckResult (std::pair<T, boost::optional<ParseError>>&& result);
      //@}
      /**
       * Try to 'recover' from a parsing error by looking for tokens that fulfill any of
       * the \c CheckFunc functors.
       */
      template<typename... CheckFunc>
      bool TryRecover (CheckFunc... func);
      /// Expect a certain token, throw an "unexpected token" error if some other is encountered
      void Expect (Lexer::TokenType tokenType);
      /// Throw an "unexpected token" error
      void UnexpectedToken ();
      /// Expect a specific token. Report \c error if another token is encountered. Non-throwing
      bool Expect (Lexer::TokenType tokenType, Error error);
      /// Expect a semicolon token. Reports an error if another token is encountered.
      bool ExpectSemicolon ();

      // Rough structure
      ast::BlockPtr ParseBlock ();
      ast::BlockStatementReturnPtr ParseStatementReturn ();

      // Expressions
      bool IsBinaryOperationToken (Lexer::TokenType tokenType);
      bool IsUnaryOperationToken (Lexer::TokenType tokenType);
      ast::ExprPtr ParseExpression ();
      ast::ExprPtr ParseExprBase ();
      ast::ExprValuePtr ParseExprValue ();
      ast::ExprPtr ParseAttributeOrArrayAccess (ast::ExprPtr&& baseExpr);
      ast::ExprPtr ParseExprMultiplication ();
      ast::ExprPtr ParseExprAddition ();
      ast::ExprPtr ParseExprUnary ();
      ast::ExprPtr ParseExprTernary (ast::ExprPtr&& prefix);
      ast::ExprPtr ParseExprCompareEqual ();
      ast::ExprPtr ParseExprComparison ();
      ast::ExprPtr ParseExprLogicOr ();
      ast::ExprPtr ParseExprLogicAnd ();

      /// Returns whether the current token is the start of an expression.
      bool IsExpression ();

      // Types
      bool IsWellKnownType (int& peekAfterType);
      bool IsWellKnownTypeOrArray (int& peekAfterType);
      ast::TypePtr ParseType ();
      boost::optional<ast::Typedef> ParseTypedef ();

      // Functions
      ast::FunctionDeclPtr ParseFunctionDecl ();
      std::vector<ast::ExprPtr> ParseFuncParamActual ();

      // Variables, Constants
      ast::VarsDecl ParseVarsDecl (bool isConst = false);

      // Branches, Loops
      ast::BlockStatementForPtr ParseFor ();
      ast::BlockStatementIfPtr ParseIf ();
      ast::BlockStatementWhilePtr ParseWhile ();
    };
  } // namespace parser
} // namespace s1

#endif // PARSER_ASTBUILDER_H_
