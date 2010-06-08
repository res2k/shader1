#ifndef __INTERMEDIATE_INTERMEDIATEGENERATORSEMANTICSHANDLER_H__
#define __INTERMEDIATE_INTERMEDIATEGENERATORSEMANTICSHANDLER_H__

#include "parser/CommonSemanticsHandler.h"

#include "base/unordered_set"
#include "forwarddecl.h"
#include "Sequence.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler : public s1::parser::SemanticsHandler
    {
    protected:
      friend class ProgramFunction;
      
      struct TypeImpl;
      struct NameImpl;
      class ScopeImpl;
      class BlockImpl;
      
      class CommandImpl;
      class ExpressionImpl;
      class ArithmeticExpressionImpl;
      class AssignmentExpressionImpl;
      class BoolExpressionImpl;
      class ComparisonExpressionImpl;
      class FunctionCallExpressionImpl;
      class LogicExpressionImpl;
      class NumericExpressionImpl;
      class UnaryExpressionImpl;
      class VariableExpressionImpl;
      
      typedef boost::shared_ptr<NameImpl> NameImplPtr;
      typedef std::tr1::unordered_set<NameImplPtr> NameImplSet;
      
      typedef boost::shared_ptr<TypeImpl> TypeImplPtr;
      TypeImplPtr voidType;
      TypeImplPtr boolType;
      TypeImplPtr intType;
      TypeImplPtr uintType;
      TypeImplPtr floatType;
      
      std::string GetTypeString (const TypeImplPtr& type);
      
      typedef boost::shared_ptr<ScopeImpl> ScopeImplPtr;
      ScopeImplPtr builtinScope;
      ScopeImplPtr globalScope;
      
      UnicodeString entryFunction;
    public:
      /**\name Type utilities
       * @{ */
      static TypeImplPtr GetHigherPrecisionType (const TypeImplPtr& t1, const TypeImplPtr& t2);
      static BaseType DetectNumericType (const UnicodeString& numericStr);
      /**@}*/
      
      IntermediateGeneratorSemanticsHandler ();
      ~IntermediateGeneratorSemanticsHandler ();
      
      /**\name Basic types
       * @{ */
      TypeImplPtr GetVoidType() const { return voidType; }
      TypeImplPtr GetBoolType() const { return boolType; }
      TypeImplPtr GetIntType() const { return intType; }
      TypeImplPtr GetUintType() const { return uintType; }
      TypeImplPtr GetFloatType() const { return floatType; }
      /** @} */
      
      /**\name Register handling
       * @{ */
      enum RegisterClassification
      {
	Variable = 'v',
	Intermediate = 'i',
	Dummy = 'd',
	Imported = 'm',
	Condition = 'c'
      };
      /**
       * Allocate a new register.
       * If name is empty generates a dummy name.
       */
      RegisterID AllocateRegister (Sequence& seq, const TypeImplPtr& type,
				   RegisterClassification classify,
				   const UnicodeString& name = UnicodeString ());
      /// Create a new generation of a register
      RegisterID AllocateRegister (Sequence& seq, const RegisterID& oldReg);
      /** @} */
      
      void GenerateCast (Sequence& seq,
			 const RegisterID& castDestination,
			 const TypeImplPtr& typeDestination,
			 const RegisterID& castSource,
			 const TypeImplPtr& typeSource);
			 
      ProgramPtr GetProgram ();
      
      void SetEntryFunction (const UnicodeString& entryFunction) { this->entryFunction = entryFunction; }
      bool IsEntryFunction (const UnicodeString& entryFunction) const { return this->entryFunction == entryFunction; }
      
      /**\name s1::parser::SemanticsHandler implementation
       * @{ */
      TypePtr CreateType (BaseType type);
      TypePtr CreateSamplerType (SamplerType dim);
      TypePtr CreateArrayType (TypePtr baseType);
      TypePtr CreateVectorType (TypePtr baseType,
				unsigned int components);
      TypePtr CreateMatrixType (TypePtr baseType,
				unsigned int columns,
				unsigned int rows);
      
      ExpressionPtr CreateConstBoolExpression (bool value);
      ExpressionPtr CreateConstNumericExpression (const UnicodeString& valueStr);
      ExpressionPtr CreateVariableExpression (NamePtr name);
      ExpressionPtr CreateAttributeAccess (ExpressionPtr expr,
					   const UnicodeString& attr);
      ExpressionPtr CreateArrayElementAccess (ExpressionPtr arrayExpr,
					      ExpressionPtr elementIndexExpr);
      ExpressionPtr CreateAssignExpression (ExpressionPtr target,
					    ExpressionPtr value);
					    
      ExpressionPtr CreateArithmeticExpression (ArithmeticOp op,
						ExpressionPtr operand1,
						ExpressionPtr operand2);
      ExpressionPtr CreateUnaryExpression (UnaryOp op,
					   ExpressionPtr operand);
      ExpressionPtr CreateTernaryExpression (ExpressionPtr condition,
					     ExpressionPtr ifExpr,
					     ExpressionPtr thenExpr);
      ExpressionPtr CreateComparisonExpression (CompareOp op,
						ExpressionPtr operand1,
						ExpressionPtr operand2);
      ExpressionPtr CreateLogicExpression (LogicOp op,
					   ExpressionPtr operand1,
					   ExpressionPtr operand2);

      ExpressionPtr CreateFunctionCallExpression (NamePtr functionName,
						  const ExpressionVector& params);
      
      ExpressionPtr CreateTypeConstructorExpression (TypePtr type,
						     const ExpressionVector& params);
      
      ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel,
			    const TypePtr& returnType);
      ScopePtr CreateScope (ScopePtr parentScope, ScopeLevel scopeLevel)
      { return CreateScope (parentScope, scopeLevel, TypePtr ()); }
      
      BlockPtr CreateBlock (ScopePtr parentScope);
      /** @} */
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_INTERMEDIATEGENERATORSEMANTICSHANDLER_H__
