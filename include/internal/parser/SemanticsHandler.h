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

#ifndef __PARSER_SEMANTICSHANDLER_H__
#define __PARSER_SEMANTICSHANDLER_H__

#include <boost/shared_ptr.hpp>
#include <unicode/unistr.h>

#include <vector>

namespace s1
{
  namespace parser
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
    struct SemanticsHandler
    {
      virtual ~SemanticsHandler() {}
      
      struct Block;
      typedef boost::shared_ptr<Block> BlockPtr;
      
      /**\name Types
       * @{ */
      /// Base types
      enum BaseType { Void, Bool, Int, UInt, Float };
      /// Sampler dimensions
      enum SamplerType { _1D, _2D, _3D, CUBE };
      
      struct Type;
      typedef boost::shared_ptr<Type> TypePtr;
      /// Representation of a type
      struct Type
      {
	enum Class
	{
	  Base, Sampler, Array, Vector, Matrix
	};
	
	virtual ~Type() {}
	
	virtual Class GetTypeClass() const = 0;
	virtual BaseType GetBaseType() const = 0;
	virtual SamplerType GetSamplerType() const = 0;
	
	virtual TypePtr GetArrayVectorMatrixBaseType() const = 0;
	
	virtual unsigned int GetVectorTypeComponents() const = 0;
	
	virtual unsigned int GetMatrixTypeCols() const = 0;
	virtual unsigned int GetMatrixTypeRows() const = 0;
      };
      
      /// Create a base type
      virtual TypePtr CreateType (BaseType type) = 0;
      /// Create a sampler type
      virtual TypePtr CreateSamplerType (SamplerType dim) = 0;
      /// Create an array type
      virtual TypePtr CreateArrayType (TypePtr baseType) = 0;
      /// Create a vector type
      virtual TypePtr CreateVectorType (TypePtr baseType,
					unsigned int components) = 0;
      /// Create a matrix type
      virtual TypePtr CreateMatrixType (TypePtr baseType,
					unsigned int columns,
					unsigned int rows) = 0;
      /** @} */
      
      /**\name Names
       * A name refers to a variable, an attribute of it or an element.
       * @{ */
      struct Name
      {
        virtual ~Name() {}
        
	/// Type of name (function or variable/parameter)
	enum NameType { Function, Variable, TypeAlias };
	
	/// Get type of name
	virtual NameType GetType() = 0;
	
	/// Get aliased type
	virtual TypePtr GetAliasedType() = 0;
      };
      typedef boost::shared_ptr<Name> NamePtr;
      /** @} */
      
      /**\name Expressions
       * @{ */
      struct Expression
      {
        virtual ~Expression() {}
      };
      typedef boost::shared_ptr<Expression> ExpressionPtr;
      
      /// Create an expression for a constant bool value
      virtual ExpressionPtr CreateConstBoolExpression (bool value) = 0;
      /// Create an expression for a numeric value
      virtual ExpressionPtr CreateConstNumericExpression (const uc::String& valueStr) = 0;
      /// Create an expression for a variable/parameter
      virtual ExpressionPtr CreateVariableExpression (NamePtr name) = 0;
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

      /**\name Functions
       * @{ */
      struct Function
      {
        virtual ~Function() {}

        /// Get function block to add commands to.
        virtual BlockPtr GetBody() = 0;
        /// Finish function. Call after body was filled.
        virtual void Finish() = 0;
      };
      typedef boost::shared_ptr<Function> FunctionPtr;
      /** @} */
      
      /**\name Scope
       * @{ */
      
      /**
       * Levels of scope.
       * Not all declarations are allowed in all levels.
       */
      enum ScopeLevel      
      { 
	/// Builtin functions (and possibly variables+types)
	Builtin,
	/// Global functions, variables and types
	Global,
	/// Function-local variables and types
	Function
      };
      
      /**
       * Scope object, managing visibility of identifiers.
       */
      struct Scope
      {
        virtual ~Scope() {}

	/// Get level of the scope.
	virtual ScopeLevel GetLevel() const = 0;
	/**
	 * Add a variable or constant.
	 * \param type Type of variable.
	 * \param identifier Identifier of variable.
	 * \param initialValue Initial value of variable, can be a 0 pointer if none
	 *  is given. Required for constants
	 * \param constant Whether it is a constant variable.
	 */
	virtual NamePtr AddVariable (TypePtr type,
	  const uc::String& identifier,
	  ExpressionPtr initialValue,
	  bool constant) = 0;
	  
	/**
	 * Add a type alias.
	 * \param aliasedType Type to alias.
	 * \param identifier Identifier of type alias.
	 */
	virtual NamePtr AddTypeAlias (TypePtr aliasedType,
	  const uc::String& identifier) = 0;
	  
        /// Type of a format parameter
        enum FormalParameterType
        {
          /// User-defined parameter
          ptUser,
          /// Automatically generated parameter, representing a global var
          ptAutoGlobal
        };
	enum FormalParameterDirection
	{
	  dirDefault = 0,
	  dirIn = 1,
	  dirOut = 2,
	  dirInOut = 3
	};
	struct FunctionFormalParameter
	{
          /// Type of parameter
          FormalParameterType paramType;
	  TypePtr type;
	  uc::String identifier;
	  ExpressionPtr defaultValue;
	  FormalParameterDirection dir;

          FunctionFormalParameter() : paramType (ptUser) {}
	};
	typedef std::vector<FunctionFormalParameter> FunctionFormalParameters;
	  
	/**
	 * Add a function.
	 * \param returnType Return type of function.
	 * \param identifier Identifier of function.
	 * \param params Formal parameters.
	 * \returns Function object.
	 */
	virtual FunctionPtr AddFunction (TypePtr returnType,
	  const uc::String& identifier,
	  const FunctionFormalParameters& params) = 0;
	  
	/// Resolve an identifier to a name
	virtual NamePtr ResolveIdentifier (const uc::String& identifier) = 0;
      };
      typedef boost::shared_ptr<Scope> ScopePtr;
      
      /**
       * Create a scope.
       * \param parentScope Parent scope. If given, identifiers will be checked against the parent
       *   scope for uniqueness checks and identifier resolution.
       * \param scopeLevel Level of scope.
       */
      virtual ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel) = 0;
      /** @} */
      
      struct Block
      {
        virtual ~Block() {}

	virtual ScopePtr GetInnerScope() = 0;
	
	virtual void AddExpressionCommand (ExpressionPtr expr) = 0;
	virtual void AddReturnCommand (ExpressionPtr returnValue) = 0;
	virtual void AddBranching (ExpressionPtr branchCondition, BlockPtr ifBlock,
				   BlockPtr elseBlock) = 0;
	virtual void AddWhileLoop (ExpressionPtr loopCond, BlockPtr loopBlock) = 0;
	virtual void AddForLoop (ExpressionPtr initExpr, ExpressionPtr loopCond, ExpressionPtr tailExpr,
				 BlockPtr loopBlock) = 0;
	virtual void AddNestedBlock (BlockPtr block) = 0;
      };
      
      virtual BlockPtr CreateBlock (ScopePtr parentScope) = 0;
    };
  } // namespace parser
} // namespace s1

#endif // __PARSER_SEMANTICSHANDLER_H__
