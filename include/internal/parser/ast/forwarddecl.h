/*
    Shader1
    Copyright (c) 2017 Frank Richter


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

      struct BlockStatementReturn;
      typedef std::unique_ptr<BlockStatementReturn> BlockStatementReturnPtr;
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
      struct Type;
      typedef std::unique_ptr<Type> TypePtr;
      struct Typedef;
      typedef std::unique_ptr<Typedef> TypedefPtr;
      struct VarsDecl;
      typedef std::unique_ptr<VarsDecl> VarsDeclPtr;
    } // namespace ast
  } // namespace parser
} // namespace s1

#endif // S1_PARSER_AST_FORWARDDECL_H_
