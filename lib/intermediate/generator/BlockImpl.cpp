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

#include "BlockImpl.h"

#include "base/format/Formatter.h"
#include "base/format/std_string.h"
#include "base/format/uc_String.h"
#include "intermediate/Exception.h"
#include "intermediate/SequenceBuilder.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceOp/SequenceOpBranch.h"
#include "intermediate/SequenceOp/SequenceOpReturn.h"
#include "intermediate/SequenceOp/SequenceOpWhile.h"
#include "AssignmentExpressionImpl.h"
#include "ExpressionImpl.h"
#include "NameImpl.h"
#include "ScopeImpl.h"
#include "VariableExpressionImpl.h"

#include <boost/make_shared.hpp>

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace intermediate
  {
    const char IntermediateGeneratorSemanticsHandler::BlockImpl::varReturnValueName[] = "$retval";

    DECLARE_STATIC_FORMATTER(FormatCondName, "$cond{0}");
    DECLARE_STATIC_FORMATTER(FormatRetvalName, "$retval{0}");

    IntermediateGeneratorSemanticsHandler::BlockImpl::BlockImpl (IntermediateGeneratorSemanticsHandler* handler,
                                                                 ScopePtr innerScope)
     : handler (handler), innerScope (innerScope),
       sequenceBuilder (boost::make_shared<SequenceBuilder> ())
    {
      uc::String newCondName;
      FormatCondName (newCondName,
                      boost::static_pointer_cast<ScopeImpl> (innerScope)->DistanceToScope (handler->globalScope));
      varCondition = boost::static_pointer_cast<NameImpl> (innerScope->AddVariable (handler->GetBoolType(),
                                                                                    newCondName,
                                                                                    ExpressionPtr(), false));
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      TypeImplPtr retTypeImpl (boost::static_pointer_cast<TypeImpl> (blockScopeImpl->GetFunctionReturnType()));
      if (retTypeImpl // @@@ retTypeImpl == 0 happens in tests; but also in real life?
        && !handler->voidType->IsEqual (*retTypeImpl))
      {
        uc::String newRetValName;
        FormatRetvalName (newRetValName,
                          boost::static_pointer_cast<ScopeImpl> (innerScope)->DistanceToScope (handler->globalScope));
        varReturnValue = boost::static_pointer_cast<NameImpl> (innerScope->AddVariable (retTypeImpl,
                                                                                        newRetValName,
                                                                                        ExpressionPtr(), false));
      }
    }
     
    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddExpressionCommand (ExpressionPtr expr)
    {
      FlushVariableInitializers();
      auto impl = static_cast<ExpressionImpl*> (expr.get());
      impl->AddToSequence (*this);
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddReturnCommand (ExpressionPtr returnValue)
    {
      FlushVariableInitializers();
      
      std::vector<RegisterPtr> retValRegs;
      if (returnValue)
      {
        /*
        ExpressionImpl* impl = static_cast<ExpressionImpl*> (returnValue.get());
        TypeImplPtr retType (boost::static_pointer_cast<TypeImpl> (
          boost::static_pointer_cast<ScopeImpl> (innerScope)->GetFunctionReturnType()));
          
        RegisterID retValReg;
        RegisterID exprTargetReg (impl->AddToSequence (*this, Intermediate));
        if (retType->IsEqual (*(impl->GetValueType ())))
        {
          retValReg = exprTargetReg;
        }
        else
        {
          retValReg = handler->AllocateRegister (*sequence, retType, Intermediate);
          handler->GenerateCast (*sequence, retValReg, retType,
                                 exprTargetReg, impl->GetValueType ());
        }
        retValRegs.push_back (retValReg);
        */
        // Emit initial assignment to return value output parameter
        assert (varReturnValue);
        ExpressionPtr retValVarExpr (handler->CreateVariableExpression (varReturnValue));
        ExpressionPtr retValAssign (handler->CreateAssignExpression (retValVarExpr, returnValue));
        AddExpressionCommand (retValAssign);
        auto retValReg = GetRegisterForName (varReturnValue, false);
        if (!retValReg)
        {
          ExpressionError (returnValue, retValReg.error ());
          return;
        }
        retValRegs.push_back (retValReg.value());
        
        // @@@ This is a hack so the return val out param is later correctly "seen" by the splitter.
        sequenceBuilder->SetExport (varReturnValueName, retValReg.value());
      }
      else
      {
        assert (!varReturnValue);
      }
      
      // Need to collect regs for all output params
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      const std::vector<uc::String>& outputParams = blockScopeImpl->GetFunctionOutputParams();
      for(const uc::String& identifier : outputParams)
      {
        retValRegs.push_back (sequenceBuilder->GetIdentifierRegister (identifier));
      }

      SequenceOpPtr seqOp (new SequenceOpReturn (retValRegs));
      sequenceBuilder->AddOp (seqOp);
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddBranching (ExpressionPtr branchCondition,
                                                                         BlockPtr ifBlock,
                                                                         BlockPtr elseBlock)
    {
      FlushVariableInitializers();
      ExpressionImpl* impl = static_cast<ExpressionImpl*> (branchCondition.get());
      RegisterPtr condReg (impl->AddToSequence (*this, Condition));
      
      if (!elseBlock)
      {
        elseBlock = handler->CreateBlock (GetInnerScope());
      }
      
      boost::shared_ptr<BlockImpl> ifBlockImpl (boost::static_pointer_cast<BlockImpl> (ifBlock));
      boost::shared_ptr<BlockImpl> elseBlockImpl (boost::static_pointer_cast<BlockImpl> (elseBlock));
      
      ifBlockImpl->FinishBlock();
      elseBlockImpl->FinishBlock();
      
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      
      // Collect registers read by the blocks
      std::vector<RegisterPtr> readRegistersIf;
      std::vector<RegisterPtr> readRegistersElse;
      {
        for (NameRegMap::const_iterator import = ifBlockImpl->nameRegisters.begin();
             import != ifBlockImpl->nameRegisters.end();
             ++import)
        {
          if (!import->second.isImported) continue;
          if ((boost::shared_ptr<ScopeImpl> (import->first->ownerScope) != blockScopeImpl)
              && !import->second.initiallyWriteable)
          {
            auto reg = GetRegisterForName (import->first, false);
            if (!reg)
            {
              ExpressionError (branchCondition, reg.error ());
              continue;
            }
            readRegistersIf.push_back (reg.value());
          }
        }
        for (NameRegMap::const_iterator import = elseBlockImpl->nameRegisters.begin();
             import != elseBlockImpl->nameRegisters.end();
             ++import)
        {
          if (!import->second.isImported) continue;
          if ((boost::shared_ptr<ScopeImpl> (import->first->ownerScope) != blockScopeImpl)
              && !import->second.initiallyWriteable)
          {
            auto reg = GetRegisterForName (import->first, false);
            if (!reg)
            {
              ExpressionError (branchCondition, reg.error ());
              continue;
            }
            readRegistersElse.push_back (reg.value());
          }
        }
      }
      
      /* Pass 'snapshot' of identifiers-to-register-ID map
         When resolving imports, the registers for variables _at the time of the block
         insertion_ is needed, hence the snapshot.
         Also, do it before allocating new registers for the 'written registers',
         as we want the ID before that */
      Sequence::IdentifierToRegMap identifierToRegMap (sequenceBuilder->GetIdentifierToRegisterMap ());
      
      NameImplSet allExportedNames (ifBlockImpl->exportedNames);
      allExportedNames.insert (elseBlockImpl->exportedNames.begin(), elseBlockImpl->exportedNames.end());
      
      /* Check for names that are exported from one block but not the other.
         In this case, the original value has to be copied inside the block from which it's not exported. */
      for (NameImplSet::const_iterator exportedName = allExportedNames.begin();
            exportedName != allExportedNames.end();
            exportedName++)
      {
        if (ifBlockImpl->exportedNames.find (*exportedName) == ifBlockImpl->exportedNames.end())
        {
          ExpressionPtr targetExpr (handler->CreateVariableExpression (*exportedName));
          ExpressionPtr valueExpr (handler->CreateVariableExpression (*exportedName));
          ExpressionPtr assignExpr (handler->CreateAssignExpression (targetExpr, valueExpr));
          ifBlockImpl->AddExpressionCommand (assignExpr); // FIXME: should prolly not change original block...
        }
        if (elseBlockImpl->exportedNames.find (*exportedName) == elseBlockImpl->exportedNames.end())
        {
          ExpressionPtr targetExpr (handler->CreateVariableExpression (*exportedName));
          ExpressionPtr valueExpr (handler->CreateVariableExpression (*exportedName));
          ExpressionPtr assignExpr (handler->CreateAssignExpression (targetExpr, valueExpr));
          elseBlockImpl->AddExpressionCommand (assignExpr); // FIXME: should prolly not change original block...
        }
      }
      
      // Generate register IDs for all values the nested blocks export
      typedef std::unordered_map<NameImplPtr, RegisterPtr> ExportedNamesMap;
      ExportedNamesMap seenExportedNames;
      std::vector<RegisterPtr> writtenRegistersIf;
      std::vector<RegisterPtr> writtenRegistersElse;
      {
        for (NameImplSet::const_iterator exportedName = ifBlockImpl->exportedNames.begin();
             exportedName != ifBlockImpl->exportedNames.end();
             exportedName++)
        {
          auto reg = GetRegisterForName (*exportedName, true);
          if (!reg)
          {
            ExpressionError (branchCondition, reg.error ());
            continue;
          }
          writtenRegistersIf.push_back (reg.value());
          seenExportedNames.insert (std::make_pair (*exportedName, reg.value()));
        }
      }
      {
        for (NameImplSet::const_iterator exportedName = elseBlockImpl->exportedNames.begin();
             exportedName != elseBlockImpl->exportedNames.end();
             exportedName++)
        {
          RegisterPtr reg;
          
          ExportedNamesMap::const_iterator prevName (seenExportedNames.find (*exportedName));
          
          if (prevName != seenExportedNames.end())
          {
            reg = prevName->second;
          }
          else
          {
            auto newReg = GetRegisterForName (*exportedName, true);
            if (!newReg)
            {
              ExpressionError (branchCondition, newReg.error ());
              continue;
            }
            reg = newReg.value ();
          }
          writtenRegistersElse.push_back (reg);
        }
      }
      
      SequenceOpPtr seqOpIf (new SequenceOpBlock (ifBlockImpl->GetSequence(),
                                                  identifierToRegMap,
                                                  sequenceBuilder->GetIdentifierToRegisterMap ()));
      SequenceOpPtr seqOpElse (new SequenceOpBlock (elseBlockImpl->GetSequence(),
                                                    identifierToRegMap,
                                                    sequenceBuilder->GetIdentifierToRegisterMap ()));
      
      SequenceOpPtr seqOp (new SequenceOpBranch (condReg, seqOpIf, seqOpElse));
      sequenceBuilder->AddOp (seqOp);
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddWhileLoop (ExpressionPtr loopCond, BlockPtr loopBlock)
    {
      FlushVariableInitializers();
      
      /*
        The while loop looks like:
          while (<cond>)
          {
            <body>
          }
        We rewrite that to:
          bool c = <cond>;	Op #1
          while (c)		Op #2
            <block>
        with <block>:
          {
            <body>
            c = <cond>;
          }
       */
      
      // Emit initial assignment to loop condition var
      {
        ExpressionPtr condVarExpr (handler->CreateVariableExpression (varCondition));
        ExpressionPtr condAssign (handler->CreateAssignExpression (condVarExpr, loopCond));
        AddExpressionCommand (condAssign);
      }
      
      boost::shared_ptr<BlockImpl> blockImpl (boost::static_pointer_cast<BlockImpl> (loopBlock));
      blockImpl->FinishBlock();
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      
      NameImplSet loopVars;
      // Condition var is implicitly read (condition check) and written (end of body)
      loopVars.insert (varCondition);
      // Look for variables that are both read+written in the body
      {
        for (NameRegMap::const_iterator import = blockImpl->nameRegisters.begin();
             import != blockImpl->nameRegisters.end();
             ++import)
        {
          if (!import->second.isImported) continue;
          // If reg is not read initially it can't be looped
          if (import->second.initiallyWriteable) continue;
          if (blockImpl->exportedNames.find (import->first) != blockImpl->exportedNames.end())
          {
            loopVars.insert (import->first);
          }
        }
      }
      
      // For those variable, allocate new 'writeable' registers
      std::vector<std::pair<RegisterPtr, RegisterPtr> > loopedRegs;
      for (NameImplSet::const_iterator loopVar = loopVars.begin();
           loopVar != loopVars.end();
           ++loopVar)
      {
        auto regIn = GetRegisterForName (*loopVar, false);
        if (!regIn)
        {
          ExpressionError (loopCond, regIn.error ());
          continue;
        }
        auto regOut = GetRegisterForName (*loopVar, true);
        if (!regOut)
        {
          ExpressionError (loopCond, regOut.error ());
          continue;
        }
        loopedRegs.emplace_back (regIn.value(), regOut.value());
      }
      
      /* Add condition expression again at the bottom of the block
         as the "condition" in the sequence op is just a simple reg */
      boost::shared_ptr<BlockImpl> newBlock (boost::make_shared<BlockImpl> (*(boost::static_pointer_cast<BlockImpl> (loopBlock))));
      {
        ExpressionPtr condVarExpr (handler->CreateVariableExpression (varCondition));
        ExpressionPtr condAssign (handler->CreateAssignExpression (condVarExpr, loopCond));
        newBlock->AddExpressionCommand (condAssign);
      }
      auto condReg = GetRegisterForName (varCondition, false);
      if (!condReg)
      {
        ExpressionError (loopCond, condReg.error ());
        return;
      }
      
      SequenceOpPtr seqOpBody (CreateBlockSeqOp (newBlock, loopVars));
      SequenceOpPtr seqOp (new SequenceOpWhile (condReg.value(), loopedRegs, seqOpBody));
      sequenceBuilder->AddOp (seqOp);
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddForLoop (ExpressionPtr initExpr, ExpressionPtr loopCond,
                                                                       ExpressionPtr tailExpr, BlockPtr loopBlock)
    {
      FlushVariableInitializers();
      
      /*
        The for loop looks like:
          for (<init>; <cond>; <tail>)
          {
            <body>
          }
        We rewrite that to:
          <init>;		Op #1
          bool c = <cond>;	Op #2
          while (c)		Op #3
            <block>
        with <block>:
          {
            <body>
            <tail>;
            c = <cond>;
          }
       */
      
      ExpressionImpl* initImpl = static_cast<ExpressionImpl*> (initExpr.get());
      if (initImpl) initImpl->AddToSequence (*this);
      
      // Emit initial assignment to loop condition var
      {
        ExpressionPtr condVarExpr (handler->CreateVariableExpression (varCondition));
        ExpressionPtr condAssign;
        if (loopCond)
          condAssign = handler->CreateAssignExpression (condVarExpr, loopCond);
        else
          condAssign = handler->CreateConstBoolExpression (true);
        AddExpressionCommand (condAssign);
      }
      
      ExpressionImpl* tailImpl = static_cast<ExpressionImpl*> (tailExpr.get());
      
      boost::shared_ptr<BlockImpl> blockImpl (boost::static_pointer_cast<BlockImpl> (loopBlock));
      blockImpl->FinishBlock();
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      
      NameImplSet loopVars;
      // Condition var is implicitly read (condition check) and written (end of body)
      loopVars.insert (varCondition);
      // Include all variables changed in the tail expression
      {
        NameImplSet tailVars (tailImpl->QueryWrittenNames (false));
        loopVars.insert (tailVars.begin(), tailVars.end());
      }
      // Look for variables that are both read+written in the body
      {
        for (NameRegMap::const_iterator import = blockImpl->nameRegisters.begin();
             import != blockImpl->nameRegisters.end();
             ++import)
        {
          if (!import->second.isImported) continue;
          // If reg is not read initially it can't be looped
          if (import->second.initiallyWriteable) continue;
          if (blockImpl->exportedNames.find (import->first) != blockImpl->exportedNames.end())
          {
            loopVars.insert (import->first);
          }
        }
      }
      
      // For those variable, allocate new 'writeable' registers
      std::vector<std::pair<RegisterPtr, RegisterPtr> > loopedRegs;
      for (NameImplSet::const_iterator loopVar = loopVars.begin();
           loopVar != loopVars.end();
           ++loopVar)
      {
        auto regIn = GetRegisterForName (*loopVar, false);
        if (!regIn)
        {
          ExpressionError (loopCond, regIn.error ());
          continue;
        }
        auto regOut = GetRegisterForName (*loopVar, true);
        if (!regOut)
        {
          ExpressionError (loopCond, regOut.error ());
          continue;
        }
        loopedRegs.emplace_back (regIn.value(), regOut.value());
      }
      
      boost::shared_ptr<BlockImpl> newBlock (boost::make_shared<BlockImpl> (*(boost::static_pointer_cast<BlockImpl> (loopBlock))));
      // Add "tail" expression to end of block
      if (tailImpl) tailImpl->AddToSequence (*newBlock);
      
      /* Add condition expression again at the bottom of the block
         as the "condition" in the sequence op is just a simple reg */
      {
        ExpressionPtr condVarExpr (handler->CreateVariableExpression (varCondition));
        ExpressionPtr condAssign;
        if (loopCond)
          condAssign = handler->CreateAssignExpression (condVarExpr, loopCond);
        else
          condAssign = handler->CreateConstBoolExpression (true);
        newBlock->AddExpressionCommand (condAssign);
      }
      auto condReg = GetRegisterForName (varCondition, false);
      if (!condReg)
      {
        ExpressionError (loopCond, condReg.error ());
        return;
      }

      SequenceOpPtr seqOpBody (CreateBlockSeqOp (newBlock, loopVars));
      SequenceOpPtr seqOp (new SequenceOpWhile (condReg.value(), loopedRegs, seqOpBody));
      sequenceBuilder->AddOp (seqOp);
    }
    
    void IntermediateGeneratorSemanticsHandler::BlockImpl::AddNestedBlock (BlockPtr block)
    {
      FlushVariableInitializers();
      SequenceOpPtr seqOp (CreateBlockSeqOp (block));
      sequenceBuilder->AddOp (seqOp);
    }
    
    const SequencePtr& IntermediateGeneratorSemanticsHandler::BlockImpl::GetSequence()
    {
      FlushVariableInitializers();
      return GetSequenceBuilder()->GetSequence();
    }
    
    const SequenceBuilderPtr& IntermediateGeneratorSemanticsHandler::BlockImpl::GetSequenceBuilder()
    {
      return sequenceBuilder;
    }
    
    void IntermediateGeneratorSemanticsHandler::BlockImpl::FlushVariableInitializers()
    {
      // Get new variables ...
      std::vector<NamePtr> newVars (boost::static_pointer_cast<ScopeImpl>(innerScope)->FlushNewVars());
      for (std::vector<NamePtr>::const_iterator varIt = newVars.begin();
           varIt != newVars.end();
           ++varIt)
      {
        boost::shared_ptr<NameImpl> name (boost::static_pointer_cast<NameImpl> (*varIt));
        // ... check if it has initialization value ...
        if (name->varValue)
        {
          auto valueExpr = static_cast<ExpressionImpl*> (name->varValue.get ());
          const auto& context = valueExpr->GetExpressionContext ();
          // ... if so, synthesize assignment
          boost::shared_ptr<ExpressionImpl> exprTarget (boost::make_shared<VariableExpressionImpl> (handler, ExpressionContext (context), name));
          ExpressionPtr expr (boost::make_shared<AssignmentExpressionImpl> (handler, ExpressionContext (context),
                                                                            exprTarget,
                                                                            boost::static_pointer_cast<ExpressionImpl> (name->varValue)));
          // Note recursion is okay as FlushNewVars() will return an empty array
          AddExpressionCommand (expr);
        }
      }
    }

    void IntermediateGeneratorSemanticsHandler::BlockImpl::ExpressionError (const ExpressionPtr& expr, Error code)
    {
      handler->ExpressionError (static_cast<ExpressionImpl*> (expr.get())->GetExpressionContext(), code);
    }
    
    SequenceOpPtr IntermediateGeneratorSemanticsHandler::BlockImpl::CreateBlockSeqOp (s1::parser::SemanticsHandler::BlockPtr block,
                                                                                      const NameImplSet& loopNames)
    {
      boost::shared_ptr<BlockImpl> blockImpl (boost::static_pointer_cast<BlockImpl> (block));
      blockImpl->FinishBlock();
      
      boost::shared_ptr<ScopeImpl> blockScopeImpl (boost::static_pointer_cast<ScopeImpl> (innerScope));
      
      // Call GetRegisterForName for all imports as a constness check
      {
        for (NameRegMap::const_iterator import = blockImpl->nameRegisters.begin();
             import != blockImpl->nameRegisters.end();
             ++import)
        {
          if (!import->second.isImported) continue;
          if ((boost::shared_ptr<ScopeImpl> (import->first->ownerScope) != blockScopeImpl)
              && !import->second.initiallyWriteable)
          {
            GetRegisterForName (import->first, false);
          }
        }
      }
      
      /* Pass 'snapshot' of identifiers-to-register-ID map
         When resolving imports, the registers for variables _at the time of the block
         insertion_ is needed, hence the snapshot.
         Also, do it before allocating new registers for the 'written registers',
         as we want the ID before that */
      Sequence::IdentifierToRegMap identifierToRegIDMap (sequenceBuilder->GetIdentifierToRegisterMap ());
      // Generate register IDs for all values the nested block exports
      {
        for (NameImplSet::const_iterator exportedName = blockImpl->exportedNames.begin();
             exportedName != blockImpl->exportedNames.end();
             exportedName++)
        {
          /* "Loop names" are treated somewhat special as the caller will have taken care of
             allocating writeable regs for these names. */
          bool isLoopName = loopNames.find (*exportedName) != loopNames.end();
          GetRegisterForName (*exportedName, !isLoopName);
        }
      }
      // Apply overrides for register IDs of exported identifiers
      return new SequenceOpBlock (blockImpl->GetSequence(),
                                  identifierToRegIDMap,
                                  sequenceBuilder->GetIdentifierToRegisterMap ());
    }

    DECLARE_STATIC_FORMATTER(FormatTernaryResult, "$tr{0}{1}");

    IntermediateGeneratorSemanticsHandler::NameImplPtr
    IntermediateGeneratorSemanticsHandler::BlockImpl::GetTernaryResultName (const TypeImplPtr& resultType)
    {
      std::string typeStr (handler->GetTypeString (resultType));
      TernaryResultVarsMap::const_iterator var (varsTernaryResult.find (typeStr));
      if (var != varsTernaryResult.end()) return var->second;
      
      uc::String newVarName;
      FormatTernaryResult (newVarName,
                           boost::static_pointer_cast<ScopeImpl> (innerScope)->DistanceToScope (handler->globalScope),
                           typeStr.c_str());
      NameImplPtr newVar (boost::static_pointer_cast<NameImpl> (innerScope->AddVariable (resultType,
                                                                                         newVarName,
                                                                                         ExpressionPtr(), false)));
      varsTernaryResult[typeStr] = newVar;
      return newVar;
    }

    DECLARE_STATIC_FORMATTER(FormatImportedReg, "{0}_B{1}");

    IntermediateGeneratorSemanticsHandler::BlockImpl::result_RegisterPtr
    IntermediateGeneratorSemanticsHandler::BlockImpl::GetRegisterForName (const NameImplPtr& name,
                                                                                      bool writeable)
    {
      bool isFromOutside = (boost::shared_ptr<ScopeImpl> (name->ownerScope) != innerScope);
      bool doImport = isFromOutside
                      && ((innerScope->GetLevel() != Function) || !name->isOutputParam)
                      && !writeable;
      bool doExport = isFromOutside
                      && writeable;
      
      if (doExport && name->varConstant)
      {
        return OUTCOME_V2_NAMESPACE::failure (Error::AssignmentTargetIsNotAnLvalue);
      }
      
      NameReg& nameReg = nameRegisters[name];
      RegisterPtr& reg = nameReg.reg;
      /* Note: asking for a register for a constant value is only an error for the second time and
          after; the first request is satisfied as the constant may have to be loaded somewhere */
      if (!reg)
      {
        nameReg.isImported = isFromOutside;
        if (isFromOutside)
        {
          uc::String importName;
          /* Add a suffix derived from the "distance" of this block's scope to the scope
            that defines 'name' in order to make local register name unique */
          int d = boost::static_pointer_cast<ScopeImpl> (innerScope)->DistanceToScope (
            boost::shared_ptr<ScopeImpl> (name->ownerScope));
          if (d >= 0)
          {
            FormatImportedReg (importName, name->identifier, d);
          }
          reg = handler->AllocateRegister (*sequenceBuilder, name->valueType, Imported,
                                          importName);
          if (doImport) sequenceBuilder->SetImport (reg, name->identifier);
        }
        else
        {
          reg = handler->AllocateRegister (*sequenceBuilder, name->valueType, Variable,
                                            name->identifier);
        }
        nameReg.initiallyWriteable = writeable;
        nameReg.initialReg = reg;
        sequenceBuilder->SetIdentifierRegister (name->identifier, reg);
      }
      else
      {
        if (writeable)
        {
          if (name->varConstant)
            return OUTCOME_V2_NAMESPACE::failure (Error::AssignmentTargetIsNotAnLvalue);
          reg = handler->AllocateRegister (*sequenceBuilder, reg);
          sequenceBuilder->SetIdentifierRegister (name->identifier, reg);
        }
      }
      if (doExport)
      {
        sequenceBuilder->SetExport (name->identifier, reg);
        exportedNames.insert (name);
      }
      return reg;
    }

    IntermediateGeneratorSemanticsHandler::BlockImpl::result_void
    IntermediateGeneratorSemanticsHandler::BlockImpl::OverrideNameRegister (const NameImplPtr& name,
                                                                            const RegisterPtr& newRegPtr)
    {
      auto origRegPtr = GetRegisterForName (name, true);
      if (!origRegPtr) return OUTCOME_V2_NAMESPACE::failure (origRegPtr.error ());

      newRegPtr->StealName (*(origRegPtr.value()));
      NameReg& nameReg = nameRegisters[name];
      nameReg.reg = newRegPtr;
      sequenceBuilder->SetIdentifierRegister (name->identifier, newRegPtr);
      if (nameReg.isImported)
        sequenceBuilder->SetExport (name->identifier, newRegPtr);
      return OUTCOME_V2_NAMESPACE::success ();
    }
  } // namespace intermediate
} // namespace s1
