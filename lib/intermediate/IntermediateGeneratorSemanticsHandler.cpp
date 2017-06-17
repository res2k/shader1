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
#include "intermediate/Exception.h"
#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceOp/SequenceOpCast.h"
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"

#include "parser/Exception.h"

#include "BlockImpl.h"
#include "FunctionCallGlobalVarAugment.h"
#include "NameImpl.h"
#include "PrependGlobalsInit.h"
#include "ScopeImpl.h"
#include "TypeImpl.h"

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

#include <boost/make_shared.hpp>
#include <boost/utility.hpp>

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace intermediate
  {
    typedef IntermediateGeneratorSemanticsHandler::NamePtr NamePtr;
    typedef IntermediateGeneratorSemanticsHandler::FunctionPtr FunctionPtr;
    typedef IntermediateGeneratorSemanticsHandler::ScopePtr ScopePtr;
    typedef IntermediateGeneratorSemanticsHandler::BlockPtr BlockPtr;
    typedef IntermediateGeneratorSemanticsHandler::TypePtr TypePtr;
    typedef IntermediateGeneratorSemanticsHandler::ExpressionPtr ExpressionPtr;

    IntermediateGeneratorSemanticsHandler::IntermediateGeneratorSemanticsHandler () : completed (false)
    {
      voidType = boost::shared_ptr<TypeImpl> (new TypeImpl (Void));
      boolType = boost::shared_ptr<TypeImpl> (new TypeImpl (Bool));
      intType = boost::shared_ptr<TypeImpl> (new TypeImpl (Int));
      uintType = boost::shared_ptr<TypeImpl> (new TypeImpl (UInt));
      floatType = boost::shared_ptr<TypeImpl> (new TypeImpl (Float));
    }

    IntermediateGeneratorSemanticsHandler::~IntermediateGeneratorSemanticsHandler ()
    {
    }

    DECLARE_STATIC_FORMATTER(FormatTSArray, "A{0}");

    static inline std::string GetBaseTypeString (parser::SemanticsHandler::BaseType base, unsigned int rows, unsigned int cols)
    {
      char typeStr[3] = { 0, 0, 0 };
      // 1st char: base type
      switch (base)
      {
      case parser::SemanticsHandler::Void:    typeStr[0] = 'V'; break;
      case parser::SemanticsHandler::Bool:    typeStr[0] = 'B'; break;
      case parser::SemanticsHandler::Int:     typeStr[0] = 'I'; break;
      case parser::SemanticsHandler::UInt:    typeStr[0] = 'U'; break;
      case parser::SemanticsHandler::Float:   typeStr[0] = 'F'; break;
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
            case _1D: return "S1";
            case _2D: return "S2";
            case _3D: return "S3";
            case CUBE: return "SC";
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
      blockImpl->FinishBlock();

      exportedNames = blockImpl->GetExportedNames();
      return blockImpl->GetSequence();
    }

    ProgramFunctionPtr IntermediateGeneratorSemanticsHandler::SynthesizeEntryFunction (const uc::String& realEntryIdentifier,
                                                                                       const TypePtr& returnType,
                                                                                       const Scope::FunctionFormalParameters& params)
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
          if ((param.dir & Scope::dirIn) != 0)
          {
            paramReg = AllocateRegister (*entrySeqBuilder, param.type, Imported, param.identifier);
            entrySeqBuilder->SetImport (paramReg, param.identifier);
            inParams.push_back (paramReg);
          }
          if ((param.dir & Scope::dirOut) != 0)
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

    void IntermediateGeneratorSemanticsHandler::GenerateCast (SequenceBuilder& seqBuilder,
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
            case Int:
              seqOp = SequenceOpPtr (new SequenceOpCast (castDestination, intermediate::Int, castSource));
              break;
            case UInt:
              seqOp = SequenceOpPtr (new SequenceOpCast (castDestination, intermediate::UInt, castSource));
              break;
            case Float:
              seqOp = SequenceOpPtr (new SequenceOpCast (castDestination, intermediate::Float, castSource));
              break;
            default:
              // Void, Bool can't be casted
              break;
          }
          if (seqOp)
          {
            seqBuilder.AddOp (seqOp);
            return;
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
            GenerateCast (seqBuilder, srcVecReg, destBaseType, castSource, typeSource);
            srcReg = srcVecReg;
          }
          else
            srcReg = castSource;
          srcVec.insert (srcVec.begin(), typeDestination->vectorDim, srcReg);
          // Generate "make vector" op
          SequenceOpPtr seqOp;
          switch (destBaseType->base)
          {
            case Bool:
              seqOp = new SequenceOpMakeVector (castDestination,
                                                intermediate::Bool,
                                                srcVec);
              break;
            case Int:
              seqOp = new SequenceOpMakeVector (castDestination,
                                                intermediate::Int,
                                                srcVec);
              break;
            case UInt:
              seqOp = new SequenceOpMakeVector (castDestination,
                                                intermediate::UInt,
                                                srcVec);
              break;
            case Float:
              seqOp = new SequenceOpMakeVector (castDestination,
                                                intermediate::Float,
                                                srcVec);
              break;
            default:
              // Void can't be casted
              break;
          }
          if (seqOp)
          {
            seqBuilder.AddOp (seqOp);
            return;
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

      throw Exception (InvalidTypeCast);
    }

    void IntermediateGeneratorSemanticsHandler::CompleteProgram ()
    {
      S1_ASSERT(!completed, S1_ASSERT_RET_VOID);

      if (globalScope)
      {
        ScopeImpl::FunctionInfoVector functions (globalScope->GetFunctions());
        for (ScopeImpl::FunctionInfoVector::const_iterator funcIt = functions.begin();
             funcIt != functions.end();
             ++funcIt)
        {
          if ((*funcIt)->builtin) continue;

          boost::shared_ptr<BlockImpl> blockImpl (boost::static_pointer_cast<BlockImpl> ((*funcIt)->block));
          blockImpl->FinishBlock();
        }
      }

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

          parser::SemanticsHandler::Scope::FunctionFormalParameters params ((*funcIt)->params);
          TypeImplPtr retTypeImpl (boost::static_pointer_cast<TypeImpl> ((*funcIt)->returnType));
          if (!voidType->IsEqual (*retTypeImpl))
          {
            parser::SemanticsHandler::Scope::FunctionFormalParameter retParam;
            retParam.type = (*funcIt)->returnType;
            retParam.identifier = BlockImpl::varReturnValueName;
            retParam.dir = parser::SemanticsHandler::Scope::dirOut;
            params.insert (params.begin(), retParam);
          }

          SequencePtr funcSeq (blockImpl->GetSequence ());

          {
            size_t inputInsertPos = 0;
            while (inputInsertPos < params.size())
            {
              if (params[inputInsertPos].dir != parser::SemanticsHandler::Scope::dirIn) break;
              inputInsertPos++;
            }
            // Augment parameters list with global vars
            for (const NamePtr& globalName : globalVars)
            {
              NameImplPtr global (boost::static_pointer_cast<NameImpl> (globalName));
              {
                parser::SemanticsHandler::Scope::FunctionFormalParameter inParam;
                inParam.paramType = parser::SemanticsHandler::Scope::ptAutoGlobal;
                inParam.type = global->valueType;
                inParam.identifier = global->identifier;
                inParam.dir = parser::SemanticsHandler::Scope::dirIn;
                params.insert (boost::next (params.begin(), inputInsertPos), inParam);
                inputInsertPos++;
              }
              if (!global->varConstant)
              {
                // TODO: Better handling of constants (no need to pass them as params)
                parser::SemanticsHandler::Scope::FunctionFormalParameter outParam;
                outParam.paramType = parser::SemanticsHandler::Scope::ptAutoGlobal;
                outParam.type = global->valueType;
                outParam.identifier = global->identifier;
                outParam.dir = parser::SemanticsHandler::Scope::dirOut;
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

    TypePtr IntermediateGeneratorSemanticsHandler::CreateType (BaseType type)
    {
      switch (type)
      {
      case Void: return voidType;
      case Bool: return boolType;
      case Int: return intType;
      case UInt: return uintType;
      case Float: return floatType;
      }
      S1_ASSERT_NOT_REACHED (TypePtr());
    }

    TypePtr IntermediateGeneratorSemanticsHandler::CreateSamplerType (SamplerType dim)
    {
      return TypePtr (new TypeImpl (dim));
    }

    TypePtr IntermediateGeneratorSemanticsHandler::CreateArrayType (TypePtr baseType)
    {
      return TypePtr (new TypeImpl (baseType));
    }

    TypePtr IntermediateGeneratorSemanticsHandler::CreateVectorType (TypePtr baseType,
                                                      unsigned int components)
    {
      return TypePtr (new TypeImpl (baseType, components));
    }

    TypePtr IntermediateGeneratorSemanticsHandler::CreateMatrixType (TypePtr baseType,
                                                      unsigned int columns,
                                                      unsigned int rows)
    {
      return TypePtr (new TypeImpl (baseType, columns, rows));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateConstBoolExpression (bool value)
    {
      return ExpressionPtr (new BoolExpressionImpl (this, value));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateConstNumericExpression (const uc::String& valueStr)
    {
      return ExpressionPtr (new NumericExpressionImpl (this, valueStr));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateVariableExpression (NamePtr name)
    {
      boost::shared_ptr<NameImpl> nameImpl (boost::static_pointer_cast<NameImpl> (name));
      switch (nameImpl->GetType())
      {
      case Name::Variable:
        {
          return ExpressionPtr (new VariableExpressionImpl (this, nameImpl));
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
      Attribute attrInfo (IdentifyAttribute (attr));
      if (attrInfo.attrClass == Attribute::Unknown)
        throw Exception (InvalidAttribute);
      return boost::make_shared<AttributeExpressionImpl> (this, expr, attrInfo);
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateArrayElementAccess (ExpressionPtr arrayExpr,
                                                                                   ExpressionPtr elementIndexExpr)
    {
      return boost::make_shared<ArrayElementExpressionImpl> (this, arrayExpr, elementIndexExpr);
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateAssignExpression (ExpressionPtr target,
                                          ExpressionPtr value)
    {
      return ExpressionPtr (new AssignmentExpressionImpl (this,
                                                          boost::static_pointer_cast<ExpressionImpl> (target),
                                                          boost::static_pointer_cast<ExpressionImpl> (value)));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateArithmeticExpression (ArithmeticOp op,
                                                                                     ExpressionPtr operand1,
                                                                                     ExpressionPtr operand2)
    {
      return ExpressionPtr (new ArithmeticExpressionImpl (this,
                                                          op,
                                                          boost::static_pointer_cast<ExpressionImpl> (operand1),
                                                          boost::static_pointer_cast<ExpressionImpl> (operand2)));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateUnaryExpression (UnaryOp op,
                                                                                ExpressionPtr operand)
    {
      return boost::make_shared<UnaryExpressionImpl> (this, op,
                                                      boost::static_pointer_cast<ExpressionImpl> (operand));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateTernaryExpression (ExpressionPtr condition,
                                                                                  ExpressionPtr ifExpr,
                                                                                  ExpressionPtr thenExpr)
    {
      return boost::make_shared<TernaryExpressionImpl> (this,
                                                        boost::static_pointer_cast<ExpressionImpl> (condition),
                                                        boost::static_pointer_cast<ExpressionImpl> (ifExpr),
                                                        boost::static_pointer_cast<ExpressionImpl> (thenExpr));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateComparisonExpression (CompareOp op,
                                                                                     ExpressionPtr operand1,
                                                                                     ExpressionPtr operand2)
    {
      return ExpressionPtr (
        boost::make_shared<ComparisonExpressionImpl> (this,
                                                      op,
                                                      boost::static_pointer_cast<ExpressionImpl> (operand1),
                                                      boost::static_pointer_cast<ExpressionImpl> (operand2)));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateLogicExpression (LogicOp op,
                                                                                ExpressionPtr operand1,
                                                                                ExpressionPtr operand2)
    {
      return ExpressionPtr (
        boost::make_shared<LogicExpressionImpl> (this,
                                                 op,
                                                 boost::static_pointer_cast<ExpressionImpl> (operand1),
                                                 boost::static_pointer_cast<ExpressionImpl> (operand2)));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateFunctionCallExpression (NamePtr functionName,
                                                                                       const ExpressionVector& params)
    {
      assert (functionName->GetType() == SemanticsHandler::Name::Function);

      return ExpressionPtr (
        boost::make_shared<FunctionCallExpressionImpl> (this,
                                                        functionName,
                                                        params));
    }

    ExpressionPtr IntermediateGeneratorSemanticsHandler::CreateTypeConstructorExpression (TypePtr type,
                                                                                          const ExpressionVector& params)
    {
      return ExpressionPtr (
        boost::make_shared<TypeConstructorExpressionImpl> (this,
                                                           boost::static_pointer_cast<TypeImpl> (type),
                                                           params));
    }

    ScopePtr IntermediateGeneratorSemanticsHandler::CreateScope (ScopePtr parentScope,
                                                                 ScopeLevel scopeLevel,
                                                                 const TypePtr& funcReturnType)
    {
      ScopeImplPtr newScope (boost::make_shared<ScopeImpl> (this,
                                                            boost::static_pointer_cast<ScopeImpl> (parentScope),
                                                            scopeLevel,
                                                            funcReturnType));
      switch (scopeLevel)
      {
      case SemanticsHandler::Builtin:
        builtinScope = newScope;
        SetupBuiltins (builtinScope);
        break;
      case Global:
        globalScope = newScope;
        break;
      default:
        break;
      }
      return newScope;
    }

    BlockPtr IntermediateGeneratorSemanticsHandler::CreateBlock (ScopePtr parentScope)
    {
      ScopePtr blockScope = CreateScope (parentScope, Function);
      return BlockPtr (new BlockImpl (this, blockScope));
    }

  } // namespace intermediate
} // namespace s1
