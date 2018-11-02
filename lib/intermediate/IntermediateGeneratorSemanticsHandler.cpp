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

#include "base/common.h"

#include "base/format/Formatter.h"
#include "base/format/std_string.h"
#include "base/format/uc_String.h"
#include "base/intrusive_ptr.h"
#include "diagnostics/Handler.h"
#include "intermediate/Diagnostics.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceOp/SequenceOpCast.h"
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"
#include "semantics/Function.h"
#include "semantics/Name.h"

#include "BlockImpl.h"
#include "FunctionCallGlobalVarAugment.h"
#include "PrependGlobalsInit.h"
#include "ScopeImpl.h"

#include "ArithmeticExpressionImpl.h"
#include "ArrayElementExpressionImpl.h"
#include "AssignmentExpressionImpl.h"
#include "AttributeExpressionImpl.h"
#include "BoolExpressionImpl.h"
#include "ComparisonExpressionImpl.h"
#include "FunctionCallExpressionImpl.h"
#include "LogicExpressionImpl.h"
#include "NumericExpressionImpl.h"
#include "TernaryExpressionImpl.h"
#include "TypeConstructorExpressionImpl.h"
#include "UnaryExpressionImpl.h"
#include "VariableExpressionImpl.h"

#include <boost/next_prior.hpp>
#include <boost/make_shared.hpp>

#include "base/format/Formatter.ipp"

namespace s1
{
  namespace intermediate
  {
    typedef semantics::NamePtr NamePtr;
    typedef semantics::FunctionPtr FunctionPtr;
    typedef semantics::ScopePtr ScopePtr;
    typedef semantics::BlockPtr BlockPtr;
    typedef semantics::TypePtr TypePtr;
    typedef semantics::ExpressionPtr ExpressionPtr;

    IntermediateGeneratorSemanticsHandler::IntermediateGeneratorSemanticsHandler () : completed (false)
    {
    }

    IntermediateGeneratorSemanticsHandler::~IntermediateGeneratorSemanticsHandler ()
    {
    }

    void IntermediateGeneratorSemanticsHandler::ExpressionError (const ExpressionContext& context, Error code)
    {
      if (diagnosticsHandler)
        diagnosticsHandler->SemanticError (code); // TODO: Do something sensible w/ context
    }

    DECLARE_STATIC_FORMATTER(FormatTSArray, "A{0}");

    static inline std::string GetBaseTypeString (semantics::BaseType base, unsigned int rows, unsigned int cols)
    {
      char typeStr[3] = { 0, 0, 0 };
      // 1st char: base type
      switch (base)
      {
      case semantics::BaseType::Invalid: typeStr[0] = 'X'; break;
      case semantics::BaseType::Void:    typeStr[0] = 'V'; break;
      case semantics::BaseType::Bool:    typeStr[0] = 'B'; break;
      case semantics::BaseType::Int:     typeStr[0] = 'I'; break;
      case semantics::BaseType::UInt:    typeStr[0] = 'U'; break;
      case semantics::BaseType::Float:   typeStr[0] = 'F'; break;
      }
      if ((rows == 0) && (cols == 0)) return typeStr;
      // 2nd char: encode dimensions
      assert (rows < 5);
      assert (cols < 5);
      typeStr[1] = 'a' + (rows + (cols * 5));
      return typeStr;
    }

    std::string IntermediateGeneratorSemanticsHandler::GetTypeString (semantics::Type* type)
    {
      switch (type->GetTypeClass())
      {
        case semantics::Type::Base:
        {
          return GetBaseTypeString (type->GetBaseType(), 0, 0);
        }
        break;
      case semantics::Type::Sampler:
        {
          switch (type->GetSamplerType())
          {
            case semantics::SamplerType::_1D: return "S1";
            case semantics::SamplerType::_2D: return "S2";
            case semantics::SamplerType::_3D: return "S3";
            case semantics::SamplerType::CUBE: return "SC";
          }
        }
        break;
      case semantics::Type::Array:
        {
          std::string s;
          FormatTSArray (s, GetTypeString (type->GetArrayBase()));
          return s;
        }
      case semantics::Type::Vector:
        {
          return GetBaseTypeString (type->GetVMBase(), type->GetVectorTypeComponents(), 0);
        }
      case semantics::Type::Matrix:
        {
          return GetBaseTypeString (type->GetVMBase(), type->GetMatrixTypeRows(), type->GetMatrixTypeCols());
        }
      }
      S1_ASSERT (false, std::string());
      return std::string ();
    }

    DECLARE_STATIC_FORMATTER(FormatRegPrefixName, "{0}{1}");
    DECLARE_STATIC_FORMATTER(FormatRegTmp, "{0}tmp{1}");

    SequencePtr IntermediateGeneratorSemanticsHandler::CreateGlobalVarInitializationSeq (NameVariableSet& exportedNames)
    {
      // Create a block with all assignments
      BlockPtr globalsInitBlock (CreateBlock (globalScope));

      const auto& globalVars = globalScope->GetAllVars();
      for (auto global : globalVars)
      {
        if (global->GetValue())
        {
          // Synthesize an expression to assign the global with the default value
          ExpressionPtr nameExpr (CreateVariableExpression (global.get()));
          ExpressionPtr assignExpr (CreateAssignExpression (nameExpr, global->GetValue()));
          globalsInitBlock->AddExpressionCommand (assignExpr);
        }
      }

      auto blockImpl = get_static_ptr<BlockImpl> (globalsInitBlock);
      blockImpl->Finish();

      exportedNames = blockImpl->GetExportedNames();
      return blockImpl->GetSequence();
    }

    ProgramFunctionPtr IntermediateGeneratorSemanticsHandler::SynthesizeEntryFunction (const uc::String& realEntryIdentifier,
                                                                                       const TypePtr& returnType,
                                                                                       const semantics::FunctionFormalParameters& params)
    {
      auto globalVars = globalScope->GetAllVars ();
      SequencePtr entryFuncSeq;

      // Generate a call to real entry function, forwarding parameters
      {
        SequenceBuilderPtr entrySeqBuilder (boost::make_shared<SequenceBuilder> ());
        std::vector<RegisterPtr> inParams;
        std::vector<RegisterPtr> outParams;
        for (const auto& param : params)
        {
          RegisterPtr paramReg;
          if ((param.dir & semantics::FunctionFormalParameter::dirIn) != 0)
          {
            paramReg = AllocateRegister (*entrySeqBuilder, param.type, Imported, param.identifier);
            entrySeqBuilder->SetImport (paramReg, param.identifier);
            inParams.push_back (paramReg);
          }
          if ((param.dir & semantics::FunctionFormalParameter::dirOut) != 0)
          {
            if (paramReg)
              paramReg = AllocateRegister (*entrySeqBuilder, paramReg);
            else
              paramReg = AllocateRegister (*entrySeqBuilder, param.type, Imported, param.identifier);
            entrySeqBuilder->SetExport (param.identifier, paramReg);
            outParams.push_back (paramReg);
          }
        }
        entrySeqBuilder->AddOp (new SequenceOpFunctionCall (realEntryIdentifier, inParams, outParams));
        entryFuncSeq = entrySeqBuilder->GetSequence ();
      }

      // Create global var initialization sequence
      {
        NameVariableSet initSeqExported;
        SequencePtr initSeq (CreateGlobalVarInitializationSeq (initSeqExported));

        SequenceBuilderPtr newSeqBuilder (boost::make_shared<SequenceBuilder> ());
        PrependGlobalsInit prependVisitor (newSeqBuilder, entryFuncSeq, initSeq);
        entryFuncSeq->Visit (prependVisitor);
        entryFuncSeq = newSeqBuilder->GetSequence ();
      }

      // Perform static var augmentation
      {
        // Create a new sequence...
        SequenceBuilderPtr newSeqBuilder (boost::make_shared<SequenceBuilder> ());
        // ...containing the original function sequence, augment with global var arguments
        {
          FunctionCallGlobalVarAugment augment (newSeqBuilder, entryFuncSeq,
                                                globalVars);
          entryFuncSeq->Visit (augment);
        }

        // Remove global variables from entry function imports
        for (const auto& global : globalVars)
        {
          newSeqBuilder->RemoveImport (global->GetIdentifier ());
        }

        entryFuncSeq = newSeqBuilder->GetSequence ();
      }

      ProgramFunctionPtr newFunc (boost::make_shared <ProgramFunction> ("$EntryFunction",
                                                                        "$EntryFunction",
                                                                        params,
                                                                        entryFuncSeq,
                                                                        true));
      return newFunc;
    }

    RegisterPtr IntermediateGeneratorSemanticsHandler::AllocateRegister (SequenceBuilder& seqBuilder,
                                                                         const TypePtr& type,
                                                                         RegisterClassification classify,
                                                                         const uc::String& name)
    {
      const uc::Char prefix[3] = { uc::Char (classify), '_', 0};
      uc::String regName;
      if (!name.isEmpty())
      {
        FormatRegPrefixName (regName, prefix, name);
      }
      else
      {
        static unsigned int allRegNum = 0;
        FormatRegTmp (regName, prefix, allRegNum++);
      }

      return seqBuilder.AllocateRegister (type, regName);
    }

    RegisterPtr IntermediateGeneratorSemanticsHandler::AllocateRegister (SequenceBuilder& seqBuilder,
                                                                         const RegisterPtr& oldReg)
    {
      return seqBuilder.AllocateRegister (oldReg);
    }

    IntermediateGeneratorSemanticsHandler::result_void
    IntermediateGeneratorSemanticsHandler::GenerateCast (SequenceBuilder& seqBuilder,
                                                         const RegisterPtr& castDestination,
                                                         semantics::Type* typeDestination,
                                                         const RegisterPtr& castSource,
                                                         semantics::Type* typeSource)
    {
      switch (typeDestination->GetTypeClass())
      {
        case semantics::Type::Base:
        {
          SequenceOpPtr seqOp;
          switch (typeDestination->GetBaseType())
          {
            case semantics::BaseType::Int:
              seqOp = SequenceOpPtr (new SequenceOpCast (castDestination, intermediate::BasicType::Int, castSource));
              break;
            case semantics::BaseType::UInt:
              seqOp = SequenceOpPtr (new SequenceOpCast (castDestination, intermediate::BasicType::UInt, castSource));
              break;
            case semantics::BaseType::Float:
              seqOp = SequenceOpPtr (new SequenceOpCast (castDestination, intermediate::BasicType::Float, castSource));
              break;
            default:
              // Void, Bool can't be casted
              break;
          }
          if (seqOp)
          {
            seqBuilder.AddOp (seqOp);
            return OUTCOME_V2_NAMESPACE::success();
          }
          break;
        }
      case semantics::Type::Sampler:
        // Cannot cast samplers
        break;
      case semantics::Type::Array:
        // TODO: Cast individual elements, if lengths match
        break;
      case semantics::Type::Vector:
        // Special case: allow casting from a base type to a vector, replicate value across all components
        if (typeSource->GetTypeClass() == semantics::Type::Base)
        {
          std::vector<RegisterPtr> srcVec;
          auto destBaseType = CreateType (typeDestination->GetVMBase());
          RegisterPtr srcReg;
          if (!destBaseType->IsEqual (*(typeSource)))
          {
            // Generate cast
            RegisterPtr srcVecReg (AllocateRegister (seqBuilder, destBaseType, Intermediate));
            auto innerCastResult = GenerateCast (seqBuilder, srcVecReg, destBaseType.get(), castSource, typeSource);
            if (innerCastResult.has_error()) return innerCastResult.error();
            srcReg = srcVecReg;
          }
          else
            srcReg = castSource;
          srcVec.insert (srcVec.begin(), typeDestination->GetVectorTypeComponents(), srcReg);
          // Generate "make vector" op
          SequenceOpPtr seqOp;
          switch (destBaseType->GetBaseType())
          {
            case semantics::BaseType::Bool:
              seqOp = new SequenceOpMakeVector (castDestination,
                                                intermediate::BasicType::Bool,
                                                srcVec);
              break;
            case semantics::BaseType::Int:
              seqOp = new SequenceOpMakeVector (castDestination,
                                                intermediate::BasicType::Int,
                                                srcVec);
              break;
            case semantics::BaseType::UInt:
              seqOp = new SequenceOpMakeVector (castDestination,
                                                intermediate::BasicType::UInt,
                                                srcVec);
              break;
            case semantics::BaseType::Float:
              seqOp = new SequenceOpMakeVector (castDestination,
                                                intermediate::BasicType::Float,
                                                srcVec);
              break;
            default:
              // Void can't be casted
              break;
          }
          if (seqOp)
          {
            seqBuilder.AddOp (seqOp);
            return OUTCOME_V2_NAMESPACE::success();
          }
          break;
        }
        assert (typeDestination->GetVectorTypeComponents() == typeSource->GetVectorTypeComponents());
        // TODO: Cast individual components
        break;
      case semantics::Type::Matrix:
        assert (typeDestination->GetMatrixTypeCols() == typeSource->GetMatrixTypeCols());
        assert (typeDestination->GetMatrixTypeRows() == typeSource->GetMatrixTypeRows());
        // TODO: Cast individual components
        break;
      }

      return Error::InvalidTypeCast;
    }

    void IntermediateGeneratorSemanticsHandler::CompleteProgram ()
    {
      S1_ASSERT(!completed, S1_ASSERT_RET_VOID);

      completed = true;
    }

    ProgramPtr IntermediateGeneratorSemanticsHandler::GetProgram (const uc::String& entryFunctionName)
    {
      S1_ASSERT(completed, ProgramPtr ());

      ProgramPtr newProg (boost::make_shared <Program> ());
      ScopeImpl::FunctionInfoPtr entryFunction;
      if (globalScope)
      {
        // Collect global vars
        auto globalVars = globalScope->GetAllVars ();
        ScopeImpl::FunctionInfoVector functions (globalScope->GetFunctions ());
        for (const auto& func : functions)
        {
          const auto& funcIdentifier = func->functionObj->GetName()->GetIdentifier();
          if (funcIdentifier == entryFunctionName)
          {
            if (!entryFunction)
            {
              entryFunction = func;
            }
            else
            {
              // TODO: Err if entry function is ambiguous
            }
          }

          auto blockImpl = get_static_ptr<BlockImpl> (func->functionObj->GetBody());

          semantics::FunctionFormalParameters params (func->functionObj->GetParameters());
          semantics::TypePtr retType = func->functionObj->GetReturnType();
          if (!voidType->IsEqual (*retType))
          {
            semantics::FunctionFormalParameter retParam;
            retParam.type = retType;
            retParam.identifier = BlockImpl::varReturnValueName;
            retParam.dir = semantics::FunctionFormalParameter::dirOut;
            params.insert (params.begin(), retParam);
          }

          SequencePtr funcSeq (blockImpl->GetSequence ());

          {
            size_t inputInsertPos = 0;
            while (inputInsertPos < params.size())
            {
              if (params[inputInsertPos].dir != semantics::FunctionFormalParameter::dirIn) break;
              inputInsertPos++;
            }
            // Augment parameters list with global vars
            for (const auto& global : globalVars)
            {
              {
                semantics::FunctionFormalParameter inParam;
                inParam.paramType = semantics::FunctionFormalParameter::ptAutoGlobal;
                inParam.type = global->GetValueType();
                inParam.identifier = global->GetIdentifier();
                inParam.dir = semantics::FunctionFormalParameter::dirIn;
                params.insert (boost::next (params.begin(), inputInsertPos), inParam);
                inputInsertPos++;
              }
              if (!global->IsConstant())
              {
                // TODO: Better handling of constants (no need to pass them as params)
                semantics::FunctionFormalParameter outParam;
                outParam.paramType = semantics::FunctionFormalParameter::ptAutoGlobal;
                outParam.type = global->GetValueType();
                outParam.identifier = global->GetIdentifier();
                outParam.dir = semantics::FunctionFormalParameter::dirOut;
                params.insert (params.end(), outParam);
              }
            }
          }

          /* Note: augmentation happens here because static var initialization may also
             call functions! */
          {
            // Create a new sequence...
            SequenceBuilderPtr newSeqBuilder (boost::make_shared<SequenceBuilder> ());
            // ...containing the original function sequence, augment with global var arguments
            {
              FunctionCallGlobalVarAugment augment (newSeqBuilder, funcSeq, globalVars);
              funcSeq->Visit (augment);
            }
            funcSeq = newSeqBuilder->GetSequence ();
          }

          ProgramFunctionPtr newFunc (boost::make_shared <ProgramFunction> (funcIdentifier,
                                                                            func->decoratedIdentifier,
                                                                            params,
                                                                            funcSeq,
                                                                            false));
          newProg->AddFunction (newFunc);
        }
        // TODO: Err if no entry function was given
        if (entryFunction)
        {
          auto entryFunctionName = entryFunction->functionObj->GetName();
          newProg->AddFunction (SynthesizeEntryFunction (entryFunctionName->GetIdentifier(),
                                                         entryFunction->functionObj->GetReturnType(),
                                                         entryFunction->functionObj->GetParameters()));
        }
      }
      return newProg;
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateConstBoolExpression (bool value)
    {
      return ExpressionPtr (new BoolExpressionImpl (this, ExpressionContext(), value));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateConstNumericExpression (const uc::String& valueStr)
    {
      return ExpressionPtr (new NumericExpressionImpl (this, ExpressionContext(), valueStr));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateVariableExpression (semantics::NameVariable* name)
    {
      return ExpressionPtr (new VariableExpressionImpl (this, ExpressionContext(), name));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateAttributeAccess (ExpressionPtr expr,
                                                                                const uc::String& attr)
    {
      if (!expr) return ExpressionPtr(); // Assume error already handled
      semantics::Attribute attrInfo (semantics::Attribute::Identify (attr));
      if (attrInfo.attrClass == semantics::Attribute::Unknown)
      {
        ExpressionError (ExpressionContext(), Error::InvalidAttribute);
        return ExpressionPtr();
      }
      return new AttributeExpressionImpl (this, ExpressionContext(), expr.get(), attrInfo);
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateArrayElementAccess (ExpressionPtr arrayExpr,
                                                                                   ExpressionPtr elementIndexExpr)
    {
      if (!arrayExpr || !elementIndexExpr) return ExpressionPtr(); // Assume error already handled
      return new ArrayElementExpressionImpl (this, ExpressionContext(), arrayExpr, elementIndexExpr);
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateAssignExpression (ExpressionPtr target,
                                          ExpressionPtr value)
    {
      if (!target || !value) return ExpressionPtr(); // Assume error already handled
      return new AssignmentExpressionImpl (this,
                                           ExpressionContext(),
                                           get_static_ptr<ExpressionImpl> (target),
                                           get_static_ptr<ExpressionImpl> (value));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateArithmeticExpression (ArithmeticOp op,
                                                                                     ExpressionPtr operand1,
                                                                                     ExpressionPtr operand2)
    {
      if (!operand1 || !operand2) return ExpressionPtr(); // Assume error already handled
      return new ArithmeticExpressionImpl (this,
                                           ExpressionContext(),
                                           op,
                                           get_static_ptr<ExpressionImpl> (operand1),
                                           get_static_ptr<ExpressionImpl> (operand2));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateUnaryExpression (UnaryOp op,
                                                                                ExpressionPtr operand)
    {
      if (!operand) return ExpressionPtr(); // Assume error already handled
      return new UnaryExpressionImpl (this, ExpressionContext(), op,
                                      get_static_ptr<ExpressionImpl> (operand));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateTernaryExpression (ExpressionPtr condition,
                                                                                  ExpressionPtr ifExpr,
                                                                                  ExpressionPtr thenExpr)
    {
      if (!condition || !ifExpr || !thenExpr) return ExpressionPtr(); // Assume error already handled
      return new TernaryExpressionImpl (this,
                                        ExpressionContext(),
                                        get_static_ptr<ExpressionImpl> (condition),
                                        get_static_ptr<ExpressionImpl> (ifExpr),
                                        get_static_ptr<ExpressionImpl> (thenExpr));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateComparisonExpression (CompareOp op,
                                                                                     ExpressionPtr operand1,
                                                                                     ExpressionPtr operand2)
    {
      if (!operand1 || !operand2) return ExpressionPtr(); // Assume error already handled
      return new ComparisonExpressionImpl (this,
                                           ExpressionContext(),
                                           op,
                                           get_static_ptr<ExpressionImpl> (operand1),
                                           get_static_ptr<ExpressionImpl> (operand2));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateLogicExpression (LogicOp op,
                                                                                ExpressionPtr operand1,
                                                                                ExpressionPtr operand2)
    {
      if (!operand1 || !operand2) return ExpressionPtr(); // Assume error already handled
      return new LogicExpressionImpl (this,
                                      ExpressionContext(),
                                      op,
                                      get_static_ptr<ExpressionImpl> (operand1),
                                      get_static_ptr<ExpressionImpl> (operand2));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateFunctionCallExpression (NamePtr functionName,
                                                                                       const ExpressionVector& params)
    {
      assert (functionName->GetType() == semantics::Name::Function);
      for (const auto& param : params)
      {
        if (!param) return ExpressionPtr(); // Assume error already handled
      }

      return new FunctionCallExpressionImpl (this,
                                             ExpressionContext(),
                                             functionName.get(),
                                             params);
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateTypeConstructorExpression (TypePtr type,
                                                                                          const ExpressionVector& params)
    {
      for (const auto& param : params)
      {
        if (!param) return ExpressionPtr(); // Assume error already handled
      }

      return new TypeConstructorExpressionImpl (this, ExpressionContext(), type.get(), params);
    }

    ScopePtr IntermediateGeneratorSemanticsHandler::CreateScope (ScopePtr parentScope,
                                                                 semantics::ScopeLevel scopeLevel,
                                                                 const TypePtr& funcReturnType)
    {
      auto newScope = make_intrusive<ScopeImpl> (this,
                                                 get_static_ptr<ScopeImpl> (parentScope),
                                                 scopeLevel,
                                                 funcReturnType.get());
      switch (scopeLevel)
      {
      case semantics::ScopeLevel::Builtin:
        builtinScope = newScope;
        SetupBuiltins (builtinScope);
        break;
      case semantics::ScopeLevel::Global:
        globalScope = newScope;
        break;
      default:
        break;
      }
      return newScope;
    }

    BlockPtr IntermediateGeneratorSemanticsHandler::CreateBlock (ScopePtr parentScope)
    {
      ScopePtr blockScope = CreateScope (parentScope, semantics::ScopeLevel::Function);
      return BlockPtr (new BlockImpl (this, blockScope.get()));
    }

  } // namespace intermediate
} // namespace s1
