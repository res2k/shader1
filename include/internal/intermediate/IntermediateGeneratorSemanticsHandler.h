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

#ifndef __INTERMEDIATE_INTERMEDIATEGENERATORSEMANTICSHANDLER_H__
#define __INTERMEDIATE_INTERMEDIATEGENERATORSEMANTICSHANDLER_H__

#include "parser/CommonSemanticsHandler.h"

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
      class FunctionImpl;
      class ScopeImpl;
      class BlockImpl;
      
      class Builtin;
      typedef boost::shared_ptr<Builtin> BuiltinPtr;
      
      class CommandImpl;
      class ExpressionImpl;
      class ArithmeticExpressionImpl;
      class ArrayElementExpressionImpl;
      class AssignmentExpressionImpl;
      class AttributeExpressionImpl;
      class BoolExpressionImpl;
      class ComparisonExpressionImpl;
      class FunctionCallExpressionImpl;
      class LogicExpressionImpl;
      class NumericExpressionImpl;
      class TernaryExpressionImpl;
      class TypeConstructorExpressionImpl;
      class UnaryExpressionImpl;
      class VariableExpressionImpl;
      
      typedef boost::shared_ptr<NameImpl> NameImplPtr;
      typedef boost::unordered_set<NameImplPtr> NameImplSet;
      
      typedef boost::shared_ptr<TypeImpl> TypeImplPtr;
      TypeImplPtr voidType;
      TypeImplPtr boolType;
      TypeImplPtr intType;
      TypeImplPtr uintType;
      TypeImplPtr floatType;
      
      static std::string GetTypeString (const TypeImplPtr& type);
      
      typedef boost::shared_ptr<ScopeImpl> ScopeImplPtr;
      ScopeImplPtr builtinScope;
      ScopeImplPtr globalScope;
      
      // Helper functions for SetupBuiltins
      Scope::FunctionFormalParameters MakeFormalParameters1 (const TypePtr& type);
      Scope::FunctionFormalParameters MakeFormalParameters2 (const TypePtr& type);
      Scope::FunctionFormalParameters MakeFormalParameters2 (const TypePtr& typeA, const TypePtr& typeB);
      void SetupBuiltins (const ScopeImplPtr& scope);
      
      UnicodeString entryFunction;
    public:
      /**\name Type utilities
       * @{ */
      static TypeImplPtr GetHigherPrecisionType (const TypeImplPtr& t1, const TypeImplPtr& t2);
      static BaseType DetectNumericType (const UnicodeString& numericStr);
      static std::string GetTypeString (const TypePtr& type);
      /**@}*/
      
      /**\name Attribute utilities
       * @{ */
      struct Attribute
      {
	enum AttrClass
	{
	  Unknown,
	  
	  arrayLength,
	  
	  matrixRow,
	  matrixCol,
	  matrixTranspose,
	  matrixInvert,
	  
	  vectorSwizzle
	};
	AttrClass attrClass;
	unsigned char swizzleCompNum;
	unsigned char swizzleComps;
	
	Attribute (AttrClass ac) : attrClass (ac), swizzleCompNum (0), swizzleComps (0) {}
	Attribute (unsigned char swizNum, unsigned char comp1 = 0, unsigned char comp2 = 0,
		   unsigned char comp3 = 0, unsigned char comp4 = 0)
	 : attrClass (vectorSwizzle), swizzleCompNum (swizNum),
	   swizzleComps ((comp1 & 3) | ((comp2 & 3) << 2) | ((comp3 & 3) << 4) | ((comp4 & 3) << 6))
	{}
	
	unsigned char GetSwizzleComp (unsigned char num) const
	{ return (swizzleComps >> (num*2)) & 3; }
      };
      static Attribute IdentifyAttribute (const UnicodeString& attributeStr);
      TypeImplPtr GetAttributeType (const TypeImplPtr& expressionType,
				    const Attribute& attr);
      /** @} */
      
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
	Condition = 'c',
	Index = 'x'
      };
      /**
       * Allocate a new register.
       * If name is empty generates a dummy name.
       */
      RegisterPtr AllocateRegister (Sequence& seq, const TypeImplPtr& type,
				    RegisterClassification classify,
				    const UnicodeString& name = UnicodeString ());
      /// Create a new generation of a register
      RegisterPtr AllocateRegister (Sequence& seq, const RegisterPtr& oldReg);
      /** @} */
      
      void GenerateCast (Sequence& seq,
			 const RegisterPtr& castDestination,
			 const TypeImplPtr& typeDestination,
			 const RegisterPtr& castSource,
			 const TypeImplPtr& typeSource);
			 
      ProgramPtr GetProgram ();

    #ifdef _MSC_VER
    #  pragma warning(push)
    #  pragma warning(disable : 4800)
    #endif
      void SetEntryFunction (const UnicodeString& entryFunction) { this->entryFunction = entryFunction; }
      bool IsEntryFunction (const UnicodeString& entryFunction) const { return this->entryFunction == entryFunction; }
    #ifdef _MSC_VER
    #  pragma warning(pop)
    #endif
      
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
