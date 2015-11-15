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

#include "base/uc/String.h"

#include "parser/CommonSemanticsHandler.h"

#include "forwarddecl.h"
#include "Sequence.h"

namespace s1
{
  namespace intermediate
  {
    class IntermediateGeneratorSemanticsHandler :
      public s1::parser::CommonSemanticsHandler
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

      /// Create a sequence containing global vars initialization
      SequencePtr CreateGlobalVarInitializationSeq (NameImplSet& exportedNames);

      ProgramFunctionPtr SynthesizeEntryFunction (const uc::String& realEntryIdentifier,
                                                  const TypePtr& returnType,
                                                  const Scope::FunctionFormalParameters& params);

      /// Whether the intermediate program was marked "completed"
      bool completed;
    public:
      /**\name Type utilities
       * @{ */
      static TypeImplPtr GetHigherPrecisionType (const TypeImplPtr& t1, const TypeImplPtr& t2);
      static std::string GetTypeString (const TypePtr& type);
      /**@}*/
      
      /**\name Attribute utilities
       * @{ */
      struct Attribute : public CommonSemanticsHandler::Attribute
      {
        using CommonSemanticsHandler::Attribute::Attribute;
        Attribute (const CommonSemanticsHandler::Attribute& other)
          : CommonSemanticsHandler::Attribute (other) {}
        Attribute (CommonSemanticsHandler::Attribute&& other)
          : CommonSemanticsHandler::Attribute (std::move (other)) {}

        unsigned char GetSwizzleComp (unsigned char num) const
        { return (swizzleComps >> (num*2)) & 3; }
      };
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
      static RegisterPtr AllocateRegister (SequenceBuilder& seqBuilder, const TypePtr& type,
                                           RegisterClassification classify,
				           const uc::String& name = uc::String ());
      /// Create a new generation of a register
      static RegisterPtr AllocateRegister (SequenceBuilder& seqBuilder, const RegisterPtr& oldReg);
      /** @} */
      
      void GenerateCast (SequenceBuilder& seqBuilder,
			 const RegisterPtr& castDestination,
			 const TypeImplPtr& typeDestination,
			 const RegisterPtr& castSource,
			 const TypeImplPtr& typeSource);

      /// Mark the program as “completed”. Required before GetProgram() is called
      void CompleteProgram ();
      ProgramPtr GetProgram (const uc::String& entryFunctionName);

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
      ExpressionPtr CreateConstNumericExpression (const uc::String& valueStr);
      ExpressionPtr CreateVariableExpression (NamePtr name);
      ExpressionPtr CreateAttributeAccess (ExpressionPtr expr,
					   const uc::String& attr);
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
