/*
    Shader1
    Copyright (c) 2010-2015 Frank Richter


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
#include "FunctionCallGlobalVarAugment.h"

#include "intermediate/SequenceOp/SequenceOp.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"

namespace s1
{
  namespace intermediate
  {
    FunctionCallGlobalVarAugment::FunctionCallGlobalVarAugment (
      const SequenceBuilderPtr& newSequenceBuilder,
      const SequencePtr& oldSeq,
      const std::vector<IntermediateGeneratorSemanticsHandler::NamePtr>& globals)
      : CloningSequenceVisitor (newSequenceBuilder), globals (globals)
    {
      typedef boost::unordered_map<uc::String, RegisterPtr> ImportsMap;
      ImportsMap importsMap;
      importsMap.insert (oldSeq->GetImports ().begin(), oldSeq->GetImports().end());
      const Sequence::RegisterExpMappings& exportsMap = oldSeq->GetExports ();
      newSequenceBuilder->AddExports (exportsMap);
      for (IntermediateGeneratorSemanticsHandler::NamePtr global : globals)
      {
        if (global->GetType () != IntermediateGeneratorSemanticsHandler::Name::Variable) continue;
        uc::String globalIdent = global->GetIdentifier ();

        // Look for import of global
        {
          RegisterPtr reg;
          uc::String originalName;
          {
            ImportsMap::const_iterator import = importsMap.find (globalIdent);
            if (import != importsMap.end ())
            {
              reg = import->second;
              originalName = reg->GetOriginalName ();
              importsMap.erase (import);
            }
          }
          if (!reg)
          {
            // We need to create a register
            reg =
              IntermediateGeneratorSemanticsHandler::AllocateRegister (
                *newSequenceBuilder, global->GetValueType (),
                IntermediateGeneratorSemanticsHandler::Imported, globalIdent);
            if (originalName.isEmpty ())
              originalName = reg->GetOriginalName ();
          }
          globalVarRegsIn[globalIdent] = reg;
          originalNameToGlobal[originalName] = globalIdent;
          newSequenceBuilder->SetImport (reg, globalIdent);

          globalVarNamesIn.push_back (globalIdent);
        }

        // Look for export of global (use as alternative original name)
        if (!global->IsConstantVariable ())
        {
          uc::String originalName;
          Sequence::RegisterExpMappings::const_iterator exp = exportsMap.find (globalIdent);
          if (exp != exportsMap.end ())
          {
            originalName = exp->second->GetOriginalName ();
            originalNameToGlobal[originalName] = globalIdent;
          }

          globalVarNamesOut.push_back (globalIdent);
        }

      }

      // Forward remaining imports
      for (const Sequence::IdentRegPair& import : importsMap)
      {
        newSequenceBuilder->SetImport (import.second, import.first);
      }
    }

    RegisterPtr FunctionCallGlobalVarAugment::MapRegisterIn (const RegisterPtr& reg)
    {
      OriginalNameToGlobalMap::const_iterator origNameGlobal = originalNameToGlobal.find (reg->GetOriginalName ());
      if (origNameGlobal != originalNameToGlobal.end ())
      {
        GlobalVarRegsMap::const_iterator globalVarIt = globalVarRegsIn.find (origNameGlobal->second);
        if (globalVarIt != globalVarRegsIn.end ())
        {
          return globalVarIt->second;
        }
      }
      return CloningSequenceVisitor::MapRegister (reg);
    }

    RegisterPtr FunctionCallGlobalVarAugment::MapRegisterOut (const RegisterPtr& reg)
    {
      OriginalNameToGlobalMap::const_iterator origNameGlobal = originalNameToGlobal.find (reg->GetOriginalName ());
      const uc::String& globalName = origNameGlobal->second;
      if (origNameGlobal != originalNameToGlobal.end ())
      {
        GlobalVarRegsMap::const_iterator globalVarIt = globalVarRegsOut.find (globalName);
        RegisterPtr newReg;
        if (globalVarIt != globalVarRegsOut.end ())
        {
          // Get a new generation of the register
          newReg = newSequenceBuilder->AllocateRegister (globalVarIt->second);
        }
        else
        {
          // Register was never written
          newReg = newSequenceBuilder->AllocateRegister (reg);
        }
        return newReg;
      }
      return CloningSequenceVisitor::MapRegister (reg);
    }

    void FunctionCallGlobalVarAugment::AddOpToSequence (const SequenceOpPtr& seqOp)
    {
      CloningSequenceVisitor::AddOpToSequence (seqOp);

      // Update map of current global var registers based on original names of written regs
      for(RegisterPtr reg : seqOp->GetWrittenRegisters ())
      {
        OriginalNameToGlobalMap::const_iterator origNameGlobal = originalNameToGlobal.find (reg->GetOriginalName ());
        if (origNameGlobal == originalNameToGlobal.end ()) continue;
        const uc::String& globalName = origNameGlobal->second;
        globalVarRegsIn[globalName] = reg;
        globalVarRegsOut[globalName] = reg;
      }
    }

    void FunctionCallGlobalVarAugment::VisitEnd ()
    {
      CloningSequenceVisitor::VisitEnd ();
      // Update exports of sequence builder

      for(const uc::String& globalOut : globalVarNamesOut)
      {
        GlobalVarRegsMap::const_iterator globalVarRegOutIt = globalVarRegsOut.find (globalOut);
        RegisterPtr reg;
        if (globalVarRegOutIt != globalVarRegsOut.end ())
        {
          reg = globalVarRegOutIt->second;
        }
        else
        {
          // Register was never written. Copy from import
          const auto globalVarInIt = globalVarRegsIn.find (globalOut);
          assert (globalVarInIt != globalVarRegsIn.end ());
          reg = newSequenceBuilder->AllocateRegister (globalVarInIt->second->GetOriginalType(), globalOut);
          SequenceOpPtr newOp (new SequenceOpAssign (reg, globalVarInIt->second));
          CloningSequenceVisitor::AddOpToSequence (newOp);
        }
        newSequenceBuilder->SetExport (globalOut, reg);
      }
    }

    void FunctionCallGlobalVarAugment::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      // Obtain current registers for all "output" global vars
      std::vector<RegisterPtr> newOutParamVals (outParamVals);
      for (const uc::String& globalOut : globalVarNamesOut)
      {
        GlobalVarRegsMap::const_iterator globalVarReg = globalVarRegsOut.find (globalOut);
        RegisterPtr reg;
        if (globalVarReg != globalVarRegsOut.end ())
        {
          reg = globalVarReg->second;
        }
        else
        {
          const auto globalVarInIt = globalVarRegsIn.find (globalOut);
          assert (globalVarInIt != globalVarRegsIn.end ());
          reg = globalVarInIt->second;
        }
        newOutParamVals.push_back (reg);
      }

      CloningSequenceVisitor::OpReturn (newOutParamVals);
    }

    void FunctionCallGlobalVarAugment::OpFunctionCall (const uc::String& funcIdent,
      const std::vector<RegisterPtr>& inParams,
      const std::vector<RegisterPtr>& outParams)
    {
      // Obtain current registers for all "input" global vars
      std::vector<RegisterPtr> newInParams (inParams);
      for (const uc::String& globalIn : globalVarNamesIn)
      {
        GlobalVarRegsMap::const_iterator globalVarReg = globalVarRegsIn.find (globalIn);
        assert (globalVarReg != globalVarRegsIn.end ());
        newInParams.push_back (globalVarReg->second);
      }

      // Alloc new registers for all "output" global vars
      std::vector<RegisterPtr> newOutParams (outParams);
      for(const uc::String& globalOut : globalVarNamesOut)
      {
        RegisterPtr newReg = GetWriteGlobalOutReg (globalOut);
        newOutParams.push_back (newReg);
      }

      CloningSequenceVisitor::OpFunctionCall (funcIdent, newInParams, newOutParams);
    }

    RegisterPtr FunctionCallGlobalVarAugment::GetWriteGlobalOutReg (const uc::String& globalOut)
    {
      GlobalVarRegsMap::const_iterator globalVarRegOutIt = globalVarRegsOut.find (globalOut);
      RegisterPtr reg;
      if (globalVarRegOutIt != globalVarRegsOut.end ())
      {
        // Register was assigned previously
        reg = globalVarRegOutIt->second;
      }
      else
      {
        // Register was never assigned before, create
        const auto globalVarInIt = globalVarRegsIn.find (globalOut);
        assert (globalVarInIt != globalVarRegsIn.end ());
        reg = globalVarInIt->second;
      }
      return newSequenceBuilder->AllocateRegister (reg);
    }
    
    void FunctionCallGlobalVarAugment::NestedBlock (CloningSequenceVisitor* handlingVisitor,
                                                    const SequencePtr& seq,
                                                    const Sequence::IdentifierToRegMap& identToReg_imp,
                                                    const Sequence::IdentifierToRegMap& identToReg_exp)
    {
      Sequence::IdentifierToRegMap newImpMap (identToReg_imp);
      for (const uc::String& globalIn : globalVarNamesIn)
      {
        const auto impIt = newImpMap.find (globalIn);
        if (impIt == newImpMap.end ())
        {
          const auto globalVarReg = globalVarRegsIn.find (globalIn);
          assert (globalVarReg != globalVarRegsIn.end ());
          newImpMap[globalIn] = globalVarReg->second;
        }
      }

      Sequence::IdentifierToRegMap newExpMap (identToReg_exp);
      for(const uc::String& globalOut : globalVarNamesOut)
      {
        const auto expIt = newExpMap.find (globalOut);
        if (expIt == newImpMap.end ())
        {
          newExpMap[globalOut] = GetWriteGlobalOutReg (globalOut);
        }
      }

      CloningSequenceVisitor::NestedBlock (handlingVisitor, seq, newImpMap, newExpMap);
    }

    CloningSequenceVisitor* FunctionCallGlobalVarAugment::Clone (const SequenceBuilderPtr& newSequenceBuilder,
      const SequencePtr& oldSequence,
      const RegisterMap& regMap)
    {
      return new FunctionCallGlobalVarAugment (newSequenceBuilder, oldSequence, globals);
    }

    CloningSequenceVisitor* FunctionCallGlobalVarAugment::Clone (const SequenceBuilderPtr& newSequenceBuilder,
      const RegisterMap& regMap)
    {
      assert (false);
      return nullptr;
    }

  } // namespace intermediate
} // namespace s1
