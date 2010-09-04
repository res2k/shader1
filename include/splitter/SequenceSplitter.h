#ifndef __SPLITTER_SEQUENCESPLITTER_H__
#define __SPLITTER_SEQUENCESPLITTER_H__

#include <boost/unordered_map.hpp>

#include "intermediate/forwarddecl.h"
#include "intermediate/RegisterID.h"
#include "intermediate/SequenceVisitor.h"

#include "Frequency.h"

namespace s1
{
  namespace splitter
  {
    class ProgramSplitter;

    class SequenceSplitter
    {
      typedef intermediate::RegisterID RegisterID;
      typedef intermediate::Sequence Sequence;
      typedef intermediate::SequencePtr SequencePtr;
      typedef intermediate::SequenceOpPtr SequenceOpPtr;
      
      intermediate::SequencePtr inputSeq;
      
      typedef std::tr1::unordered_map<UnicodeString, unsigned int> ParamMap;
      ParamMap paramFlags;
      
      class InputVisitor : public intermediate::SequenceVisitor
      {
	SequenceSplitter& parent;
	
	void PromoteRegister (const RegisterID& reg, int frequency, unsigned int& availability);
	void SplitBinaryOp (const RegisterID& destination, const SequenceOpPtr& op,
			    const RegisterID& source1, const RegisterID& source2);
	int ComputeHighestFreq (const std::vector<RegisterID>& sources);
	unsigned int PromoteAll (int freq, const std::vector<RegisterID>& sources);
	
	void AddOpToSequences (const SequenceOpPtr& op, unsigned int freqMask);
	
	typedef std::pair<RegisterID, RegisterID> LoopedReg;
	typedef std::vector<LoopedReg> LoopedRegs;
	void SplitBlock (const SequencePtr& blockSequence,
			 const Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
			 const Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
			 const std::vector<RegisterID>& writtenRegisters,
			 SequenceOpPtr* newSequences,
			 const LoopedRegs& loopedRegs = LoopedRegs(),
			 bool keepEmpty = false);
      public:
	InputVisitor (SequenceSplitter& parent);
	
	void OpConstBool (const RegisterID& destination,
			  bool value);
	void OpConstInt (const RegisterID& destination,
			 int value);
	void OpConstUInt (const RegisterID& destination,
			  unsigned int value);
	void OpConstFloat (const RegisterID& destination,
			   float value);
				  
	void OpAssign (const RegisterID& destination,
		       const RegisterID& source);
				  
	void OpCast (const RegisterID& destination,
		     BaseType destType,
		     const RegisterID& source);
				    
	void OpMakeVector (const RegisterID& destination,
			   BaseType compType,
			   const std::vector<RegisterID>& sources);

	void OpMakeMatrix (const RegisterID& destination,
			   BaseType compType,
			   unsigned int matrixRows, unsigned int matrixCols,
			   const std::vector<RegisterID>& sources);

	void OpMakeArray (const RegisterID& destination,
			  const std::vector<RegisterID>& sources);
	void OpExtractArrayElement (const RegisterID& destination,
				    const RegisterID& source,
				    const RegisterID& index);
	void OpChangeArrayElement (const RegisterID& destination,
				   const RegisterID& source,
				   const RegisterID& index,
				   const RegisterID& newValue);
	void OpGetArrayLength (const RegisterID& destination,
			       const RegisterID& array);

	void OpExtractVectorComponent (const RegisterID& destination,
				       const RegisterID& source,
				       unsigned int comp);
				  
	void OpArith (const RegisterID& destination,
		      ArithmeticOp op,
		      const RegisterID& source1,
		      const RegisterID& source2);

	void OpCompare (const RegisterID& destination,
			CompareOp op,
			const RegisterID& source1,
			const RegisterID& source2);
				  
	void OpLogic (const RegisterID& destination,
		      LogicOp op,
		      const RegisterID& source1,
		      const RegisterID& source2);

	void OpUnary (const RegisterID& destination,
		      UnaryOp op,
		      const RegisterID& source);
				
	void OpBlock (const SequencePtr& subSequence,
		      const Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
		      const Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
		      const std::vector<RegisterID>& writtenRegisters);
			      
	void OpBranch (const RegisterID& conditionReg,
		       const SequenceOpPtr& seqOpIf,
		       const SequenceOpPtr& seqOpElse);
	void OpWhile (const RegisterID& conditionReg,
		      const LoopedRegs& loopedRegs,
		      const SequenceOpPtr& seqOpBody);
			      
	void OpReturn (const RegisterID& retValReg);
	void OpFunctionCall (const RegisterID& destination,
			     const UnicodeString& funcIdent,
			     const std::vector<RegisterID>& inParams,
			     const std::vector<RegisterID>& outParams);
	void OpBuiltinCall (const RegisterID& destination,
			    intermediate::BuiltinFunction what,
			    const std::vector<RegisterID>& inParams);
      };
    public:
      SequenceSplitter (ProgramSplitter& progSplit);
      
      void SetInputSequence (const intermediate::SequencePtr& sequence)
      { inputSeq = sequence; }
      
      // set input param frequencies
      void SetInputFreqFlags (const UnicodeString& input, unsigned int freqFlags);
      void SetLocalRegFreqFlags (const RegisterID& regID, unsigned int freqFlags)
      { setAvailability[regID] = freqFlags; }
      unsigned int GetLocalRegFreqFlags (const RegisterID& regID)
      { return setAvailability[regID]; }
      
      void PerformSplit ();
      
      const std::vector<RegisterID>& GetTransferRegs (int srcFreq) const
      { return transferRegs[srcFreq]; }
      
      intermediate::SequencePtr GetOutputVertexSequence ()
      { return outputSeq[freqVertex]; }
      intermediate::SequencePtr GetOutputFragmentSequence ()
      { return outputSeq[freqFragment]; }
    protected:
      ProgramSplitter& progSplit;
      
      intermediate::SequencePtr outputSeq[freqNum];
      
      /** Registers to transfer to next frequency.
          The registers in element 0 need to be transferred from frequency 0 to frequency 1,
          the registers in element 1 from frequency 2 to frequency 2 and so on. */
      std::vector<RegisterID> transferRegs[freqNum-1];
      
      unsigned int GetRegAvailability (const RegisterID& reg);
      void SetRegAvailability (const RegisterID& regID, unsigned int freqFlags)
      { regAvailability[regID] = freqFlags; }
      
      UnicodeString GetTransferIdent ();
    private:
      typedef boost::unordered_map<RegisterID, unsigned int> AvailabilityMap;
      AvailabilityMap regAvailability;
      AvailabilityMap setAvailability;
      
      unsigned int transferIdentNum;
    };
  } // namespace splitter
} // namespace s1

#endif // __SPLITTER_SEQUENCESPLITTER_H__
