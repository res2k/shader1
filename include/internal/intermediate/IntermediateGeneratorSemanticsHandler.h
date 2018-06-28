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

#include "diagnostics/forwarddecl.h"
#include "semantics/CommonSemanticsHandler.h"
#include "semantics/CommonType.h"

#include "forwarddecl.h"
#include "Sequence.h"

#include <outcome/outcome.hpp>

namespace s1
{
  namespace intermediate
  {
    enum struct Error : unsigned int;

    class IntermediateGeneratorSemanticsHandler :
      public semantics::CommonSemanticsHandler
    {
    protected:
      friend class ProgramFunction;

      typedef semantics::ExpressionPtr ExpressionPtr;

      typedef semantics::CommonType TypeImpl;
      struct NameImpl;
      class FunctionImpl;
      class ScopeImpl;
      class BlockImpl;

      class Builtin;
      typedef boost::intrusive_ptr<Builtin> BuiltinPtr;

      /// Context information for expression objects. Used for error/warning reporting.
      struct ExpressionContext
      {
      };

      class ExpressionImpl;
      class ArithmeticExpressionImpl;
      class ArrayElementExpressionImpl;
      class AssignmentExpressionImpl;
      class AttributeExpressionImpl;
      class BinaryExpressionImpl;
      class BoolExpressionImpl;
      class ComparisonExpressionImpl;
      class FunctionCallExpressionImpl;
      class LogicExpressionImpl;
      class NumericExpressionImpl;
      class TernaryExpressionImpl;
      class TypeConstructorExpressionImpl;
      class UnaryExpressionImpl;
      class VariableExpressionImpl;

      typedef boost::intrusive_ptr<NameImpl> NameImplPtr;
      typedef std::unordered_set<NameImplPtr> NameImplSet;

      /// Diagnostics handler
      diagnostics::Handler* diagnosticsHandler = nullptr;

      typedef boost::intrusive_ptr<TypeImpl> TypeImplPtr;
      TypeImplPtr invalidType;
      TypeImplPtr voidType;
      TypeImplPtr boolType;
      TypeImplPtr intType;
      TypeImplPtr uintType;
      TypeImplPtr floatType;

      static std::string GetTypeString (TypeImpl* type);

      typedef boost::intrusive_ptr<ScopeImpl> ScopeImplPtr;
      ScopeImplPtr builtinScope;
      ScopeImplPtr globalScope;

      // Helper functions for SetupBuiltins
      semantics::Scope::FunctionFormalParameters MakeFormalParameters1 (const semantics::TypePtr& type);
      semantics::Scope::FunctionFormalParameters MakeFormalParameters2 (const semantics::TypePtr& type);
      semantics::Scope::FunctionFormalParameters MakeFormalParameters2 (const semantics::TypePtr& typeA,
                                                                        const semantics::TypePtr& typeB);
      void SetupBuiltins (const ScopeImplPtr& scope);

      /// Create a sequence containing global vars initialization
      SequencePtr CreateGlobalVarInitializationSeq (NameImplSet& exportedNames);

      ProgramFunctionPtr SynthesizeEntryFunction (const uc::String& realEntryIdentifier,
                                                  const semantics::TypePtr& returnType,
                                                  const semantics::Scope::FunctionFormalParameters& params);

      /// Whether the intermediate program was marked "completed"
      bool completed;
    public:
      /**\name Type utilities
       * @{ */
      static TypeImpl* GetHigherPrecisionType (TypeImpl* t1, TypeImpl* t2);
      static std::string GetTypeString (semantics::Type* type);
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
      TypeImplPtr GetAttributeType (TypeImpl* expressionType,
                                    const Attribute& attr);
      /** @} */

      IntermediateGeneratorSemanticsHandler ();
      ~IntermediateGeneratorSemanticsHandler ();

      /// Set diagnostics handler
      void SetDiagnosticsHandler (diagnostics::Handler* diagnosticsHandler)
      { this->diagnosticsHandler = diagnosticsHandler; }

      /**\name Error handling
       * @{ */
      // TODO: void Warning ()
      /// Signal an error in intermediate generation
      void ExpressionError (const ExpressionContext& context, Error code);
      /** @} */

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
      static RegisterPtr AllocateRegister (SequenceBuilder& seqBuilder,
                                           const semantics::TypePtr& type,
                                           RegisterClassification classify,
                                           const uc::String& name = uc::String ());
      /// Create a new generation of a register
      static RegisterPtr AllocateRegister (SequenceBuilder& seqBuilder, const RegisterPtr& oldReg);
      /** @} */

      typedef OUTCOME_V2_NAMESPACE::result<void, Error> result_void;
      result_void GenerateCast (SequenceBuilder& seqBuilder,
                                const RegisterPtr& castDestination,
                                const TypeImplPtr& typeDestination,
                                const RegisterPtr& castSource,
                                const TypeImplPtr& typeSource);

      /// Mark the program as “completed”. Required before GetProgram() is called
      void CompleteProgram ();
      ProgramPtr GetProgram (const uc::String& entryFunctionName);

      /**\name s1::parser::SemanticsHandler implementation
       * @{ */
      semantics::TypePtr CreateType (semantics::BaseType type);
      semantics::TypePtr CreateSamplerType (semantics::SamplerType dim);
      semantics::TypePtr CreateArrayType (semantics::TypePtr baseType);
      semantics::TypePtr CreateVectorType (semantics::TypePtr baseType,
                                           unsigned int components);
      semantics::TypePtr CreateMatrixType (semantics::TypePtr baseType,
                                           unsigned int columns,
                                           unsigned int rows);

      ExpressionPtr CreateConstBoolExpression (bool value);
      ExpressionPtr CreateConstNumericExpression (const uc::String& valueStr);
      ExpressionPtr CreateVariableExpression (semantics::NamePtr name);
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

      ExpressionPtr CreateFunctionCallExpression (semantics::NamePtr functionName,
                                                  const ExpressionVector& params);

      ExpressionPtr CreateTypeConstructorExpression (semantics::TypePtr type,
                                                     const ExpressionVector& params);

      semantics::ScopePtr CreateScope (semantics::ScopePtr parentScope, semantics::ScopeLevel scopeLevel,
                                       const semantics::TypePtr& returnType);
      semantics::ScopePtr CreateScope (semantics::ScopePtr parentScope, semantics::ScopeLevel scopeLevel)
      { return CreateScope (parentScope, scopeLevel, semantics::TypePtr ()); }

      semantics::BlockPtr CreateBlock (semantics::ScopePtr parentScope);
      /** @} */
    };

  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_INTERMEDIATEGENERATORSEMANTICSHANDLER_H__
