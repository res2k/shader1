/*
    Shader1
    Copyright (c) 2017-2018 Frank Richter


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
 * AST forward declaration
 */
#ifndef S1_PARSER_AST_FORWARDDECL_H_
#define S1_PARSER_AST_FORWARDDECL_H_

#include <memory>

namespace s1
{
  namespace parser
  {
    namespace ast
    {
      struct Identifier;
      struct Node;
      typedef std::unique_ptr<Node> NodePtr;

      struct Block;
      typedef std::unique_ptr<Block> BlockPtr;
      struct BlockStatement;
      typedef std::unique_ptr<BlockStatement> BlockStatementPtr;
      struct BlockStatementExpr;
      typedef std::unique_ptr<BlockStatementExpr> BlockStatementExprPtr;
      struct BlockStatementFor;
      typedef std::unique_ptr<BlockStatementFor> BlockStatementForPtr;
      struct BlockStatementIf;
      typedef std::unique_ptr<BlockStatementIf> BlockStatementIfPtr;
      struct BlockStatementNestedBlock;
      typedef std::unique_ptr<BlockStatementNestedBlock> BlockStatementNestedBlockPtr;
      struct BlockStatementReturn;
      typedef std::unique_ptr<BlockStatementReturn> BlockStatementReturnPtr;
      struct BlockStatementTypedef;
      typedef std::unique_ptr<BlockStatementTypedef> BlockStatementTypedefPtr;
      struct BlockStatementVarsDecl;
      typedef std::unique_ptr<BlockStatementVarsDecl> BlockStatementVarsDeclPtr;
      struct BlockStatementWhile;
      typedef std::unique_ptr<BlockStatementWhile> BlockStatementWhilePtr;
      struct Expr;
      typedef std::unique_ptr<Expr> ExprPtr;
      struct ExprArrayElement;
      typedef std::unique_ptr<ExprArrayElement> ExprArrayElementPtr;
      struct ExprAttribute;
      typedef std::unique_ptr<ExprAttribute> ExprAttributePtr;
      struct ExprBinary;
      typedef std::unique_ptr<ExprBinary> ExprBinaryPtr;
      struct ExprFunctionCall;
      typedef std::unique_ptr<ExprFunctionCall> ExprFunctionCallPtr;
      struct ExprTernary;
      typedef std::unique_ptr<ExprTernary> ExprTernaryPtr;
      struct ExprUnary;
      typedef std::unique_ptr<ExprUnary> ExprUnaryPtr;
      struct ExprValue;
      typedef std::unique_ptr<ExprValue> ExprValuePtr;
      struct FunctionDecl;
      typedef std::unique_ptr<FunctionDecl> FunctionDeclPtr;
      struct Program;
      typedef std::unique_ptr<Program> ProgramPtr;
      struct ProgramStatement;
      typedef std::unique_ptr<ProgramStatement> ProgramStatementPtr;
      struct ProgramStatementFunctionDecl;
      typedef std::unique_ptr<ProgramStatementFunctionDecl> ProgramStatementFunctionDeclPtr;
      struct ProgramStatementTypedef;
      typedef std::unique_ptr<ProgramStatementTypedef> ProgramStatementTypedefPtr;
      struct ProgramStatementVarsDecl;
      typedef std::unique_ptr<ProgramStatementVarsDecl> ProgramStatementVarsDeclPtr;
      struct Type;
      typedef std::unique_ptr<Type> TypePtr;
      struct TypeArray;
      typedef std::unique_ptr<TypeArray> TypeArrayPtr;
      struct Typedef;
      typedef std::unique_ptr<Typedef> TypedefPtr;
      struct TypeIdentifier;
      typedef std::unique_ptr<TypeIdentifier> TypeIdentifierPtr;
      struct TypeWellKnown;
      typedef std::unique_ptr<TypeWellKnown> TypeWellKnownPtr;
      struct VarsDecl;
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_FORWARDDECL_H_
