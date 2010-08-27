#ifndef __SPLITTER_SEQUENCESPLITTER_H__
#define __SPLITTER_SEQUENCESPLITTER_H__

#include <boost/unordered_map.hpp>

#include "intermediate/forwarddecl.h"
#include "intermediate/RegisterID.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace splitter
  {
    class SequenceSplitter
    {
      /*
      template<typename Extra>
      class SequenceOpExtender
      {
	template<typename Base>
	class Op : public Base, public Extra
	{
	public:
	  Op() : Base () {}
	  
	  template<typename A1>
	  Op (const A1& a1) : Base (a1) {}
	};
      };
      
      */
      
      typedef intermediate::RegisterID RegisterID;
      typedef intermediate::Sequence Sequence;
      typedef intermediate::SequencePtr SequencePtr;
      typedef intermediate::SequenceOpPtr SequenceOpPtr;
      
      intermediate::SequencePtr inputSeq;
      
      class InputVisitor : public intermediate::SequenceVisitor
      {
	SequenceSplitter& parent;
	
	void PromoteRegister (const RegisterID& reg, int frequency, unsigned int& availability);
	void SplitBinaryOp (const RegisterID& destination, const SequenceOpPtr& op,
			    const RegisterID& source1, const RegisterID& source2);
	int ComputeHighestFreq (const std::vector<RegisterID>& sources);
	unsigned int PromoteAll (int freq, const std::vector<RegisterID>& sources);
	
	void SplitBlock (const SequencePtr& blockSequence,
			 const Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
			 const Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
			 const std::vector<RegisterID>& writtenRegisters,
			 SequenceOpPtr* newSequences, bool keepEmpty = false);
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
		      const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
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
      enum Frequency
      {
	freqVertex,
	freqFragment,
	
	freqNum,
	freqHighest = freqNum - 1
      };
      enum
      {
	freqFlagV = 1 << freqVertex,
	freqFlagF = 1 << freqFragment,
      };
      
      void SetInputSequence (const intermediate::SequencePtr& sequence)
      { inputSeq = sequence; }
      // TODO: methods to set input param frequencies
      
      void SetLocalRegFreqFlags (const RegisterID& regID, unsigned int freqFlags)
      { regAvailability[regID] = freqFlags; }
      
      void PerformSplit ();
      
      intermediate::SequencePtr GetOutputVertexSequence ()
      { return outputSeq[freqVertex]; }
      intermediate::SequencePtr GetOutputFragmentSequence ()
      { return outputSeq[freqFragment]; }
    protected:
      intermediate::SequencePtr outputSeq[freqNum];
      
      boost::unordered_map<RegisterID, unsigned int> regAvailability;
      std::vector<RegisterID> transferRegs[freqNum-1];
    };
  } // namespace splitter
} // namespace s1

#endif // __SPLITTER_SEQUENCESPLITTER_H__
