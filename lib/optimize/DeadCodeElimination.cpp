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
#include "optimize/DeadCodeElimination.h"

#include "CommonSequenceVisitor.h"
#include "intermediate/SequenceOp/SequenceOp.h"

namespace s1
{
  namespace optimize
  {
    class DeadCodeElimination::DeadCodeChecker : public CommonSequenceVisitor
    {
      bool& seqChanged;
      intermediate::RegisterSet usedRegisters;
    public:
      DeadCodeChecker (const intermediate::SequenceBuilderPtr& outputSeqBuilder,
                       const intermediate::RegisterSet& usedRegistersSeed,
                       bool& seqChanged)
       : CommonSequenceVisitor (outputSeqBuilder),
         seqChanged (seqChanged),
         usedRegisters (usedRegistersSeed)
      {
      }

      CommonSequenceVisitor* Clone (const intermediate::SequenceBuilderPtr& newSequenceBuilder,
                                    const RegisterMap& regMap) override
      {
        intermediate::RegisterSet newUsedRegisters;
        for(const RegisterPtr& reg : usedRegisters)
        {
          RegisterMap::const_iterator newSeqReg = regMap.find (reg);
          if (newSeqReg != regMap.end())
          {
            newUsedRegisters.insert (newSeqReg->second);
          }
        }
        return new DeadCodeChecker (newSequenceBuilder, newUsedRegisters, seqChanged);
      }

      bool VisitBackwards() const override { return true; }

      void PostVisitSequence (CommonSequenceVisitor* visitor,
                              const intermediate::SequenceBuilderPtr& newSequenceBuilder,
                              const RegisterMap& regMap) override
      {
        newSequenceBuilder->CleanUnusedImportsExports();
      }

      void AddOpToSequence (const SequenceOpPtr& seqOp) override
      {
        // DeadCodeChecker is meant to use with reverse Sequence visiting,
        // so to 'add' an op, insert it at the beginning
        newSequenceBuilder->FrontInsertOp (seqOp);
      }

      void OpConstBool (const RegisterPtr& destination,
                        bool value) override;
      void OpConstInt (const RegisterPtr& destination,
                        int value) override;
      void OpConstUInt (const RegisterPtr& destination,
                        unsigned int value) override;
      void OpConstFloat (const RegisterPtr& destination,
                          float value) override;

      void OpAssign (const RegisterPtr& destination,
                      const RegisterPtr& source) override;

      void OpCast (const RegisterPtr& destination,
                    intermediate::BasicType destType,
                    const RegisterPtr& source) override;

      void OpMakeVector (const RegisterPtr& destination,
                          intermediate::BasicType compType,
                          const std::vector<RegisterPtr>& sources) override;
      void OpVectorDot (const RegisterPtr& destination,
                        const RegisterPtr& source1,
                        const RegisterPtr& source2) override;
      void OpVectorCross (const RegisterPtr& destination,
                          const RegisterPtr& source1,
                          const RegisterPtr& source2) override;
      void OpVectorNormalize (const RegisterPtr& destination,
                              const RegisterPtr& source) override;
      void OpVectorLength (const RegisterPtr& destination,
                           const RegisterPtr& source) override;

      void OpMakeMatrix (const RegisterPtr& destination,
                          intermediate::BasicType compType,
                          unsigned int matrixRows, unsigned int matrixCols,
                          const std::vector<RegisterPtr>& sources) override;
      void OpMatrixLinAlgMul (const RegisterPtr& destination,
                              const RegisterPtr& source1,
                              const RegisterPtr& source2) override;

      void OpMakeArray (const RegisterPtr& destination,
                        const std::vector<RegisterPtr>& sources) override;
      void OpExtractArrayElement (const RegisterPtr& destination,
                                  const RegisterPtr& source,
                                  const RegisterPtr& index) override;
      void OpChangeArrayElement (const RegisterPtr& destination,
                                  const RegisterPtr& source,
                                  const RegisterPtr& index,
                                  const RegisterPtr& newValue) override;
      void OpGetArrayLength (const RegisterPtr& destination,
                              const RegisterPtr& array) override;

      void OpExtractVectorComponent (const RegisterPtr& destination,
                                      const RegisterPtr& source,
                                      unsigned int comp) override;

      void OpArith (const RegisterPtr& destination,
                    ArithmeticOp op,
                    const RegisterPtr& source1,
                    const RegisterPtr& source2) override;

      void OpLogic (const RegisterPtr& destination,
                    LogicOp op,
                    const RegisterPtr& source1,
                    const RegisterPtr& source2) override;

      void OpUnary (const RegisterPtr& destination,
                    UnaryOp op,
                    const RegisterPtr& source) override;

      void OpCompare (const RegisterPtr& destination,
                      CompareOp op,
                      const RegisterPtr& source1,
                      const RegisterPtr& source2) override;

      void OpBlock (const intermediate::SequencePtr& seq,
                    const Sequence::IdentifierToRegMap& identToRegID_imp,
                    const Sequence::IdentifierToRegMap& identToRegID_exp) override;

      void OpBranch (const RegisterPtr& conditionReg,
                      const intermediate::SequenceOpPtr& seqOpIf,
                      const intermediate::SequenceOpPtr& seqOpElse) override;
      void OpWhile (const RegisterPtr& conditionReg,
                    const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                    const intermediate::SequenceOpPtr& seqOpBody) override;

      void OpSampleTexture (const RegisterPtr& destination,
                            SampleTextureOp what,
                            const RegisterPtr& sampler,
                            const RegisterPtr& coord) override;

      void OpReturn (const std::vector<RegisterPtr>& outParamVals) override;
      void OpFunctionCall (const uc::String& funcIdent,
                            const std::vector<RegisterPtr>& inParams,
                            const std::vector<RegisterPtr>& outParams) override;
      void OpBuiltinCall (const RegisterPtr& destination,
                          intermediate::BuiltinFunction what,
                          const std::vector<RegisterPtr>& inParams) override;
    };

    void DeadCodeElimination::DeadCodeChecker::OpConstBool (const RegisterPtr& destination,
                                                            bool value)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      CommonSequenceVisitor::OpConstBool (destination, value);
    }

    void DeadCodeElimination::DeadCodeChecker::OpConstInt (const RegisterPtr& destination,
                                                           int value)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      CommonSequenceVisitor::OpConstInt (destination, value);
    }

    void DeadCodeElimination::DeadCodeChecker::OpConstUInt (const RegisterPtr& destination,
                                                            unsigned int value)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      CommonSequenceVisitor::OpConstUInt (destination, value);
    }

    void DeadCodeElimination::DeadCodeChecker::OpConstFloat (const RegisterPtr& destination,
                                                             float value)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      CommonSequenceVisitor::OpConstFloat (destination, value);
    }

    void DeadCodeElimination::DeadCodeChecker::OpAssign (const RegisterPtr& destination,
                                                         const RegisterPtr& source)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source);
      CommonSequenceVisitor::OpAssign (destination, source);
    }

    void DeadCodeElimination::DeadCodeChecker::OpCast (const RegisterPtr& destination,
                                                       intermediate::BasicType destType,
                                                       const RegisterPtr& source)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source);
      CommonSequenceVisitor::OpCast (destination, destType, source);
    }

    void DeadCodeElimination::DeadCodeChecker::OpMakeVector (const RegisterPtr& destination,
                                                             intermediate::BasicType compType,
                                                             const std::vector<RegisterPtr>& sources)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (sources.begin(), sources.end());
      CommonSequenceVisitor::OpMakeVector (destination, compType, sources);
    }

    void DeadCodeElimination::DeadCodeChecker::OpVectorDot (const RegisterPtr& destination,
                                                            const RegisterPtr& source1,
                                                            const RegisterPtr& source2)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source1);
      usedRegisters.insert (source2);
      CommonSequenceVisitor::OpVectorDot (destination, source1, source2);
    }

    void DeadCodeElimination::DeadCodeChecker::OpVectorCross (const RegisterPtr& destination,
                                                              const RegisterPtr& source1,
                                                              const RegisterPtr& source2)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source1);
      usedRegisters.insert (source2);
      CommonSequenceVisitor::OpVectorCross (destination, source1, source2);
    }

    void DeadCodeElimination::DeadCodeChecker::OpVectorNormalize (const RegisterPtr& destination,
                                                                  const RegisterPtr& source)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source);
      CommonSequenceVisitor::OpVectorNormalize (destination, source);
    }

    void DeadCodeElimination::DeadCodeChecker::OpVectorLength (const RegisterPtr& destination,
                                                               const RegisterPtr& source)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source);
      CommonSequenceVisitor::OpVectorLength (destination, source);
    }

    void DeadCodeElimination::DeadCodeChecker::OpMakeMatrix (const RegisterPtr& destination,
                                                             intermediate::BasicType compType,
                                                             unsigned int matrixRows, unsigned int matrixCols,
                                                             const std::vector<RegisterPtr>& sources)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (sources.begin(), sources.end());
      CommonSequenceVisitor::OpMakeMatrix (destination, compType, matrixRows, matrixCols, sources);
    }

    void DeadCodeElimination::DeadCodeChecker::OpMatrixLinAlgMul (const RegisterPtr& destination,
                                                                  const RegisterPtr& source1,
                                                                  const RegisterPtr& source2)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source1);
      usedRegisters.insert (source2);
      CommonSequenceVisitor::OpMatrixLinAlgMul (destination, source1, source2);
    }

    void DeadCodeElimination::DeadCodeChecker::OpMakeArray (const RegisterPtr& destination,
                                                            const std::vector<RegisterPtr>& sources)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (sources.begin(), sources.end());
      CommonSequenceVisitor::OpMakeArray (destination, sources);
    }

    void DeadCodeElimination::DeadCodeChecker::OpExtractArrayElement (const RegisterPtr& destination,
                                                                      const RegisterPtr& source,
                                                                      const RegisterPtr& index)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source);
      usedRegisters.insert (index);
      CommonSequenceVisitor::OpExtractArrayElement (destination, source, index);
    }

    void DeadCodeElimination::DeadCodeChecker::OpChangeArrayElement (const RegisterPtr& destination,
                                                                     const RegisterPtr& source,
                                                                     const RegisterPtr& index,
                                                                     const RegisterPtr& newValue)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source);
      usedRegisters.insert (index);
      usedRegisters.insert (newValue);
      CommonSequenceVisitor::OpChangeArrayElement (destination, source, index, newValue);
    }

    void DeadCodeElimination::DeadCodeChecker::OpGetArrayLength (const RegisterPtr& destination,
                                                                 const RegisterPtr& array)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (array);
      CommonSequenceVisitor::OpGetArrayLength (destination, array);
    }

    void DeadCodeElimination::DeadCodeChecker::OpExtractVectorComponent (const RegisterPtr& destination,
                                                                         const RegisterPtr& source,
                                                                         unsigned int comp)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source);
      CommonSequenceVisitor::OpExtractVectorComponent (destination, source, comp);
    }

    void DeadCodeElimination::DeadCodeChecker::OpArith (const RegisterPtr& destination,
                                                        ArithmeticOp op,
                                                        const RegisterPtr& source1,
                                                        const RegisterPtr& source2)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source1);
      usedRegisters.insert (source2);
      CommonSequenceVisitor::OpArith (destination, op, source1, source2);
    }

    void DeadCodeElimination::DeadCodeChecker::OpLogic (const RegisterPtr& destination,
                                                        LogicOp op,
                                                        const RegisterPtr& source1,
                                                        const RegisterPtr& source2)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source1);
      usedRegisters.insert (source2);
      CommonSequenceVisitor::OpLogic (destination, op, source1, source2);
    }

    void DeadCodeElimination::DeadCodeChecker::OpUnary (const RegisterPtr& destination,
                                                        UnaryOp op,
                                                        const RegisterPtr& source)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source);
      CommonSequenceVisitor::OpUnary (destination, op, source);
    }

    void DeadCodeElimination::DeadCodeChecker::OpCompare (const RegisterPtr& destination,
                                                          CompareOp op,
                                                          const RegisterPtr& source1,
                                                          const RegisterPtr& source2)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (source1);
      usedRegisters.insert (source2);
      CommonSequenceVisitor::OpCompare (destination, op, source1, source2);
    }

    void DeadCodeElimination::DeadCodeChecker::OpBlock (const intermediate::SequencePtr& seq,
                                                        const Sequence::IdentifierToRegMap& identToRegID_imp,
                                                        const Sequence::IdentifierToRegMap& identToRegID_exp)
    {
      intermediate::RegisterSet sequenceExported (seq->GetExportOuterRegs (identToRegID_exp));
      size_t exportedUsed = 0;
      for(const RegisterPtr& reg : sequenceExported)
      {
        if (usedRegisters.count (reg) > 0)
          exportedUsed++;
      }
      if (exportedUsed == 0)
      {
        seqChanged = true;
        return;
      }
      intermediate::RegisterSet sequenceImported (seq->GetExportOuterRegs (identToRegID_exp));
      usedRegisters.insert (sequenceImported.begin(), sequenceImported.end());
      CommonSequenceVisitor::OpBlock (seq, identToRegID_imp, identToRegID_exp);
      /* @@@ DCE for nested sequence?
         Though probably not necessary as DCE is  usually applied _after_ inlining...
       */
    }

    void DeadCodeElimination::DeadCodeChecker::OpBranch (const RegisterPtr& conditionReg,
                                                         const intermediate::SequenceOpPtr& seqOpIf,
                                                         const intermediate::SequenceOpPtr& seqOpElse)
    {
      intermediate::RegisterSet readRegisters;
      intermediate::RegisterSet writtenRegisters;
      if (seqOpIf)
      {
        readRegisters = seqOpIf->GetReadRegisters();
        writtenRegisters = seqOpIf->GetWrittenRegisters();
      }
      if (seqOpElse)
      {
        intermediate::RegisterSet elseReadRegs (seqOpElse->GetReadRegisters());
        readRegisters.insert (elseReadRegs.begin(), elseReadRegs.end());
        intermediate::RegisterSet elseWrittenRegs (seqOpElse->GetWrittenRegisters());
        writtenRegisters.insert (elseWrittenRegs.begin(), elseWrittenRegs.end());
      }
      size_t writtenRegsUsed = 0;
      for(const RegisterPtr& reg : writtenRegisters)
      {
        if (usedRegisters.count (reg) > 0)
          writtenRegsUsed++;
      }
      if (writtenRegsUsed == 0)
      {
        seqChanged = true;
        return;
      }

      usedRegisters.insert (conditionReg);
      usedRegisters.insert (readRegisters.begin(), readRegisters.end());
      CommonSequenceVisitor::OpBranch (conditionReg, seqOpIf, seqOpElse);
    }

    void DeadCodeElimination::DeadCodeChecker::OpWhile (const RegisterPtr& conditionReg,
                                                        const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                                                        const intermediate::SequenceOpPtr& seqOpBody)
    {
      intermediate::RegisterSet readRegisters;
      intermediate::RegisterSet writtenRegisters;
      if (seqOpBody)
      {
        readRegisters = seqOpBody->GetReadRegisters();
        writtenRegisters = seqOpBody->GetWrittenRegisters();
      }
      typedef std::pair<RegisterPtr, RegisterPtr> RegPair;
      for(const RegPair& loopPair : loopedRegs)
      {
        readRegisters.insert (loopPair.first);
        writtenRegisters.insert (loopPair.second);
      }
      size_t writtenRegsUsed = 0;
      for(const RegisterPtr& reg : writtenRegisters)
      {
        if (usedRegisters.count (reg) > 0)
          writtenRegsUsed++;
      }
      if (writtenRegsUsed == 0)
      {
        seqChanged = true;
        return;
      }

      usedRegisters.insert (conditionReg);
      usedRegisters.insert (writtenRegisters.begin (), writtenRegisters.end ());
      CommonSequenceVisitor::OpWhile (conditionReg, loopedRegs, seqOpBody);
      usedRegisters.insert (readRegisters.begin(), readRegisters.end());
      /* TODO: Tagging all loop registers as 'used' diminishes the effectiveness
       * of the DCE.
       * However, to compute which registers are truly "used" we need a more
       * sophisticated approach: a graph of register dependencies. */
    }

    void DeadCodeElimination::DeadCodeChecker::OpSampleTexture (const RegisterPtr& destination,
                                                                SampleTextureOp what,
                                                                const RegisterPtr& sampler,
                                                                const RegisterPtr& coord)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (sampler);
      usedRegisters.insert (coord);
      CommonSequenceVisitor::OpSampleTexture (destination, what, sampler, coord);
    }

    void DeadCodeElimination::DeadCodeChecker::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      usedRegisters.insert (outParamVals.begin(), outParamVals.end());
      CommonSequenceVisitor::OpReturn (outParamVals);
    }

    void DeadCodeElimination::DeadCodeChecker::OpFunctionCall (const uc::String& funcIdent,
                                                               const std::vector<RegisterPtr>& inParams,
                                                               const std::vector<RegisterPtr>& outParams)
    {
      size_t outParamsUsed = 0;
      for(const RegisterPtr& reg : outParams)
      {
        if (usedRegisters.count (reg) > 0)
          outParamsUsed++;
      }
      if (outParamsUsed == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (inParams.begin(), inParams.end());
      CommonSequenceVisitor::OpFunctionCall (funcIdent, inParams, outParams);
    }

    void DeadCodeElimination::DeadCodeChecker::OpBuiltinCall (const RegisterPtr& destination,
                                                              intermediate::BuiltinFunction what,
                                                              const std::vector<RegisterPtr>& inParams)
    {
      if (usedRegisters.count (destination) == 0)
      {
        seqChanged = true;
        return;
      }
      usedRegisters.insert (inParams.begin(), inParams.end());
      CommonSequenceVisitor::OpBuiltinCall (destination, what, inParams);
    }

    //-----------------------------------------------------------------------

    bool DeadCodeElimination::EliminateDeadCode (const intermediate::SequenceBuilderPtr& outputSeqBuilder,
                                                 const intermediate::SequencePtr& inputSeq,
                                                 const intermediate::RegisterSet& usedRegistersSeed)
    {
      bool seqChanged (false);
      DeadCodeChecker checker (outputSeqBuilder, usedRegistersSeed, seqChanged);
      inputSeq->ReverseVisit (checker);
      return seqChanged;
    }
  } // namespace optimize
} // namespace s1
