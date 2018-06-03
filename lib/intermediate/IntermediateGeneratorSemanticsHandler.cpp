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
#include "diagnostics/Handler.h"
#include "intermediate/Diagnostics.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceOp/SequenceOpCast.h"
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"

#include "BlockImpl.h"
#include "FunctionCallGlobalVarAugment.h"
#include "NameImpl.h"
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

#include "base/format/Formatter.tpp"

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
      invalidType = boost::shared_ptr<TypeImpl> (new TypeImpl (semantics::BaseType::Invalid));
      voidType = boost::shared_ptr<TypeImpl> (new TypeImpl (semantics::BaseType::Void));
      boolType = boost::shared_ptr<TypeImpl> (new TypeImpl (semantics::BaseType::Bool));
      intType = boost::shared_ptr<TypeImpl> (new TypeImpl (semantics::BaseType::Int));
      uintType = boost::shared_ptr<TypeImpl> (new TypeImpl (semantics::BaseType::UInt));
      floatType = boost::shared_ptr<TypeImpl> (new TypeImpl (semantics::BaseType::Float));
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

    std::string IntermediateGeneratorSemanticsHandler::GetTypeString (const TypeImplPtr& type)
    {
      switch (type->typeClass)
      {
        case TypeImpl::Base:
        {
          return GetBaseTypeString (type->base, 0, 0);
        }
        break;
      case TypeImpl::Sampler:
        {
          switch (type->sampler)
          {
            case semantics::SamplerType::_1D: return "S1";
            case semantics::SamplerType::_2D: return "S2";
            case semantics::SamplerType::_3D: return "S3";
            case semantics::SamplerType::CUBE: return "SC";
          }
        }
        break;
      case TypeImpl::Array:
        {
          std::string s;
          FormatTSArray (s, GetTypeString (boost::static_pointer_cast<TypeImpl> (type->avmBase)));
          return s;
        }
      case TypeImpl::Vector:
        {
          auto compType = boost::static_pointer_cast<TypeImpl> (type->avmBase);
          S1_ASSERT (compType->typeClass == TypeImpl::Base, std::string());
          return GetBaseTypeString (compType->base, type->vectorDim, 0);
        }
      case TypeImpl::Matrix:
        {
          auto compType = boost::static_pointer_cast<TypeImpl> (type->avmBase);
          S1_ASSERT (compType->typeClass == TypeImpl::Base, std::string());
          return GetBaseTypeString (compType->base, type->matrixRows, type->matrixCols);
        }
      }
      S1_ASSERT (false, std::string());
      return std::string ();
    }

    std::string IntermediateGeneratorSemanticsHandler::GetTypeString (const TypePtr& type)
    {
      return GetTypeString (boost::static_pointer_cast<TypeImpl> (type));
    }

    boost::shared_ptr<IntermediateGeneratorSemanticsHandler::TypeImpl>
    IntermediateGeneratorSemanticsHandler::GetHigherPrecisionType (
      const boost::shared_ptr<TypeImpl>& t1, const boost::shared_ptr<TypeImpl>& t2)
    {
      return boost::static_pointer_cast<TypeImpl> (CommonSemanticsHandler::GetHigherPrecisionType (t1, t2));
    }

    IntermediateGeneratorSemanticsHandler::TypeImplPtr
    IntermediateGeneratorSemanticsHandler::GetAttributeType (const TypeImplPtr& expressionType,
                                                             const Attribute& attr)
    {
      return boost::static_pointer_cast<TypeImpl> (CommonSemanticsHandler::GetAttributeType (expressionType, attr));
    }

    DECLARE_STATIC_FORMATTER(FormatRegPrefixName, "{0}{1}");
    DECLARE_STATIC_FORMATTER(FormatRegTmp, "{0}tmp{1}");

    SequencePtr IntermediateGeneratorSemanticsHandler::CreateGlobalVarInitializationSeq (NameImplSet& exportedNames)
    {
      // Create a block with all assignments
      BlockPtr globalsInitBlock (CreateBlock (globalScope));

      const std::vector<NamePtr>& globalVars (globalScope->GetAllVars());
      for (auto global : globalVars)
      {
        boost::shared_ptr<NameImpl> nameImpl (boost::static_pointer_cast<NameImpl> (global));
        if (nameImpl->varValue)
        {
          // Synthesize an expression to assign the global with the default value
          ExpressionPtr nameExpr (CreateVariableExpression (nameImpl));
          ExpressionPtr assignExpr (CreateAssignExpression (nameExpr, nameImpl->varValue));
          globalsInitBlock->AddExpressionCommand (assignExpr);
        }
      }

      boost::shared_ptr<BlockImpl> blockImpl (boost::static_pointer_cast<BlockImpl> (globalsInitBlock));
      blockImpl->Finish();

      exportedNames = blockImpl->GetExportedNames();
      return blockImpl->GetSequence();
    }

    ProgramFunctionPtr IntermediateGeneratorSemanticsHandler::SynthesizeEntryFunction (const uc::String& realEntryIdentifier,
                                                                                       const TypePtr& returnType,
                                                                                       const semantics::Scope::FunctionFormalParameters& params)
    {
      std::vector<NamePtr> globalVars (globalScope->GetAllVars ());
      SequencePtr entryFuncSeq;

      // Generate a call to real entry function, forwarding parameters
      {
        SequenceBuilderPtr entrySeqBuilder (boost::make_shared<SequenceBuilder> ());
        std::vector<RegisterPtr> inParams;
        std::vector<RegisterPtr> outParams;
        for (const auto& param : params)
        {
          RegisterPtr paramReg;
          if ((param.dir & semantics::Scope::dirIn) != 0)
          {
            paramReg = AllocateRegister (*entrySeqBuilder, param.type, Imported, param.identifier);
            entrySeqBuilder->SetImport (paramReg, param.identifier);
            inParams.push_back (paramReg);
          }
          if ((param.dir & semantics::Scope::dirOut) != 0)
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
        NameImplSet initSeqExported;
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
                                                         const TypeImplPtr& typeDestination,
                                                         const RegisterPtr& castSource,
                                                         const TypeImplPtr& typeSource)
    {
      switch (typeDestination->typeClass)
      {
        case TypeImpl::Base:
        {
          SequenceOpPtr seqOp;
          switch (typeDestination->base)
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
      case TypeImpl::Sampler:
        // Cannot cast samplers
        break;
      case TypeImpl::Array:
        // TODO: Cast individual elements, if lengths match
        break;
      case TypeImpl::Vector:
        // Special case: allow casting from a base type to a vector, replicate value across all components
        if (typeSource->typeClass == TypeImpl::Base)
        {
          std::vector<RegisterPtr> srcVec;
          TypeImplPtr destBaseType (boost::static_pointer_cast<TypeImpl> (typeDestination->avmBase));
          RegisterPtr srcReg;
          if (!destBaseType->IsEqual (*(typeSource)))
          {
            // Generate cast
            RegisterPtr srcVecReg (AllocateRegister (seqBuilder, destBaseType, Intermediate));
            auto innerCastResult = GenerateCast (seqBuilder, srcVecReg, destBaseType, castSource, typeSource);
            if (innerCastResult.has_error()) return innerCastResult.error();
            srcReg = srcVecReg;
          }
          else
            srcReg = castSource;
          srcVec.insert (srcVec.begin(), typeDestination->vectorDim, srcReg);
          // Generate "make vector" op
          SequenceOpPtr seqOp;
          switch (destBaseType->base)
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
        assert (typeDestination->vectorDim == typeSource->vectorDim);
        // TODO: Cast individual components
        break;
      case TypeImpl::Matrix:
        assert (typeDestination->matrixCols == typeSource->matrixCols);
        assert (typeDestination->matrixRows == typeSource->matrixRows);
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
        std::vector<NamePtr> globalVars (globalScope->GetAllVars ());
        ScopeImpl::FunctionInfoVector functions (globalScope->GetFunctions ());
        for (ScopeImpl::FunctionInfoVector::const_iterator funcIt = functions.begin();
             funcIt != functions.end();
             ++funcIt)
        {
          if ((*funcIt)->originalIdentifier == entryFunctionName)
          {
            if (!entryFunction)
            {
              entryFunction = *funcIt;
            }
            else
            {
              // TODO: Err if entry function is ambiguous
            }
          }

          boost::shared_ptr<BlockImpl> blockImpl (boost::static_pointer_cast<BlockImpl> ((*funcIt)->block));

          semantics::Scope::FunctionFormalParameters params ((*funcIt)->params);
          TypeImplPtr retTypeImpl (boost::static_pointer_cast<TypeImpl> ((*funcIt)->returnType));
          if (!voidType->IsEqual (*retTypeImpl))
          {
            semantics::Scope::FunctionFormalParameter retParam;
            retParam.type = (*funcIt)->returnType;
            retParam.identifier = BlockImpl::varReturnValueName;
            retParam.dir = semantics::Scope::dirOut;
            params.insert (params.begin(), retParam);
          }

          SequencePtr funcSeq (blockImpl->GetSequence ());

          {
            size_t inputInsertPos = 0;
            while (inputInsertPos < params.size())
            {
              if (params[inputInsertPos].dir != semantics::Scope::dirIn) break;
              inputInsertPos++;
            }
            // Augment parameters list with global vars
            for (const NamePtr& globalName : globalVars)
            {
              NameImplPtr global (boost::static_pointer_cast<NameImpl> (globalName));
              {
                semantics::Scope::FunctionFormalParameter inParam;
                inParam.paramType = semantics::Scope::ptAutoGlobal;
                inParam.type = global->valueType;
                inParam.identifier = global->identifier;
                inParam.dir = semantics::Scope::dirIn;
                params.insert (boost::next (params.begin(), inputInsertPos), inParam);
                inputInsertPos++;
              }
              if (!global->varConstant)
              {
                // TODO: Better handling of constants (no need to pass them as params)
                semantics::Scope::FunctionFormalParameter outParam;
                outParam.paramType = semantics::Scope::ptAutoGlobal;
                outParam.type = global->valueType;
                outParam.identifier = global->identifier;
                outParam.dir = semantics::Scope::dirOut;
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

          ProgramFunctionPtr newFunc (boost::make_shared <ProgramFunction> ((*funcIt)->originalIdentifier,
                                                                            (*funcIt)->identifier,
                                                                            params,
                                                                            funcSeq,
                                                                            false));
          newProg->AddFunction (newFunc);
        }
        // TODO: Err if no entry function was given
        if (entryFunction)
        {
          newProg->AddFunction (SynthesizeEntryFunction (entryFunction->identifier,
                                                         entryFunction->returnType,
                                                         entryFunction->params));
        }
      }
      return newProg;
    }

    semantics::TypePtr IntermediateGeneratorSemanticsHandler::CreateType (semantics::BaseType type)
    {
      switch (type)
      {
      case semantics::BaseType::Invalid: return invalidType;
      case semantics::BaseType::Void: return voidType;
      case semantics::BaseType::Bool: return boolType;
      case semantics::BaseType::Int: return intType;
      case semantics::BaseType::UInt: return uintType;
      case semantics::BaseType::Float: return floatType;
      }
      S1_ASSERT_NOT_REACHED (TypePtr());
    }

    semantics::TypePtr IntermediateGeneratorSemanticsHandler::CreateSamplerType (semantics::SamplerType dim)
    {
      return semantics::TypePtr (new TypeImpl (dim));
    }

    semantics::TypePtr IntermediateGeneratorSemanticsHandler::CreateArrayType (semantics::TypePtr baseType)
    {
      return semantics::TypePtr (new TypeImpl (baseType));
    }

    semantics::TypePtr IntermediateGeneratorSemanticsHandler::CreateVectorType (semantics::TypePtr baseType,
                                                                                unsigned int components)
    {
      return semantics::TypePtr (new TypeImpl (baseType, components));
    }

    semantics::TypePtr IntermediateGeneratorSemanticsHandler::CreateMatrixType (semantics::TypePtr baseType,
                                                                                unsigned int columns,
                                                                                unsigned int rows)
    {
      return semantics::TypePtr (new TypeImpl (baseType, columns, rows));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateConstBoolExpression (bool value)
    {
      return ExpressionPtr (new BoolExpressionImpl (this, ExpressionContext(), value));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateConstNumericExpression (const uc::String& valueStr)
    {
      return ExpressionPtr (new NumericExpressionImpl (this, ExpressionContext(), valueStr));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateVariableExpression (NamePtr name)
    {
      boost::shared_ptr<NameImpl> nameImpl (boost::static_pointer_cast<NameImpl> (name));
      switch (nameImpl->GetType())
      {
      case semantics::Name::Variable:
        {
          return ExpressionPtr (new VariableExpressionImpl (this, ExpressionContext(), nameImpl));
        }
        break;
      default:
        // TODO: Throw or so
        break;
      }
      return ExpressionPtr();
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateAttributeAccess (ExpressionPtr expr,
                                                                                const uc::String& attr)
    {
      if (!expr) return ExpressionPtr(); // Assume error already handled
      Attribute attrInfo (IdentifyAttribute (attr));
      if (attrInfo.attrClass == Attribute::Unknown)
      {
        ExpressionError (ExpressionContext(), Error::InvalidAttribute);
        return ExpressionPtr();
      }
      return boost::make_shared<AttributeExpressionImpl> (this, ExpressionContext(), expr, attrInfo);
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateArrayElementAccess (ExpressionPtr arrayExpr,
                                                                                   ExpressionPtr elementIndexExpr)
    {
      if (!arrayExpr || !elementIndexExpr) return ExpressionPtr(); // Assume error already handled
      return boost::make_shared<ArrayElementExpressionImpl> (this, ExpressionContext(), arrayExpr, elementIndexExpr);
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateAssignExpression (ExpressionPtr target,
                                          ExpressionPtr value)
    {
      if (!target || !value) return ExpressionPtr(); // Assume error already handled
      return ExpressionPtr (new AssignmentExpressionImpl (this,
                                                          ExpressionContext(),
                                                          boost::static_pointer_cast<ExpressionImpl> (target),
                                                          boost::static_pointer_cast<ExpressionImpl> (value)));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateArithmeticExpression (ArithmeticOp op,
                                                                                     ExpressionPtr operand1,
                                                                                     ExpressionPtr operand2)
    {
      if (!operand1 || !operand2) return ExpressionPtr(); // Assume error already handled
      return ExpressionPtr (new ArithmeticExpressionImpl (this,
                                                          ExpressionContext(),
                                                          op,
                                                          boost::static_pointer_cast<ExpressionImpl> (operand1),
                                                          boost::static_pointer_cast<ExpressionImpl> (operand2)));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateUnaryExpression (UnaryOp op,
                                                                                ExpressionPtr operand)
    {
      if (!operand) return ExpressionPtr(); // Assume error already handled
      return boost::make_shared<UnaryExpressionImpl> (this, ExpressionContext(), op,
                                                      boost::static_pointer_cast<ExpressionImpl> (operand));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateTernaryExpression (ExpressionPtr condition,
                                                                                  ExpressionPtr ifExpr,
                                                                                  ExpressionPtr thenExpr)
    {
      if (!condition || !ifExpr || !thenExpr) return ExpressionPtr(); // Assume error already handled
      return boost::make_shared<TernaryExpressionImpl> (this,
                                                        ExpressionContext(),
                                                        boost::static_pointer_cast<ExpressionImpl> (condition),
                                                        boost::static_pointer_cast<ExpressionImpl> (ifExpr),
                                                        boost::static_pointer_cast<ExpressionImpl> (thenExpr));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateComparisonExpression (CompareOp op,
                                                                                     ExpressionPtr operand1,
                                                                                     ExpressionPtr operand2)
    {
      if (!operand1 || !operand2) return ExpressionPtr(); // Assume error already handled
      return ExpressionPtr (
        boost::make_shared<ComparisonExpressionImpl> (this,
                                                      ExpressionContext(),
                                                      op,
                                                      boost::static_pointer_cast<ExpressionImpl> (operand1),
                                                      boost::static_pointer_cast<ExpressionImpl> (operand2)));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateLogicExpression (LogicOp op,
                                                                                ExpressionPtr operand1,
                                                                                ExpressionPtr operand2)
    {
      if (!operand1 || !operand2) return ExpressionPtr(); // Assume error already handled
      return ExpressionPtr (
        boost::make_shared<LogicExpressionImpl> (this,
                                                 ExpressionContext(),
                                                 op,
                                                 boost::static_pointer_cast<ExpressionImpl> (operand1),
                                                 boost::static_pointer_cast<ExpressionImpl> (operand2)));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateFunctionCallExpression (NamePtr functionName,
                                                                                       const ExpressionVector& params)
    {
      assert (functionName->GetType() == semantics::Name::Function);
      for (const auto& param : params)
      {
        if (!param) return ExpressionPtr(); // Assume error already handled
      }

      return ExpressionPtr (
        boost::make_shared<FunctionCallExpressionImpl> (this,
                                                        ExpressionContext(),
                                                        functionName,
                                                        params));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateTypeConstructorExpression (TypePtr type,
                                                                                          const ExpressionVector& params)
    {
      for (const auto& param : params)
      {
        if (!param) return ExpressionPtr(); // Assume error already handled
      }

      return ExpressionPtr (
        boost::make_shared<TypeConstructorExpressionImpl> (this,
                                                           ExpressionContext(),
                                                           boost::static_pointer_cast<TypeImpl> (type),
                                                           params));
    }

    ScopePtr IntermediateGeneratorSemanticsHandler::CreateScope (ScopePtr parentScope,
                                                                 semantics::ScopeLevel scopeLevel,
                                                                 const TypePtr& funcReturnType)
    {
      ScopeImplPtr newScope (boost::make_shared<ScopeImpl> (this,
                                                            boost::static_pointer_cast<ScopeImpl> (parentScope),
                                                            scopeLevel,
                                                            funcReturnType));
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
      return BlockPtr (new BlockImpl (this, blockScope));
    }

  } // namespace intermediate
} // namespace s1
