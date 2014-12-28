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

#ifndef __SPLITTER_SEQUENCESPLITTER_H__
#define __SPLITTER_SEQUENCESPLITTER_H__

#include <boost/unordered_map.hpp>

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
      
      typedef boost::unordered_map<UnicodeString, unsigned int> ParamMap;
      ParamMap paramFlags;
      
      class InputVisitor : public intermediate::SequenceVisitor
      {
	SequenceSplitter& parent;
	
	unsigned int PromoteRegister (const RegisterPtr& reg, int frequency);
	void SplitBinaryOp (const RegisterPtr& destination, const SequenceOpPtr& op,
			    const RegisterPtr& source1, const RegisterPtr& source2);
        template<typename Container>
	unsigned int ComputeCombinedFreqs (const Container& sources);
        template<typename Container>
	int ComputeHighestFreq (const Container& sources);
        template<typename Container>
	unsigned int PromoteAll (int freq, const Container& sources);
	
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
	
	typedef std::pair<RegisterPtr, RegisterPtr> RegisterPair;
	typedef std::vector<RegisterPair> RenamedBranchOutputs;
	void EmitUnconditionalBranchBlock (const char* suffix, const SplitResult& splitRes, int f,
					   RenamedBranchOutputs& outputs);
	SequenceOpPtr AugmentBranchBlockWithRenames (const char* suffix,
						     const SplitResult& splitRes,
						     const RenamedBranchOutputs* renames,
						     int f);
      public:
	InputVisitor (SequenceSplitter& parent);
	
	void SetVisitedOp (const s1::intermediate::SequenceOpPtr& op) {}
	
	void OpConstBool (const RegisterPtr& destination,
			  bool value);
	void OpConstInt (const RegisterPtr& destination,
			 int value);
	void OpConstUInt (const RegisterPtr& destination,
			  unsigned int value);
	void OpConstFloat (const RegisterPtr& destination,
			   float value);
				  
	void OpAssign (const RegisterPtr& destination,
		       const RegisterPtr& source);
				  
	void OpCast (const RegisterPtr& destination,
		     intermediate::BasicType destType,
		     const RegisterPtr& source);
				    
	void OpMakeVector (const RegisterPtr& destination,
			   intermediate::BasicType compType,
			   const std::vector<RegisterPtr>& sources);

	void OpMakeMatrix (const RegisterPtr& destination,
			   intermediate::BasicType compType,
			   unsigned int matrixRows, unsigned int matrixCols,
			   const std::vector<RegisterPtr>& sources);

	void OpMakeArray (const RegisterPtr& destination,
			  const std::vector<RegisterPtr>& sources);
	void OpExtractArrayElement (const RegisterPtr& destination,
				    const RegisterPtr& source,
				    const RegisterPtr& index);
	void OpChangeArrayElement (const RegisterPtr& destination,
				   const RegisterPtr& source,
				   const RegisterPtr& index,
				   const RegisterPtr& newValue);
	void OpGetArrayLength (const RegisterPtr& destination,
			       const RegisterPtr& array);

	void OpExtractVectorComponent (const RegisterPtr& destination,
				       const RegisterPtr& source,
				       unsigned int comp);
				  
	void OpArith (const RegisterPtr& destination,
		      ArithmeticOp op,
		      const RegisterPtr& source1,
		      const RegisterPtr& source2);

	void OpCompare (const RegisterPtr& destination,
			CompareOp op,
			const RegisterPtr& source1,
			const RegisterPtr& source2);
				  
	void OpLogic (const RegisterPtr& destination,
		      LogicOp op,
		      const RegisterPtr& source1,
		      const RegisterPtr& source2);

	void OpUnary (const RegisterPtr& destination,
		      UnaryOp op,
		      const RegisterPtr& source);
				
	void OpBlock (const SequencePtr& subSequence,
		      const Sequence::IdentifierToRegMap& identToRegIDs_imp,
		      const Sequence::IdentifierToRegMap& identToRegIDs_exp);
			      
	void OpBranch (const RegisterPtr& conditionReg,
		       const SequenceOpPtr& seqOpIf,
		       const SequenceOpPtr& seqOpElse);
	void OpWhile (const RegisterPtr& conditionReg,
		      const LoopedRegs& loopedRegs,
		      const SequenceOpPtr& seqOpBody);
			      
	void OpReturn (const std::vector<RegisterPtr>& outParamVals);
	void OpFunctionCall (const UnicodeString& funcIdent,
			     const std::vector<RegisterPtr>& inParams,
			     const std::vector<RegisterPtr>& outParams);
	void OpBuiltinCall (const RegisterPtr& destination,
			    intermediate::BuiltinFunction what,
			    const std::vector<RegisterPtr>& inParams);
      };
    public:
      SequenceSplitter (ProgramSplitter& progSplit, bool mergeUniformToVF = true);
      
      void SetInputSequence (const intermediate::SequencePtr& sequence)
      { inputSeq = sequence; }
      
      // set input param frequencies
      void SetInputFreqFlags (const UnicodeString& input, unsigned int freqFlags);
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
      
      static int GetDefaultFrequencyForType (const parser::SemanticsHandler::TypePtr& type);
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
      
      UnicodeString GetTransferIdent (const UnicodeString& origName = UnicodeString());
      
      /// Allocate register in all output sequences
      RegisterPtr AllocateRegister (const s1::parser::SemanticsHandler::TypePtr& originalType,
				    const UnicodeString& name);
    private:
      typedef boost::unordered_map<RegisterPtr, unsigned int> AvailabilityMap;
      AvailabilityMap regAvailability;
      AvailabilityMap setAvailability;
      
      unsigned int transferIdentNum;
    };
  } // namespace splitter
} // namespace s1

#endif // __SPLITTER_SEQUENCESPLITTER_H__
