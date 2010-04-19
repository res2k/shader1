#ifndef __INTERMEDIATE_INTERMEDIATEGENERATORSEMANTICSHANDLER_H__
#define __INTERMEDIATE_INTERMEDIATEGENERATORSEMANTICSHANDLER_H__

#include "parser/CommonSemanticsHandler.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler : public s1::parser::SemanticsHandler
    {
      struct TypeImpl;
      struct NameImpl;
      class ScopeImpl;
      class BlockImpl;
    public:
      TypePtr CreateType (BaseType type);
      TypePtr CreateSamplerType (SamplerType dim);
      TypePtr CreateArrayType (TypePtr baseType);
      TypePtr CreateVectorType (TypePtr baseType,
				unsigned int components);
      TypePtr CreateMatrixType (TypePtr baseType,
				unsigned int columns,
				unsigned int rows);
      
      ExpressionPtr CreateConstBoolExpression (bool value)
      { return ExpressionPtr(); }
      ExpressionPtr CreateConstNumericExpression (const UnicodeString& valueStr)
      { return ExpressionPtr(); }
      ExpressionPtr CreateVariableExpression (NamePtr name)
      { return ExpressionPtr(); }
      ExpressionPtr CreateAttributeAccess (ExpressionPtr expr,
					   const UnicodeString& attr)
      { return ExpressionPtr(); }
      ExpressionPtr CreateArrayElementAccess (ExpressionPtr arrayExpr,
					      ExpressionPtr elementIndexExpr)
      { return ExpressionPtr(); }
      ExpressionPtr CreateAssignExpression (ExpressionPtr target,
					    ExpressionPtr value)

      { return ExpressionPtr(); }
      ExpressionPtr CreateArithmeticExpression (ArithmeticOp op,
						ExpressionPtr operand1,
						ExpressionPtr operand2)
      { return ExpressionPtr(); }
      ExpressionPtr CreateUnaryExpression (UnaryOp op,
					   ExpressionPtr operand)
						   
      { return ExpressionPtr(); }
      ExpressionPtr CreateTernaryExpression (ExpressionPtr condition,
					     ExpressionPtr ifExpr,
					     ExpressionPtr thenExpr)
      { return ExpressionPtr(); }
      ExpressionPtr CreateComparisonExpression (CompareOp op,
						ExpressionPtr operand1,
						ExpressionPtr operand2)
      { return ExpressionPtr(); }
      ExpressionPtr CreateLogicExpression (LogicOp op,
					   ExpressionPtr operand1,
					   ExpressionPtr operand2)
      { return ExpressionPtr(); }

      ExpressionPtr CreateFunctionCallExpression (NamePtr functionName,
						  const ExpressionVector& params)
      { return ExpressionPtr(); }
      
      ExpressionPtr CreateTypeConstructorExpression (TypePtr type,
						     const ExpressionVector& params)
      { return ExpressionPtr(); }
      
      ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel);
      
      BlockPtr CreateBlock (ScopePtr parentScope);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_INTERMEDIATEGENERATORSEMANTICSHANDLER_H__
