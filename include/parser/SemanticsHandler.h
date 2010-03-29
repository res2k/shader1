#ifndef __PARSER_SEMANTICSHANDLER_H__
#define __PARSER_SEMANTICSHANDLER_H__

#include <boost/shared_ptr.hpp>
#include <unicode/unistr.h>

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
      
      /**\name Names
       * A name refers to a variable, an attribute of it or an element.
       * @{ */
      struct Name
      {
	/// Type of name (function or variable/parameter)
	enum NameType { Function, Variable };
	
	/// Get tpye of name
	virtual NameType GetType() = 0;
      };
      typedef boost::shared_ptr<Name> NamePtr;
      
      /// Resolve an identifier to a name
      virtual NamePtr ResolveIdentifier (const UnicodeString& identifier) = 0;
      /** @} */
      
      /**\name Expressions
       * @{ */
      struct Expression
      {
      };
      typedef boost::shared_ptr<Expression> ExpressionPtr;
      
      /// Create an expression for a constant bool value
      virtual ExpressionPtr CreateConstBoolExpression (bool value) = 0;
      /// Create an expression for a numeric value
      virtual ExpressionPtr CreateConstNumericExpression (const UnicodeString& valueStr) = 0;
      /// Create an expression for a variable/parameter
      virtual ExpressionPtr CreateVariableExpression (NamePtr name) = 0;
      /// Create an expression that access an attribute of some other expression
      virtual ExpressionPtr CreateAttributeAccess (ExpressionPtr expr,
						   const UnicodeString& attr) = 0;
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
      /** @} */
      
      /**\name Types
       * @{ */
      /// Representation of a type
      struct Type
      {
      };
      typedef boost::shared_ptr<Type> TypePtr;
      
      /// Base types
      enum BaseType { Bool, Int, UInt, Float };
      /// Create a base type
      virtual TypePtr CreateType (BaseType type) = 0;
      /// Sampler dimensions
      enum SamplerType { _1D, _2D, _3D, CUBE };
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
    };
  } // namespace parser
} // namespace s1

#endif // __PARSER_SEMANTICSHANDLER_H__
