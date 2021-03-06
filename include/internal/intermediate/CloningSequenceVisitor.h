/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __INTERMEDIATE_COMMONSEQUENCEVISITOR_H__
#define __INTERMEDIATE_COMMONSEQUENCEVISITOR_H__

#include "SequenceBuilder.h"
#include "SequenceVisitor.h"

#include "SequenceOp/SequenceOp.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpBlock;

    /**
     * SequenceVisitor implementation that clones a sequence to a target.
     * Optionally applies a mapping to registers. The addition of
     * operations to the target sequence can be hooked.
     */
    class CloningSequenceVisitor : public SequenceVisitor
    {
    public:
      SequenceBuilderPtr newSequenceBuilder;

      CloningSequenceVisitor (const SequenceBuilderPtr& newSequenceBuilder);

      /// Map a register.
      virtual RegisterPtr MapRegister (const RegisterPtr& reg)
      { return reg; }
      virtual RegisterPtr MapRegisterIn (const RegisterPtr& reg) { return MapRegister (reg); }
      virtual RegisterPtr MapRegisterOut (const RegisterPtr& reg) { return MapRegister (reg); }
      /// Add an operation to the target sequence.
      virtual void AddOpToSequence (const SequenceOpPtr& seqOp)
      {
        newSequenceBuilder->AddOp (seqOp);
      }

      void PreVisitOp (const SequenceOpPtr& op) override
      { visitedOp = op; }
      void PostVisitOp () override {}
      void VisitEnd() override
      { visitedOp.reset(); }

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
                    BasicType destType,
                    const RegisterPtr& source) override;

      void OpMakeVector (const RegisterPtr& destination,
                          BasicType compType,
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
                          BasicType compType,
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

      void OpBlock (const SequencePtr& seq,
                    const Sequence::IdentifierToRegMap& identToRegID_imp,
                    const Sequence::IdentifierToRegMap& identToRegID_exp) override;

      void OpBranch (const RegisterPtr& conditionReg,
                      const SequenceOpPtr& seqOpIf,
                      const SequenceOpPtr& seqOpElse) override;
      void OpWhile (const RegisterPtr& conditionReg,
                    const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
                    const SequenceOpPtr& seqOpBody) override;

      void OpSampleTexture (const RegisterPtr& destination,
                            SampleTextureOp what,
                            const RegisterPtr& sampler,
                            const RegisterPtr& coord) override;

      void OpReturn (const std::vector<RegisterPtr>& outParamVals) override;
      void OpFunctionCall (const uc::String& funcIdent,
                            const std::vector<RegisterPtr>& inParams,
                            const std::vector<RegisterPtr>& outParams) override;
      void OpBuiltinCall (const RegisterPtr& destination,
                          BuiltinFunction what,
                          const std::vector<RegisterPtr>& inParams) override;
    protected:
      class BlockNestingSequenceVisitor;
      typedef boost::intrusive_ptr<SequenceOpBlock> SequenceOpBlockPtr;
      virtual SequenceOpBlockPtr NestedBlock (const SequencePtr& seq,
                                              const Sequence::IdentifierToRegMap& identToReg_imp,
                                              const Sequence::IdentifierToRegMap& identToReg_exp);

      SequenceOpPtr visitedOp;

      typedef std::unordered_map<RegisterPtr, RegisterPtr> RegisterMap;
      virtual CloningSequenceVisitor* Clone (const SequenceBuilderPtr& newSequenceBuilder,
                                             const SequencePtr& oldSequence,
                                             const RegisterMap& regMap);
      virtual CloningSequenceVisitor* Clone (const SequenceBuilderPtr& newSequenceBuilder,
                                             const RegisterMap& regMap) = 0;
      virtual bool VisitBackwards() const { return false; }
      virtual void PostVisitSequence (CloningSequenceVisitor* visitor,
                                      const SequenceBuilderPtr& newSequenceBuilder,
                                      const RegisterMap& regMap) {}
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_COMMONSEQUENCEVISITOR_H__
