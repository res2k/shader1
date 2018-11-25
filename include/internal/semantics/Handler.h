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

#ifndef SEMANTICS_HANDLER_H_
#define SEMANTICS_HANDLER_H_

#include "parser/Diagnostics_fwd.h"

#include "forwarddecl.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace s1
{
  namespace semantics
  {
    /**
     * Semantics handler.
     * The parser calls methods of this class while parsing the syntax.
     * The idea is that implementations build an internal representation from the
     * syntax. Also, semantic checking must be performed by the implementation.
     *
     * The syntax parser will ask the implementation to create objects in response
     * to syntactic constructs. Necessary context will be provided as well.
     *
     * In structure of objects is, in essence, tree-like (although the exact
     * organization is up the implementation).
     */
    class Handler
    {
    protected:
      TypePtr invalidType;
      TypePtr voidType;
      TypePtr boolType;
      TypePtr intType;
      TypePtr uintType;
      TypePtr floatType;
    public:
      Handler();
      virtual ~Handler();

      /**\name Types
       * @{ */
      /// Create a base type
      TypePtr CreateType (BaseType type);
      /// Create a sampler type
      TypePtr CreateSamplerType (SamplerType dim);
      /// Create an array type
      TypePtr CreateArrayType (TypePtr baseType);
      /// Create a vector type
      TypePtr CreateVectorType (TypePtr baseType,
                                unsigned int components);
      /// Create a matrix type
      TypePtr CreateMatrixType (TypePtr baseType,
                                unsigned int columns,
                                unsigned int rows);
      /** @} */

      /**\name Expressions
       * @{ */
      /// Create an expression for a constant bool value
      virtual ExpressionPtr CreateConstBoolExpression (bool value) = 0;
      /// Create an expression for a numeric value
      virtual ExpressionPtr CreateConstNumericExpression (const uc::String& valueStr) = 0;
      /// Create an expression for a variable/parameter
      virtual ExpressionPtr CreateVariableExpression (NameVariable* name) = 0;
      /// Create an expression that access an attribute of some other expression
      virtual ExpressionPtr CreateAttributeAccess (ExpressionPtr expr,
                                                   const uc::String& attr) = 0;
      /// Create an expression that accesses a member of an array
      virtual ExpressionPtr CreateArrayElementAccess (ExpressionPtr arrayExpr,
                                                      ExpressionPtr elementIndexExpr) = 0;
                                                   
      /// Create an expression that accesses a member of an array
      virtual ExpressionPtr CreateAssignExpression (ExpressionPtr target,
                                                    ExpressionPtr value) = 0;

      /// Possible arithmetic operations
      enum ArithmeticOp { Add, Sub, Mul, Div, Mod };
      /// Create an arithmetic expression
      virtual ExpressionPtr CreateArithmeticExpression (ArithmeticOp op,
                                                        ExpressionPtr operand1,
                                                        ExpressionPtr operand2) = 0;
      /// Possible unary operations
      enum UnaryOp { Neg, Inv, Not };
      /// Create an unary expression
      virtual ExpressionPtr CreateUnaryExpression (UnaryOp op,
                                                   ExpressionPtr operand) = 0;
                                                   
      /// Create a ternary expression
      virtual ExpressionPtr CreateTernaryExpression (ExpressionPtr condition,
                                                     ExpressionPtr ifExpr,
                                                     ExpressionPtr thenExpr) = 0;
                                                     
      /// Possible comparisons
      enum CompareOp { Equals, NotEquals, Smaller, SmallerEqual, Larger, LargerEqual };
      /// Create a comparison expression
      virtual ExpressionPtr CreateComparisonExpression (CompareOp op,
                                                        ExpressionPtr operand1,
                                                        ExpressionPtr operand2) = 0;
      /// Possible logic operations
      enum LogicOp { And, Or };
      /// Create a logic expression
      virtual ExpressionPtr CreateLogicExpression (LogicOp op,
                                                   ExpressionPtr operand1,
                                                   ExpressionPtr operand2) = 0;
                                                   
      typedef std::vector<ExpressionPtr> ExpressionVector;
      virtual ExpressionPtr CreateFunctionCallExpression (NamePtr functionName,
                                                          const ExpressionVector& params) = 0;
      
      virtual ExpressionPtr CreateTypeConstructorExpression (TypePtr type,
                                                          const ExpressionVector& params) = 0;
      /** @} */

      /**
       * Create a scope.
       * \param parentScope Parent scope. If given, identifiers will be checked against the parent
       *   scope for uniqueness checks and identifier resolution.
       * \param scopeLevel Level of scope.
       */
      virtual ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel) = 0;

      virtual BlockPtr CreateBlock (ScopePtr parentScope) = 0;

      /**
       * Create a function in the given scope.
       * \param scope Containing scope.
       * \param returnType Return type of function.
       * \param identifier Identifier of function.
       * \param params Formal parameters.
       * \returns Function object.
       */
      virtual FunctionPtr CreateFunction (Scope* parentScope,
                                          Type* returnType,
                                          const uc::String& identifier,
                                          const FunctionFormalParameters& params) = 0;
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_HANDLER_H_
