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

#ifndef __SPLITTER_SEQUENCESPLITTER_H__
#define __SPLITTER_SEQUENCESPLITTER_H__

#include "intermediate/forwarddecl.h"
#include "intermediate/SequenceVisitor.h"

#include "Frequency.h"

namespace s1
{
  namespace splitter
  {
    class ProgramSplitter;

    class SequenceSplitter
    {
      typedef intermediate::RegisterPtr RegisterPtr;
      typedef intermediate::Sequence Sequence;
      typedef intermediate::SequencePtr SequencePtr;
      typedef intermediate::SequenceBuilderPtr SequenceBuilderPtr;
      typedef intermediate::SequenceOpPtr SequenceOpPtr;

      intermediate::SequencePtr inputSeq;

      typedef std::unordered_map<uc::String, unsigned int> ParamMap;
      ParamMap paramFlags;
      typedef std::unordered_map<uc::String, Frequency> OutputFreqMap;
      OutputFreqMap outputFreqs;

      class InputVisitor : public intermediate::SequenceVisitor
      {
        SequenceSplitter& parent;

        void SplitBinaryOp (const RegisterPtr& destination, const SequenceOpPtr& op,
                            const RegisterPtr& source1, const RegisterPtr& source2);
        template<typename Container>
        unsigned int ComputeCombinedFreqs (const Container& sources);
        int ComputeHighestFreq (const RegisterPtr& reg);
        template<typename... T>
        int ComputeHighestFreq (const RegisterPtr& reg, const T&... regs);
        template<typename Container>
        int ComputeHighestFreq (const Container& sources);
        template<typename Container>
        unsigned int Promote (int freq, const Container& sources);
        unsigned int Promote (int freq, const RegisterPtr& reg);
        template<typename... T>
        unsigned int Promote (int freq, const RegisterPtr& reg, const T&... regs);

        unsigned int AddOpToSequences (const SequenceOpPtr& op, unsigned int freqMask);

        typedef std::pair<RegisterPtr, RegisterPtr> LoopedReg;
        typedef std::vector<LoopedReg> LoopedRegs;
        struct SplitResult
        {
          SequenceBuilderPtr seqBuilder;
          Sequence::IdentifierToRegMap identToRegs_imp;
          Sequence::IdentifierToRegMap identToRegs_exp;
        };
        void SplitBlock (const SequencePtr& blockSequence,
                         const Sequence::IdentifierToRegMap& identToRegIDs_imp,
                         const Sequence::IdentifierToRegMap& identToRegIDs_exp,
                         SplitResult* newSequences,
                         bool keepEmpty = false,
                         bool mergeUniformToVF = false);

        typedef std::pair<uc::String, RegisterPtr> RenamedRegister;
        typedef std::vector<RenamedRegister> RenamedBranchOutputs;
        void EmitUnconditionalBranchBlock (SequencePtr origSeq, const char* suffix,
                                           const SplitResult& splitRes,
                                           int f,
                                           RenamedBranchOutputs& outputs);
        SequenceOpPtr AugmentBranchBlockWithRenames (const char* suffix,
                                                     const SplitResult& splitRes,
                                                     const RenamedBranchOutputs* renames,
                                                     int f);
      public:
        InputVisitor (SequenceSplitter& parent);

        void PreVisitOp (const intermediate::SequenceOpPtr& op) override {}
        void PostVisitOp () override {}
        void VisitEnd () override;

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

        void OpCompare (const RegisterPtr& destination,
                        CompareOp op,
                        const RegisterPtr& source1,
                        const RegisterPtr& source2) override;

        void OpLogic (const RegisterPtr& destination,
                      LogicOp op,
                      const RegisterPtr& source1,
                      const RegisterPtr& source2) override;

        void OpUnary (const RegisterPtr& destination,
                      UnaryOp op,
                      const RegisterPtr& source) override;

        void OpBlock (const SequencePtr& subSequence,
                      const Sequence::IdentifierToRegMap& identToRegIDs_imp,
                      const Sequence::IdentifierToRegMap& identToRegIDs_exp) override;

        void OpBranch (const RegisterPtr& conditionReg,
                       const SequenceOpPtr& seqOpIf,
                       const SequenceOpPtr& seqOpElse) override;
        void OpWhile (const RegisterPtr& conditionReg,
                      const LoopedRegs& loopedRegs,
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
                            intermediate::BuiltinFunction what,
                            const std::vector<RegisterPtr>& inParams) override;
      };
    public:
      SequenceSplitter (ProgramSplitter& progSplit, bool mergeUniformToVF = true);

      void SetInputSequence (const intermediate::SequencePtr& sequence)
      { inputSeq = sequence; }

      // set input param frequencies
      void SetInputFreqFlags (const uc::String& input, unsigned int freqFlags);
      // Set output param frequencies
      void SetOutputFreq (const uc::String& output, Frequency freq)
      { outputFreqs[output] = freq; }
      void SetLocalRegFreqFlags (const RegisterPtr& regID, unsigned int freqFlags)
      { setAvailability[regID] = freqFlags; }
      unsigned int GetLocalRegFreqFlags (const RegisterPtr& regID)
      { return setAvailability[regID]; }
      unsigned int GetRegAvailability (const RegisterPtr& reg);

      void PerformSplit ();

      typedef std::vector<RegisterPtr> TransferRegsVector;
      const TransferRegsVector& GetTransferRegs (int srcFreq) const
      { return transferRegs[srcFreq]; }

      intermediate::SequenceBuilderPtr GetOutputUniformSequence ()
      { return outputSeqBuilder[freqUniform]; }
      intermediate::SequenceBuilderPtr GetOutputVertexSequence ()
      { return outputSeqBuilder[freqVertex]; }
      intermediate::SequenceBuilderPtr GetOutputFragmentSequence ()
      { return outputSeqBuilder[freqFragment]; }

      intermediate::SequenceBuilderPtr GetOutputSequence (int freq)
      { return outputSeqBuilder[freq]; }
    protected:
      ProgramSplitter& progSplit;
      bool mergeUniformToVF;

      intermediate::SequenceBuilderPtr outputSeqBuilder[freqNum];

      /** Registers to transfer to next frequency.
          The registers in element 0 need to be transferred from frequency 0 to frequency 1,
          the registers in element 1 from frequency 2 to frequency 2 and so on. */
      TransferRegsVector transferRegs[freqNum-1];

      void SetRegAvailability (const RegisterPtr& regID, unsigned int freqFlags)
      { regAvailability[regID] = freqFlags; }

      uc::String GetTransferIdent (const uc::String& origName = uc::String());

      /// Allocate register in all output sequences
      RegisterPtr AllocateRegister (const semantics::TypePtr& originalType,
                                    const uc::String& name);
    private:
      typedef std::unordered_map<RegisterPtr, unsigned int> AvailabilityMap;
      AvailabilityMap regAvailability;
      AvailabilityMap setAvailability;
      std::unordered_map<RegisterPtr, Frequency> desiredRegFreq;

      unsigned int transferIdentNum;
    };
  } // namespace splitter
} // namespace s1

#endif // __SPLITTER_SEQUENCESPLITTER_H__
