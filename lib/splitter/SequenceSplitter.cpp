#include "base/common.h"
#include "base/hash_UnicodeString.h"

#include "splitter/SequenceSplitter.h"

#include "intermediate/SequenceOp/SequenceOpArith.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceOp/SequenceOpBranch.h"
#include "intermediate/SequenceOp/SequenceOpBuiltinCall.h"
#include "intermediate/SequenceOp/SequenceOpCast.h"
#include "intermediate/SequenceOp/SequenceOpChangeArrayElement.h"
#include "intermediate/SequenceOp/SequenceOpCompare.h"
#include "intermediate/SequenceOp/SequenceOpConst.h"
#include "intermediate/SequenceOp/SequenceOpExtractArrayElement.h"
#include "intermediate/SequenceOp/SequenceOpExtractVectorComponent.h"
#include "intermediate/SequenceOp/SequenceOpGetArrayLength.h"
#include "intermediate/SequenceOp/SequenceOpLogic.h"
#include "intermediate/SequenceOp/SequenceOpMakeArray.h"
#include "intermediate/SequenceOp/SequenceOpMakeMatrix.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"
#include "intermediate/SequenceOp/SequenceOpReturn.h"
#include "intermediate/SequenceOp/SequenceOpUnaryOp.h"
#include "intermediate/SequenceOp/SequenceOpWhile.h"

#include <boost/make_shared.hpp>
#include <iostream>

namespace s1
{
  namespace splitter
  {
    SequenceSplitter::InputVisitor::InputVisitor (SequenceSplitter& parent)
     : parent (parent)
    {
    }
    
    void SequenceSplitter::InputVisitor::PromoteRegister (const RegisterID& reg, int frequency,
							  unsigned int& availability)
    {
      int f = 0;
      // Find lowest frequency the reg is available in
      while ((f < freqNum) && ((availability & (1 << f)) == 0))
	f++;
      // Sanity checks
      assert (f < freqNum);
      assert (f <= frequency);
      // Promote register until the requested frequence is reached
      while (f < frequency)
      {
	parent.transferRegs[f].push_back (reg);
	f++;
	availability |= (1 << f);
      }
      parent.SetRegAvailability (reg, availability);
    }
    
    void SequenceSplitter::InputVisitor::SplitBinaryOp (const RegisterID& destination,
							const SequenceOpPtr& op,
							const RegisterID& source1,
							const RegisterID& source2)
    {
      unsigned int src1Avail = parent.GetRegAvailability (source1);
      unsigned int src2Avail = parent.GetRegAvailability (source2);
      
      // Determine highest computation frequency
      int highestFreq = freqNum;
      while (highestFreq-- > 0)
      {
	if (((src1Avail & (1 << highestFreq)) != 0)
	    || ((src2Avail & (1 << highestFreq)) != 0))
	{
	  break;
	}
      }
      assert (highestFreq >= 0);
      
      // Make sure each source reg is also available in the highest frequency
      PromoteRegister (source1, highestFreq, src1Avail);
      PromoteRegister (source2, highestFreq, src2Avail);
      
      unsigned int destAvail = 0;
      // Add operation to all frequencies that are supported by both operands
      for (int f = 0; f < freqNum; f++)
      {
	if (((src1Avail & (1 << f)) == 0) || ((src2Avail & (1 << f)) == 0))
	  continue;
	parent.outputSeq[f]->AddOp (op);
	destAvail |= 1 << f;
      }
      parent.SetRegAvailability (destination, destAvail);
    }
    
    int SequenceSplitter::InputVisitor::ComputeHighestFreq (const std::vector<RegisterID>& sources)
    {
      bool found = false;
      int highestFreq = freqNum;
      while (!found && (highestFreq-- > 0))
      {
	for (std::vector<RegisterID>::const_iterator it (sources.begin());
	     it != sources.end();
	     ++it)
	{
	  unsigned int srcAvail = parent.GetRegAvailability (*it);
	  if ((srcAvail & (1 << highestFreq)) != 0)
	  {
	    found = true;
	    break;
	  }
	}
      }
      assert (highestFreq >= 0);
      
      return highestFreq;
    }
    
    unsigned int SequenceSplitter::InputVisitor::PromoteAll (int freq, const std::vector<RegisterID>& sources)
    {
      unsigned int commonFreqs = (1 << freqNum) - 1;
      for (std::vector<RegisterID>::const_iterator it (sources.begin());
	    it != sources.end();
	    ++it)
      {
	unsigned int srcAvail = parent.GetRegAvailability (*it);
	PromoteRegister (*it, freq, srcAvail);
	parent.SetRegAvailability (*it, srcAvail);
	commonFreqs &= srcAvail;
      }
      
      assert ((commonFreqs & (1 << freq)) != 0);
      
      return commonFreqs;
    }

    void SequenceSplitter::InputVisitor::AddOpToSequences (const SequenceOpPtr& op, unsigned int freqMask)
    {
      for (int f = 0; f < freqNum; f++)
      {
	if (freqMask & (1 << f)) parent.outputSeq[f]->AddOp (op);
      }
    }
    
    void SequenceSplitter::InputVisitor::OpConstBool (const RegisterID& destination,
						      bool value)
    {
      parent.SetRegAvailability (destination, freqFlagU);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpConst> (destination, value));
      AddOpToSequences (newSeqOp, freqFlagU);
    }
			  
    void SequenceSplitter::InputVisitor::OpConstInt (const RegisterID& destination,
						     int value)
    {
      parent.SetRegAvailability (destination, freqFlagU);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpConst> (destination, value));
      AddOpToSequences (newSeqOp, freqFlagU);
    }
			  
    void SequenceSplitter::InputVisitor::OpConstUInt (const RegisterID& destination,
						      unsigned int value)
    {
      parent.SetRegAvailability (destination, freqFlagU);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpConst> (destination, value));
      AddOpToSequences (newSeqOp, freqFlagU);
    }
			  
    void SequenceSplitter::InputVisitor::OpConstFloat (const RegisterID& destination,
						       float value)
    {
      parent.SetRegAvailability (destination, freqFlagU);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpConst> (destination, value));
      AddOpToSequences (newSeqOp, freqFlagU);
    }
    
    void SequenceSplitter::InputVisitor::OpAssign (const RegisterID& destination,
						   const RegisterID& source)
    {
      unsigned int srcAvail = parent.GetRegAvailability (source);
      parent.SetRegAvailability (destination, srcAvail);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpAssign> (destination, source));
      AddOpToSequences (newSeqOp, srcAvail);
    }
    
    static inline intermediate::BasicType ToBasicType (intermediate::SequenceVisitor::BaseType t)
    {
      switch (t)
      {
      case intermediate::SequenceVisitor::Bool:		return intermediate::Bool;
      case intermediate::SequenceVisitor::Int:		return intermediate::Int;
      case intermediate::SequenceVisitor::UInt:		return intermediate::UInt;
      case intermediate::SequenceVisitor::Float:	return intermediate::Float;
      }
      
      assert (false);
      return intermediate::Int;
    }
			  
    void SequenceSplitter::InputVisitor::OpCast (const RegisterID& destination,
						 BaseType destType,
						 const RegisterID& source)
    {
      unsigned int srcAvail = parent.GetRegAvailability (source);
      parent.SetRegAvailability (destination, srcAvail);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpCast> (destination,
										ToBasicType (destType),
										source));
      AddOpToSequences (newSeqOp, srcAvail);
    }
    
    void SequenceSplitter::InputVisitor::OpMakeVector (const RegisterID& destination,
						       BaseType compType,
						       const std::vector<RegisterID>& sources)
    {
      int highestFreq = ComputeHighestFreq (sources);
      unsigned int commonFreqs = PromoteAll (highestFreq, sources);
      
      parent.SetRegAvailability (destination, commonFreqs);
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpMakeVector> (destination,
										      ToBasicType (compType),
										      sources));
      AddOpToSequences (newSeqOp, commonFreqs);
    }
    
    void SequenceSplitter::InputVisitor::OpMakeMatrix (const RegisterID& destination,
						       BaseType compType,
						       unsigned int matrixRows, unsigned int matrixCols,
						       const std::vector<RegisterID>& sources)
    {
      int highestFreq = ComputeHighestFreq (sources);
      unsigned int commonFreqs = PromoteAll (highestFreq, sources);
      
      parent.SetRegAvailability (destination, commonFreqs);
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpMakeMatrix> (destination,
										      ToBasicType (compType), 
										      matrixRows, matrixCols,
										      sources));
      AddOpToSequences (newSeqOp, commonFreqs);
    }
    
    void SequenceSplitter::InputVisitor::OpMakeArray (const RegisterID& destination,
						      const std::vector<RegisterID>& sources)
    {
      int highestFreq = ComputeHighestFreq (sources);
      unsigned int commonFreqs = PromoteAll (highestFreq, sources);
      
      parent.SetRegAvailability (destination, commonFreqs);
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpMakeArray> (destination, sources));
      AddOpToSequences (newSeqOp, commonFreqs);
    }
		      
    void SequenceSplitter::InputVisitor::OpExtractArrayElement (const RegisterID& destination,
								const RegisterID& source,
								const RegisterID& index)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpExtractArrayElement> (destination, source, index));
      SplitBinaryOp (destination, newSeqOp, source, index);
    }
    
    void SequenceSplitter::InputVisitor::OpChangeArrayElement (const RegisterID& destination,
							       const RegisterID& source,
							       const RegisterID& index,
							       const RegisterID& newValue)
    {
      std::vector<RegisterID> sourceRegs;
      sourceRegs.push_back (source);
      sourceRegs.push_back (index);
      sourceRegs.push_back (newValue);
      int highestFreq = ComputeHighestFreq (sourceRegs);
      unsigned int commonFreqs = PromoteAll (highestFreq, sourceRegs);
      
      parent.SetRegAvailability (destination, commonFreqs);
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpChangeArrayElement> (destination, source,
											      index, newValue));
      AddOpToSequences (newSeqOp, commonFreqs);
    }
    
    void SequenceSplitter::InputVisitor::OpGetArrayLength (const RegisterID& destination,
							   const RegisterID& array)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpGetArrayLength> (destination, array));
      
      unsigned int srcAvail = parent.GetRegAvailability (array);
      AddOpToSequences (newSeqOp, srcAvail);
      parent.SetRegAvailability (destination, srcAvail);
    }

    void SequenceSplitter::InputVisitor::OpExtractVectorComponent (const RegisterID& destination,
								   const RegisterID& source,
								   unsigned int comp)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpExtractVectorComponent> (destination, source, comp));
      
      unsigned int srcAvail = parent.GetRegAvailability (source);
      AddOpToSequences (newSeqOp, srcAvail);
      parent.SetRegAvailability (destination, srcAvail);
    }
    
    void SequenceSplitter::InputVisitor::OpArith (const RegisterID& destination,
						  ArithmeticOp op,
						  const RegisterID& source1,
						  const RegisterID& source2)
    {
      /*
	The criterium for whether we can run an op at vertex frequency, or have to run it
	at fragment frequency, is 'interpolatibility'.
	Consider: 'forwarding' a value from V to F program usually means it's linearly
	interpolated. A operation can be safely executed at vertex freq when the
	interpolated result of the operation would be the same as the result of the operation
	with interpolated operands (lerp (op (x_1, y_1), op (x_2, y_2), f)
	  == op (lerp (x_1, x_2, f), lerp (y_1, y_2, f)).
       */
      
      bool lerpSafe;
      
      unsigned int src1Avail = parent.GetRegAvailability (source1);
      unsigned int src2Avail = parent.GetRegAvailability (source2);
      
      switch (op)
      {
      case Add:
      case Sub:
	/* Addition, subtraction are interpolation-safe */
	lerpSafe = true;	
	break;
      case Mul:
      case Div:
	{
	  if (((src1Avail & freqFlagU) != 0) || ((src2Avail & freqFlagU) != 0))
	  {
	    /* Multiplication/Division with uniform is safe */
	    lerpSafe = true;	
	  }
	  else
	  {
	    /* If both operands are of at least vertex freq, need to compute at fragment freq */
	    lerpSafe = false;
	  }
	}
	break;
      case Mod:
	/* Mod isn't interpolation safe at all */
	lerpSafe = false;	
	break;
      }
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpArith> (destination, op, source1, source2));
      if (lerpSafe)
      {
	SplitBinaryOp (destination, newSeqOp, source1, source2);
      }
      else
      {
	// Promote inputs to fragment freq
	PromoteRegister (source1, freqFragment, src1Avail);
	PromoteRegister (source2, freqFragment, src2Avail);
	
	// Add operation to fragment frequency
	parent.outputSeq[freqFragment]->AddOp (newSeqOp);
	parent.SetRegAvailability (destination, freqFlagF);
      }
    }
    
    void SequenceSplitter::InputVisitor::OpCompare (const RegisterID& destination,
						    CompareOp op,
						    const RegisterID& source1,
						    const RegisterID& source2)
    {
      // Interpolation safety: comparison ops really aren't
      unsigned int src1Avail = parent.GetRegAvailability (source1);
      unsigned int src2Avail = parent.GetRegAvailability (source2);
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpCompare> (destination, op, source1, source2));
      PromoteRegister (source1, freqFragment, src1Avail);
      PromoteRegister (source2, freqFragment, src2Avail);
      
      // Add operation to fragment frequency
      parent.outputSeq[freqFragment]->AddOp (newSeqOp);
      parent.SetRegAvailability (destination, freqFlagF);
    }
			      
    void SequenceSplitter::InputVisitor::OpLogic (const RegisterID& destination,
						  LogicOp op,
						  const RegisterID& source1,
						  const RegisterID& source2)
    {
      // Interpolation safety: logic ops really aren't
      unsigned int src1Avail = parent.GetRegAvailability (source1);
      unsigned int src2Avail = parent.GetRegAvailability (source2);
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpLogic> (destination, op, source1, source2));
      PromoteRegister (source1, freqFragment, src1Avail);
      PromoteRegister (source2, freqFragment, src2Avail);
      
      // Add operation to fragment frequency
      parent.outputSeq[freqFragment]->AddOp (newSeqOp);
      parent.SetRegAvailability (destination, freqFlagF);
    }
    
    void SequenceSplitter::InputVisitor::OpUnary (const RegisterID& destination,
						  UnaryOp op,
						  const RegisterID& source)
    {
      bool lerpSafe;
      
      switch (op)
      {
      case Neg:
      case Not:
	/* Negation/logical NOT are interpolation-safe */
	lerpSafe = true;	
	break;
      case Inv:
	/* Bit-wise invert isn't */
	lerpSafe = false;	
	break;
      }
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpUnaryOp> (destination, op, source));
      unsigned int srcAvail = parent.GetRegAvailability (source);
      if (lerpSafe)
      {
	AddOpToSequences (newSeqOp, srcAvail);
	parent.SetRegAvailability (destination, srcAvail);
      }
      else
      {
	// Promote inputs to fragment freq
	PromoteRegister (source, freqFragment, srcAvail);
	
	// Add operation to fragment frequency
	parent.outputSeq[freqFragment]->AddOp (newSeqOp);
	parent.SetRegAvailability (destination, freqFlagF);
      }
    }
    
    void SequenceSplitter::InputVisitor::SplitBlock (const SequencePtr& blockSequence,
						     const Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
						     const Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
						     const std::vector<RegisterID>& writtenRegisters,
						     SequenceOpPtr* newSequences, bool keepEmpty)
    {
      SequenceSplitter blockSplitter;
      blockSplitter.SetInputSequence (blockSequence);
      
      // Forward frequencies to subSeqSplitter
      std::vector<RegisterID> newReadRegisters[freqNum];
      for (Sequence::IdentifierToRegIDMap::const_iterator imports = identToRegIDs_imp.begin();
	   imports != identToRegIDs_imp.end();
	   ++imports)
      {
	RegisterID reg (parent.inputSeq->GetIdentifierRegisterID (imports->first));
	RegisterID reg_subseq (imports->second);
	
	unsigned int reg_subseqAvail = parent.GetRegAvailability (reg);
	blockSplitter.SetLocalRegFreqFlags (reg_subseq, reg_subseqAvail);
	for (int f = 0; f < freqNum; f++)
	{
	  if ((reg_subseqAvail & (1 << f)) == 0) continue;
	  newReadRegisters[f].push_back (reg);
	}
      }      
      
      blockSplitter.PerformSplit();
      
      // Compute new writtenRegisters, update availability flags
      std::vector<RegisterID> newWrittenRegisters[freqNum];
      for (Sequence::IdentifierToRegIDMap::const_iterator exports = identToRegIDs_exp.begin();
	   exports != identToRegIDs_exp.end();
	   ++exports)
      {
	RegisterID reg (parent.inputSeq->GetIdentifierRegisterID (exports->first));
	RegisterID reg_subseq (exports->second);
	
	unsigned int reg_subseqAvail = blockSplitter.GetRegAvailability (reg_subseq);
	for (int f = 0; f < freqNum; f++)
	{
	  if ((reg_subseqAvail & (1 << f)) == 0) continue;
	  newWrittenRegisters[f].push_back (reg);
	}
	parent.SetRegAvailability (reg, reg_subseqAvail);
      }      
      
      for (int f = 0; f < freqNum; f++)
      {
	if (!keepEmpty && (blockSplitter.outputSeq[f]->GetNumOps() == 0)) continue;
	SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpBlock> (blockSplitter.outputSeq[f],
										   identToRegIDs_imp,
										   identToRegIDs_exp,
										   newReadRegisters[f],
										   newWrittenRegisters[f]));
	newSequences[f] = newSeqOp;
      }
    }
    
    void SequenceSplitter::InputVisitor::OpBlock (const SequencePtr& subSequence,
						  const Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
						  const Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
						  const std::vector<RegisterID>& writtenRegisters)
    {
      SequenceOpPtr newSeqOps[freqNum];
      SplitBlock (subSequence, identToRegIDs_imp, identToRegIDs_exp, writtenRegisters, newSeqOps);
      
      for (int f = 0; f < freqNum; f++)
      {
	if (!newSeqOps[f]) continue;
	parent.outputSeq[f]->AddOp (newSeqOps[f]);
      }
    }
    
    void SequenceSplitter::InputVisitor::OpBranch (const RegisterID& conditionReg,
						   const SequenceOpPtr& seqOpIf,
						   const SequenceOpPtr& seqOpElse)
    {
      boost::shared_ptr<intermediate::SequenceOpBlock> ifBlock (
	boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (seqOpIf));
      assert (ifBlock);
      boost::shared_ptr<intermediate::SequenceOpBlock> elseBlock (
	boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (seqOpElse));
      assert (elseBlock);
      
      // Compute highest frequency of condition and all sequence inputs
      std::vector<RegisterID> allInputs;
      allInputs.push_back (conditionReg);
      {
	const Sequence::IdentifierToRegIDMap& identToRegIDs_imp (ifBlock->GetImportIdentToRegs());
	for (Sequence::IdentifierToRegIDMap::const_iterator imports = identToRegIDs_imp.begin();
	     imports != identToRegIDs_imp.end();
	     ++imports)
	{
	  allInputs.push_back (parent.inputSeq->GetIdentifierRegisterID (imports->first));
	}      
      }
      {
	const Sequence::IdentifierToRegIDMap& identToRegIDs_imp (elseBlock->GetImportIdentToRegs());
	for (Sequence::IdentifierToRegIDMap::const_iterator imports = identToRegIDs_imp.begin();
	     imports != identToRegIDs_imp.end();
	     ++imports)
	{
	  allInputs.push_back (parent.inputSeq->GetIdentifierRegisterID (imports->first));
	}      
      }
      
      // Propagate condition and all sequence inputs to highest frequency
      int highestFreq = ComputeHighestFreq (allInputs);
      unsigned int commonFreqs = PromoteAll (highestFreq, allInputs);
      
      // Output branch op to frequencies supported by condition and all sequence inputs
      SequenceOpPtr newIfOps[freqNum];
      {
	std::vector<RegisterID> writtenRegs;
	writtenRegs.insert (writtenRegs.begin(),
			    ifBlock->GetWrittenRegisters().begin(),
			    ifBlock->GetWrittenRegisters().end());
	SplitBlock (ifBlock->GetSequence(),
		    ifBlock->GetImportIdentToRegs(),
		    ifBlock->GetExportIdentToRegs(),
		    writtenRegs, newIfOps, true);
      }
      SequenceOpPtr newElseOps[freqNum];
      {
	std::vector<RegisterID> writtenRegs;
	writtenRegs.insert (writtenRegs.begin(),
			    elseBlock->GetWrittenRegisters().begin(),
			    elseBlock->GetWrittenRegisters().end());
	SplitBlock (elseBlock->GetSequence(),
		    elseBlock->GetImportIdentToRegs(),
		    elseBlock->GetExportIdentToRegs(),
		    writtenRegs, newIfOps, true);
      }
      /* @@@ CHECK: Registers are supposed to be written to in both branches.
         Frequencies of those regs should be intersection of frequencies
         from _both_ branches, */
      for (int f = 0; f < freqNum; f++)
      {
	//if (!newSeqOps[f]) continue;
	if ((commonFreqs & (1 << f)) == 0) continue;
	assert(newIfOps[f] != 0);
	assert(newElseOps[f] != 0);
	
	SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpBranch> (conditionReg,
										    newIfOps[f],
										    newElseOps[f]));
	parent.outputSeq[f]->AddOp (newSeqOp);
      }
    }
    
    void SequenceSplitter::InputVisitor::OpWhile (const RegisterID& conditionReg,
						  const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
						  const SequenceOpPtr& seqOpBody)
    {
      boost::shared_ptr<intermediate::SequenceOpBlock> body (
	boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (seqOpBody));
      assert (body);

      // Compute highest frequency of condition and all loop inputs
      std::vector<RegisterID> allInputs;
      allInputs.push_back (conditionReg);
      {
	for (std::vector<std::pair<RegisterID, RegisterID> >::const_iterator loopedReg = loopedRegs.begin();
	     loopedReg != loopedRegs.end();
	     ++loopedReg)
	{
	  allInputs.push_back (loopedReg->first);
	}      
      }
      // Propagate condition and all sequence inputs to highest frequency
      /* @@@ Force highest frequency to cover the case where a looped reg gets a higher
	 frequency after a loop */
      int highestFreq = freqHighest/*ComputeHighestFreq (allInputs)*/;
      unsigned int commonFreqs = PromoteAll (highestFreq, allInputs);
      
      // Output branch op to frequencies supported by condition and all sequence inputs
      SequenceOpPtr newOps[freqNum];
      {
	std::vector<RegisterID> writtenRegs;
	writtenRegs.insert (writtenRegs.begin(),
			    body->GetWrittenRegisters().begin(),
			    body->GetWrittenRegisters().end());
	SplitBlock (body->GetSequence(),
		    body->GetImportIdentToRegs(),
		    body->GetExportIdentToRegs(),
		    writtenRegs, newOps, true);
      }
      // TODO: Could filter looped regs
      for (int f = 0; f < freqNum; f++)
      {
	if ((commonFreqs & (1 << f)) == 0) continue;
	assert(newOps[f] != 0);
	
	SequenceOpPtr newWhileOp (boost::make_shared<intermediate::SequenceOpWhile> (conditionReg,
										     loopedRegs,
										     newOps[f]));
	parent.outputSeq[f]->AddOp (newWhileOp);
      }
    }
    
    void SequenceSplitter::InputVisitor::OpReturn (const RegisterID& retValReg)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpReturn> (retValReg));
      
      /* 'return': only execute on fragment frequency */
      if (retValReg.IsValid())
      {
	unsigned int srcAvail = parent.GetRegAvailability (retValReg);
	PromoteRegister (retValReg, freqFragment, srcAvail);
      }
      parent.outputSeq[freqFragment]->AddOp (newSeqOp);
    }
    
    void SequenceSplitter::InputVisitor::OpFunctionCall (const RegisterID& destination,
							 const UnicodeString& funcIdent,
							 const std::vector<RegisterID>& inParams,
							 const std::vector<RegisterID>& outParams)
    {
      /*
        This is complicated:
        To know the frequencies of the output params/return value, we have to analyze
        the function body, taking into account the frequencies of the input params
        (very likely the output depends on the input)...
       
        Recursion:
        When coming upon an input frequency combination that is currently split,
        guess the frequencies of output parameters by choosing "all" frequencies...
       */
    }
			  
    void SequenceSplitter::InputVisitor::OpBuiltinCall (const RegisterID& destination,
							intermediate::BuiltinFunction what,
							const std::vector<RegisterID>& inParams)
    {
      bool lerpSafe;
      switch (what)
      {
      case intermediate::dot:
      case intermediate::cross:
      case intermediate::mul:
	/* dot, cross, matrix mul: interpolation-safe if one operand is an uniform */
	{
	  unsigned int src1Avail = parent.GetRegAvailability (inParams[0]);
	  unsigned int src2Avail = parent.GetRegAvailability (inParams[1]);
	  lerpSafe = ((src1Avail & freqFlagU) != 0) || ((src2Avail & freqFlagU) != 0);
	}
	break;
      case intermediate::normalize:
      case intermediate::length:
      case intermediate::min:
      case intermediate::max:
      case intermediate::pow:
      case intermediate::tex1D:
      case intermediate::tex2D:
      case intermediate::tex3D:
      case intermediate::texCUBE:
	/* All others: not interpolation-safe */
	break;
      }
      
      if (lerpSafe)
      {
	int highestFreq = ComputeHighestFreq (inParams);
	unsigned int commonFreqs = PromoteAll (highestFreq, inParams);
	
	parent.SetRegAvailability (destination, commonFreqs);
	SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpBuiltinCall> (destination,
											  what,
											  inParams));
	AddOpToSequences (newSeqOp, commonFreqs);
      }
      else
      {
	PromoteAll (freqFragment, inParams);
	
	parent.SetRegAvailability (destination, freqFlagF);
	SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpBuiltinCall> (destination,
											  what,
											  inParams));
	parent.outputSeq[freqFragment]->AddOp (newSeqOp);
      }
    }
    
    //-----------------------------------------------------------------------
    
    void SequenceSplitter::SetInputFreqFlags (const UnicodeString& input, unsigned int freqFlags)
    {
      paramFlags[input] = freqFlags;
    }
    
    void SequenceSplitter::PerformSplit ()
    {
      regAvailability.clear();
      transferRegs[0].clear();
      for (int f = 0; f < freqNum; f++)
      {
	outputSeq[f] = boost::make_shared<intermediate::Sequence> ();
	outputSeq[f]->CopyRegisterBanks (inputSeq);
	
      }
      for (intermediate::Sequence::RegisterImpMappings::const_iterator impMap = inputSeq->GetImports().begin();
	   impMap != inputSeq->GetImports().end();
	   ++impMap)
      {
	for (int f = 0; f < freqNum; f++)
	{
	  outputSeq[f]->AddImport (impMap->first, impMap->second);
	}
	
	ParamMap::const_iterator inputFreq = paramFlags.find (impMap->first);
	if (inputFreq != paramFlags.end())
	{
	  Sequence::IdentifierToRegIDMap::const_iterator toReg =
	    inputSeq->GetIdentifierToRegisterIDMap().find (inputFreq->first);
	  if (toReg != inputSeq->GetIdentifierToRegisterIDMap().end())
	  {
	    regAvailability[toReg->second] = inputFreq->second;
	  }
	}
      }
      for (intermediate::Sequence::RegisterExpMappings::const_iterator expMap = inputSeq->GetExports().begin();
	   expMap != inputSeq->GetExports().end();
	   ++expMap)
      {
	for (int f = 0; f < freqNum; f++)
	{
	  outputSeq[f]->SetExport (expMap->first, expMap->second);
	}
      }
	
      InputVisitor visitor (*this);
      
      inputSeq->Visit (visitor);
      
      // Insert 'uniform' sequence at start of both V/F sequences
      for (size_t i = 0; i < outputSeq[freqUniform]->GetNumOps(); i++)
      {
	SequenceOpPtr uniOp (outputSeq[freqUniform]->GetOp (i));
	outputSeq[freqVertex]->InsertOp (i, uniOp);
	outputSeq[freqFragment]->InsertOp (i, uniOp);
      }
    }
    
    static int GetDefaultFrequencyForType (const parser::SemanticsHandler::TypePtr& type)
    {
      switch (type->GetTypeClass())
      {
      case parser::SemanticsHandler::Type::Base:
	return freqVertex;
      case parser::SemanticsHandler::Type::Sampler:
	return freqFragment;
      case parser::SemanticsHandler::Type::Array:
	return GetDefaultFrequencyForType (type->GetArrayVectorMatrixBaseType());
      case parser::SemanticsHandler::Type::Vector:
	return freqVertex;
      case parser::SemanticsHandler::Type::Matrix:
	return freqUniform;
      }
      assert (false);
      return 0;
    }
    
    unsigned int SequenceSplitter::GetRegAvailability (const RegisterID& reg)
    {
      AvailabilityMap::const_iterator avail = regAvailability.find (reg);
      if (avail == regAvailability.end())
      {
	intermediate::Sequence::RegisterBankPtr regBankPtr;
	intermediate::Sequence::RegisterPtr regPtr = inputSeq->QueryRegisterPtrFromID (reg, regBankPtr);
	
	int defaultFreq = GetDefaultFrequencyForType (regBankPtr->GetOriginalType());
	
	unsigned int defaultAvail = 1 << defaultFreq;
	regAvailability[reg] = defaultAvail;
	
	const char* const defFreqName[freqNum] = { "uniform",  "vertex", "fragment" };
	const UnicodeString& regName = regPtr->GetName();
	std::string regNameStr;
	regName.toUTF8String (regNameStr);
	// FIXME: use a special 'warning' mechanism for this
	std::cerr << "Register " << regNameStr << " has no associated availability, using '"
	  << defFreqName[defaultFreq] << "'" << std::endl;
	return defaultAvail;
      }
      return avail->second;
    }
  } // namespace splitter
} // namespace s1
