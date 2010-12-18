#include "base/common.h"

#include "splitter/SequenceSplitter.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
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
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "intermediate/SequenceOp/SequenceOpGetArrayLength.h"
#include "intermediate/SequenceOp/SequenceOpLogic.h"
#include "intermediate/SequenceOp/SequenceOpMakeArray.h"
#include "intermediate/SequenceOp/SequenceOpMakeMatrix.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"
#include "intermediate/SequenceOp/SequenceOpReturn.h"
#include "intermediate/SequenceOp/SequenceOpUnaryOp.h"
#include "intermediate/SequenceOp/SequenceOpWhile.h"
#include "splitter/ProgramSplitter.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

#include <unicode/ustdio.h>

namespace s1
{
  namespace splitter
  {
    SequenceSplitter::InputVisitor::InputVisitor (SequenceSplitter& parent)
     : parent (parent)
    {
    }
    
    static inline int LowestFreq (unsigned int availabilityFlag)
    {
      int f = __builtin_ffs (availabilityFlag) - 1;
      // Sanity checks
      assert ((f >= 0) && (f < freqNum));
      return f;
    }
    
    static inline int HighestFreq (unsigned int availabilityFlag)
    {
      assert (availabilityFlag != 0);
      int f =  (CHAR_BIT * sizeof (unsigned int) - __builtin_clz (availabilityFlag) - 1);
      // Sanity checks
      assert ((f >= 0) && (f < freqNum));
      return f;
    }
    
    unsigned int SequenceSplitter::InputVisitor::PromoteRegister (const RegisterPtr& reg,
								  int frequency)
    {
      unsigned int availability = parent.GetRegAvailability (reg);
      
      int f = LowestFreq (availability);
      // Sanity checks
      assert (f <= frequency);
      // Promote register until the requested frequence is reached
      while (f < frequency)
      {
	f++;
	if ((availability & (1 << f)) == 0)
	{
	  parent.transferRegs[f-1].push_back (reg);
	}
	availability |= (1 << f);
      }
      parent.SetRegAvailability (reg, availability);
      
      return availability;
    }
    
    void SequenceSplitter::InputVisitor::SplitBinaryOp (const RegisterPtr& destination,
							const SequenceOpPtr& op,
							const RegisterPtr& source1,
							const RegisterPtr& source2)
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
      src1Avail = PromoteRegister (source1, highestFreq);
      src2Avail = PromoteRegister (source2, highestFreq);
      
      /* Hack: If both sources are available in uniform freq,
         _only_ add op to uniform freq - uniform ops will later be
         copied into vertex and fragment ops anyway */
      if (((src1Avail & freqFlagU) != 0) && ((src2Avail & freqFlagU) != 0))
      {
	src1Avail = src2Avail = freqFlagU;
      }
      
      unsigned int destAvail = 0;
      // Add operation to all frequencies that are supported by both operands
      for (int f = 0; f < freqNum; f++)
      {
	if (((src1Avail & (1 << f)) == 0) || ((src2Avail & (1 << f)) == 0))
	  continue;
	parent.outputSeq[f]->AddOp (op);
	destAvail |= 1 << f;
	break;
      }
      parent.SetRegAvailability (destination, destAvail);
    }
    
    unsigned int SequenceSplitter::InputVisitor::ComputeCombinedFreqs (const std::vector<RegisterPtr>& sources)
    {
      unsigned int allFreq = 0;
      for (std::vector<RegisterPtr>::const_iterator it (sources.begin());
	   it != sources.end();
	   ++it)
      {
	unsigned int srcAvail = parent.GetRegAvailability (*it);
	allFreq |= srcAvail;
      }
      return allFreq;
    }
    
    int SequenceSplitter::InputVisitor::ComputeHighestFreq (const std::vector<RegisterPtr>& sources)
    {
      bool found = false;
      int highestFreq = freqNum;
      while (!found && (highestFreq-- > 0))
      {
	for (std::vector<RegisterPtr>::const_iterator it (sources.begin());
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
    
    unsigned int SequenceSplitter::InputVisitor::PromoteAll (int freq, const std::vector<RegisterPtr>& sources)
    {
      unsigned int commonFreqs = (1 << freqNum) - 1;
      for (std::vector<RegisterPtr>::const_iterator it (sources.begin());
	    it != sources.end();
	    ++it)
      {
	unsigned int srcAvail = parent.GetRegAvailability (*it);
	srcAvail = PromoteRegister (*it, freq);
	parent.SetRegAvailability (*it, srcAvail);
	commonFreqs &= srcAvail;
      }
      
      assert ((commonFreqs & (1 << freq)) != 0);
      
      return commonFreqs;
    }

    unsigned int SequenceSplitter::InputVisitor::AddOpToSequences (const SequenceOpPtr& op, unsigned int freqMask)
    {
      /* Hack: If op is _also_ executed in uniform freq,
         _only_ add op to uniform freq - uniform ops will later be
         copied into vertex and fragment ops anyway */
      if (freqMask & freqFlagU) freqMask = freqFlagU;
      
      for (int f = 0; f < freqNum; f++)
      {
	if (freqMask & (1 << f))
	{
	  parent.outputSeq[f]->AddOp (op);
	  return 1 << f;
	}
      }
      assert (false);
      return 0;
    }
    
    void SequenceSplitter::InputVisitor::OpConstBool (const RegisterPtr& destination,
						      bool value)
    {
      parent.SetRegAvailability (destination, freqFlagU);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpConst> (destination, value));
      AddOpToSequences (newSeqOp, freqFlagU);
    }
			  
    void SequenceSplitter::InputVisitor::OpConstInt (const RegisterPtr& destination,
						     int value)
    {
      parent.SetRegAvailability (destination, freqFlagU);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpConst> (destination, value));
      AddOpToSequences (newSeqOp, freqFlagU);
    }
			  
    void SequenceSplitter::InputVisitor::OpConstUInt (const RegisterPtr& destination,
						      unsigned int value)
    {
      parent.SetRegAvailability (destination, freqFlagU);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpConst> (destination, value));
      AddOpToSequences (newSeqOp, freqFlagU);
    }
			  
    void SequenceSplitter::InputVisitor::OpConstFloat (const RegisterPtr& destination,
						       float value)
    {
      parent.SetRegAvailability (destination, freqFlagU);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpConst> (destination, value));
      AddOpToSequences (newSeqOp, freqFlagU);
    }
    
    void SequenceSplitter::InputVisitor::OpAssign (const RegisterPtr& destination,
						   const RegisterPtr& source)
    {
      unsigned int srcAvail = parent.GetRegAvailability (source);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpAssign> (destination, source));
      parent.SetRegAvailability (destination, AddOpToSequences (newSeqOp, srcAvail));
    }
    
    void SequenceSplitter::InputVisitor::OpCast (const RegisterPtr& destination,
						 intermediate::BasicType destType,
						 const RegisterPtr& source)
    {
      unsigned int srcAvail = parent.GetRegAvailability (source);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpCast> (destination,
										destType,
										source));
      parent.SetRegAvailability (destination, AddOpToSequences (newSeqOp, srcAvail));
    }
    
    void SequenceSplitter::InputVisitor::OpMakeVector (const RegisterPtr& destination,
						       intermediate::BasicType compType,
						       const std::vector<RegisterPtr>& sources)
    {
      /* @@@ Consider operations like: float4 foo = float4 (RGB, 1)
	 Now, if 'foo' is used in a fragment frequency op, 'foo' is promoted -
	 even though this is suboptimal, as the 4th component is constant anyway.
	 Better: just promote RGB */
      int highestFreq = ComputeHighestFreq (sources);
      unsigned int commonFreqs = PromoteAll (highestFreq, sources);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpMakeVector> (destination,
										      compType,
										      sources));
      parent.SetRegAvailability (destination, AddOpToSequences (newSeqOp, commonFreqs));
    }
    
    void SequenceSplitter::InputVisitor::OpMakeMatrix (const RegisterPtr& destination,
						       intermediate::BasicType compType,
						       unsigned int matrixRows, unsigned int matrixCols,
						       const std::vector<RegisterPtr>& sources)
    {
      int highestFreq = ComputeHighestFreq (sources);
      unsigned int commonFreqs = PromoteAll (highestFreq, sources);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpMakeMatrix> (destination,
										      compType, 
										      matrixRows, matrixCols,
										      sources));
      parent.SetRegAvailability (destination, AddOpToSequences (newSeqOp, commonFreqs));
    }
    
    void SequenceSplitter::InputVisitor::OpMakeArray (const RegisterPtr& destination,
						      const std::vector<RegisterPtr>& sources)
    {
      int highestFreq = ComputeHighestFreq (sources);
      unsigned int commonFreqs = PromoteAll (highestFreq, sources);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpMakeArray> (destination, sources));
      parent.SetRegAvailability (destination, AddOpToSequences (newSeqOp, commonFreqs));
    }
		      
    void SequenceSplitter::InputVisitor::OpExtractArrayElement (const RegisterPtr& destination,
								const RegisterPtr& source,
								const RegisterPtr& index)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpExtractArrayElement> (destination, source, index));
      SplitBinaryOp (destination, newSeqOp, source, index);
    }
    
    void SequenceSplitter::InputVisitor::OpChangeArrayElement (const RegisterPtr& destination,
							       const RegisterPtr& source,
							       const RegisterPtr& index,
							       const RegisterPtr& newValue)
    {
      std::vector<RegisterPtr> sourceRegs;
      sourceRegs.push_back (source);
      sourceRegs.push_back (index);
      sourceRegs.push_back (newValue);
      int highestFreq = ComputeHighestFreq (sourceRegs);
      unsigned int commonFreqs = PromoteAll (highestFreq, sourceRegs);
      
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpChangeArrayElement> (destination, source,
											      index, newValue));
      parent.SetRegAvailability (destination, AddOpToSequences (newSeqOp, commonFreqs));
    }
    
    void SequenceSplitter::InputVisitor::OpGetArrayLength (const RegisterPtr& destination,
							   const RegisterPtr& array)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpGetArrayLength> (destination, array));
      
      unsigned int srcAvail = parent.GetRegAvailability (array);
      parent.SetRegAvailability (destination, AddOpToSequences (newSeqOp, srcAvail));
    }

    void SequenceSplitter::InputVisitor::OpExtractVectorComponent (const RegisterPtr& destination,
								   const RegisterPtr& source,
								   unsigned int comp)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpExtractVectorComponent> (destination, source, comp));
      
      unsigned int srcAvail = parent.GetRegAvailability (source);
      parent.SetRegAvailability (destination, AddOpToSequences (newSeqOp, srcAvail));
    }
    
    void SequenceSplitter::InputVisitor::OpArith (const RegisterPtr& destination,
						  ArithmeticOp op,
						  const RegisterPtr& source1,
						  const RegisterPtr& source2)
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
	PromoteRegister (source1, freqFragment);
	PromoteRegister (source2, freqFragment);
	
	// Add operation to fragment frequency
	parent.outputSeq[freqFragment]->AddOp (newSeqOp);
	parent.SetRegAvailability (destination, freqFlagF);
      }
    }
    
    void SequenceSplitter::InputVisitor::OpCompare (const RegisterPtr& destination,
						    CompareOp op,
						    const RegisterPtr& source1,
						    const RegisterPtr& source2)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpCompare> (destination, op, source1, source2));
      // Interpolation safety: comparison ops really aren't, except if both operands are uniform
      unsigned int src1Avail = parent.GetRegAvailability (source1);
      unsigned int src2Avail = parent.GetRegAvailability (source2);
      bool lerpSafe = (((src1Avail & freqFlagU) != 0) && ((src2Avail & freqFlagU) != 0));
      if (lerpSafe)
      {
	SplitBinaryOp (destination, newSeqOp, source1, source2);
	return;
      }
      
      PromoteRegister (source1, freqFragment);
      PromoteRegister (source2, freqFragment);
      
      // Add operation to fragment frequency
      parent.outputSeq[freqFragment]->AddOp (newSeqOp);
      parent.SetRegAvailability (destination, freqFlagF);
    }
			      
    void SequenceSplitter::InputVisitor::OpLogic (const RegisterPtr& destination,
						  LogicOp op,
						  const RegisterPtr& source1,
						  const RegisterPtr& source2)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpLogic> (destination, op, source1, source2));
      // Interpolation safety: logic ops really aren't, except if both operands are uniform
      unsigned int src1Avail = parent.GetRegAvailability (source1);
      unsigned int src2Avail = parent.GetRegAvailability (source2);
      bool lerpSafe = (((src1Avail & freqFlagU) != 0) && ((src2Avail & freqFlagU) != 0));
      if (lerpSafe)
      {
	SplitBinaryOp (destination, newSeqOp, source1, source2);
	return;
      }
      
      PromoteRegister (source1, freqFragment);
      PromoteRegister (source2, freqFragment);
      
      // Add operation to fragment frequency
      parent.outputSeq[freqFragment]->AddOp (newSeqOp);
      parent.SetRegAvailability (destination, freqFlagF);
    }
    
    void SequenceSplitter::InputVisitor::OpUnary (const RegisterPtr& destination,
						  UnaryOp op,
						  const RegisterPtr& source)
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
	PromoteRegister (source, freqFragment);
	
	// Add operation to fragment frequency
	parent.outputSeq[freqFragment]->AddOp (newSeqOp);
	parent.SetRegAvailability (destination, freqFlagF);
      }
    }
    
    void SequenceSplitter::InputVisitor::SplitBlock (const SequencePtr& blockSequence,
						     const Sequence::IdentifierToRegMap& identToRegIDs_imp,
						     const Sequence::IdentifierToRegMap& identToRegIDs_exp,
						     SequenceOpPtr* newSequences,
						     bool keepEmpty,
						     bool mergeUniformToVF)
    {
      AvailabilityMap regAvailability;
      SequenceSplitter blockSplitter (parent.progSplit, mergeUniformToVF);
      blockSplitter.SetInputSequence (blockSequence);
      
      // Forward frequencies to subSeqSplitter
      const Sequence::RegisterImpMappings& blockImports = blockSequence->GetImports();
      for (Sequence::RegisterImpMappings::const_iterator imports = blockImports.begin();
	   imports != blockImports.end();
	   ++imports)
      {
	Sequence::IdentifierToRegMap::const_iterator impRegID = identToRegIDs_imp.find (imports->first);
	assert (impRegID != identToRegIDs_imp.end());
	RegisterPtr reg (impRegID->second);
	RegisterPtr reg_subseq (imports->second);

	unsigned int reg_subseqAvail = parent.GetRegAvailability (reg);
	blockSplitter.SetLocalRegFreqFlags (reg_subseq, reg_subseqAvail);
      }
      
      blockSplitter.PerformSplit();
      
      Sequence::IdentifierToRegMap newIdentToRegIDs_imp (identToRegIDs_imp);
      Sequence::IdentifierToRegMap newIdentToRegIDs_exp (identToRegIDs_exp);
      // Propagate registers transferred by nested block to this block
      typedef boost::unordered_map<RegisterPtr, std::pair<RegisterPtr, UnicodeString> > TransferRegsMap;
      TransferRegsMap transferRegsMap;
      for (int f = 0; f < freqNum-1; f++)
      {
	const SequencePtr& srcSeq (blockSplitter.GetOutputSequence (f));
	const SequencePtr& dstSeq (blockSplitter.GetOutputSequence (f+1));
	const TransferRegsVector& transferRegs = blockSplitter.GetTransferRegs (f);
	BOOST_FOREACH(const RegisterPtr& reg, transferRegs)
	{
	  RegisterPtr newReg;
	  UnicodeString regName;
	  TransferRegsMap::const_iterator transferReg (transferRegsMap.find (reg));
	  if (transferReg != transferRegsMap.end())
	  {
	    newReg = transferReg->second.first;
	    regName = transferReg->second.second;
	  }
	  else
	  {
	    regName = parent.GetTransferIdent ();
	    newReg = parent.AllocateRegister (reg->GetOriginalType(), regName);
	    transferRegsMap[reg] = std::make_pair (newReg, regName);
	    
	    newIdentToRegIDs_imp[regName] = newReg;
	    newIdentToRegIDs_exp[regName] = newReg;
	  
	    parent.SetRegAvailability (newReg, blockSplitter.GetRegAvailability (reg));
	    srcSeq->SetExport (regName, reg);
	  }
	  
	  dstSeq->AddImport (regName, reg);
	  parent.transferRegs[f].push_back (newReg);
	  //parent.SetRegAvailability (newReg, (1 << f) | (1 << (f+1)));
	  //parent.SetRegAvailability (newReg, (1 << f));
	}
      }
      
      // Update availability flags
      const Sequence::RegisterExpMappings& blockExports = blockSequence->GetExports();
      for (Sequence::RegisterExpMappings::const_iterator exports = blockExports.begin();
	   exports != blockExports.end();
	   ++exports)
      {
	Sequence::IdentifierToRegMap::const_iterator expRegID = newIdentToRegIDs_exp.find (exports->first);
	assert (expRegID != newIdentToRegIDs_exp.end());
	RegisterPtr reg (expRegID->second);
	RegisterPtr reg_subseq (exports->second);
	
	unsigned int reg_subseqAvail = blockSplitter.GetRegAvailability (reg_subseq);
	parent.SetRegAvailability (reg, reg_subseqAvail);
      }
      
      for (int f = 0; f < freqNum; f++)
      {
	if (!keepEmpty && (blockSplitter.outputSeq[f]->GetNumOps() == 0)) continue;
	SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpBlock> (blockSplitter.outputSeq[f],
										   newIdentToRegIDs_imp,
										   newIdentToRegIDs_exp));
	newSequences[f] = newSeqOp;
      }
    }
    
    void SequenceSplitter::InputVisitor::OpBlock (const SequencePtr& subSequence,
						  const Sequence::IdentifierToRegMap& identToRegIDs_imp,
						  const Sequence::IdentifierToRegMap& identToRegIDs_exp)
    {
      SequenceOpPtr newSeqOps[freqNum];
      SplitBlock (subSequence, identToRegIDs_imp, identToRegIDs_exp, newSeqOps);
      
      for (int f = 0; f < freqNum; f++)
      {
	if (!newSeqOps[f]) continue;
	parent.outputSeq[f]->AddOp (newSeqOps[f]);
      }
    }
    
    void SequenceSplitter::InputVisitor::EmitUnconditionalBranchBlock (const char* suffix,
								       const SequenceOpPtr& blockOp,
								       int f,
								       RenamedBranchOutputs& outputs)
    {
      boost::shared_ptr<intermediate::SequenceOpBlock> newBlock (
	boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (blockOp));
      SequencePtr seq (newBlock->GetSequence());
      
      Sequence::RegisterExpMappings newExports;
      Sequence::RegisterExpMappings& seqExports (seq->GetExports());
      Sequence::IdentifierToRegMap newIdentToRegIDsExp (newBlock->GetExportIdentToRegs());
      std::vector<RegisterPtr> writtenRegs;
      for (Sequence::RegisterExpMappings::iterator seqExp (seqExports.begin());
	    seqExp != seqExports.end();
	    ++seqExp)
      {
	RegisterPtr oldReg (parent.inputSeq->GetIdentifierRegister (seqExp->first));
	if (!oldReg)
	{
	  /* Happens for transfer regs created by block splitting.
	     Keep them as-is. */
	  newExports[seqExp->first] = seqExp->second;
	  Sequence::IdentifierToRegMap::const_iterator expReg = newIdentToRegIDsExp.find (seqExp->first);
	  assert (expReg != newIdentToRegIDsExp.end());
	  writtenRegs.push_back (expReg->second);
	  continue;
	}
	UnicodeString newIdent (oldReg->GetName());
	newIdent.append ("$");
	newIdent.append (suffix);
	newIdent.append (UChar ('0' + f));
	
	RegisterPtr newReg (parent.AllocateRegister (oldReg->GetOriginalType(), newIdent));
	newExports[newIdent] = seqExp->second;
	newIdentToRegIDsExp[newIdent] = newReg;
	
	parent.SetRegAvailability (newReg, 1 << f);
	outputs.push_back (std::make_pair (oldReg, newReg));
	writtenRegs.push_back (newReg);
      }
      seqExports = newExports;
    
      std::vector<RegisterPtr> readRegs;
      readRegs.insert (readRegs.begin(), newBlock->GetReadRegisters().begin(), newBlock->GetReadRegisters().end());
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpBlock> (seq,
										 newBlock->GetImportIdentToRegs(),
										 newIdentToRegIDsExp));
      parent.outputSeq[f]->AddOp (newSeqOp);
    }
    
    intermediate::SequenceOpPtr SequenceSplitter::InputVisitor::AugmentBranchBlockWithRenames (const char* suffix,
											       const SequenceOpPtr& blockOp,
											       const RenamedBranchOutputs* renames,
											       int f)
    {
      boost::shared_ptr<intermediate::SequenceOpBlock> newBlock (
	boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (blockOp));
      SequencePtr seq (newBlock->GetSequence());
      
      Sequence::IdentifierToRegMap newIdentToRegIDsImp (newBlock->GetImportIdentToRegs());
      Sequence::IdentifierToRegMap newIdentToRegIDsExp (newBlock->GetExportIdentToRegs());
      
      std::vector<RegisterPtr> readRegs;
      readRegs.insert (readRegs.begin(), newBlock->GetReadRegisters().begin(), newBlock->GetReadRegisters().end());
      std::vector<RegisterPtr> writtenRegs;
      writtenRegs.insert (writtenRegs.begin(), newBlock->GetWrittenRegisters().begin(), newBlock->GetWrittenRegisters().end());
      for (int g = 0; g < f; g++)
      {
	BOOST_FOREACH(const RegisterPair& rename, renames[g])
	{
	  PromoteRegister (rename.second, f);
	  
	  intermediate::Sequence::RegisterPtr srcRegPtr (rename.second);
	  intermediate::Sequence::RegisterPtr dstRegPtr (rename.first);
	  
	  UnicodeString newIdent (dstRegPtr->GetName());
	  newIdent.append ("$");
	  newIdent.append (suffix);
	  newIdent.append (UChar ('0' + f));
	  
	  RegisterPtr newReg (seq->AllocateRegister (srcRegPtr->GetOriginalType(), newIdent));
	  seq->AddImport (srcRegPtr->GetName(), newReg);
	  seq->SetExport (dstRegPtr->GetName(), newReg);
	  
	  writtenRegs.push_back (rename.first);
	  readRegs.push_back (rename.second);
	  
	  newIdentToRegIDsImp[srcRegPtr->GetName()] = rename.second;
	  newIdentToRegIDsExp[dstRegPtr->GetName()] = rename.first;
	  
	  parent.SetRegAvailability (rename.first, 1 << f);
	}
      }
    
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpBlock> (seq,
										 newIdentToRegIDsImp,
										 newIdentToRegIDsExp));
      
      return newSeqOp;
    }
    
    void SequenceSplitter::InputVisitor::OpBranch (const RegisterPtr& conditionReg,
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
      std::vector<RegisterPtr> allInputs;
      allInputs.push_back (conditionReg);
      {
	const Sequence::RegisterImpMappings& seqImports (ifBlock->GetSequence()->GetImports());
	for (Sequence::RegisterImpMappings::const_iterator imports = seqImports.begin();
	     imports != seqImports.end();
	     ++imports)
	{
	  allInputs.push_back (parent.inputSeq->GetIdentifierRegister (imports->first));
	}      
      }
      {
	const Sequence::RegisterImpMappings& seqImports (elseBlock->GetSequence()->GetImports());
	for (Sequence::RegisterImpMappings::const_iterator imports = seqImports.begin();
	     imports != seqImports.end();
	     ++imports)
	{
	  allInputs.push_back (parent.inputSeq->GetIdentifierRegister (imports->first));
	}      
      }
      
      // Propagate condition and all sequence inputs to highest frequency
      unsigned int combinedFreqs = ComputeCombinedFreqs (allInputs);
      int highestFreq = ComputeHighestFreq (allInputs);
      unsigned int commonFreqs = PromoteAll (highestFreq, allInputs);
      
      // Output branch op to frequencies supported by condition and all sequence inputs
      SequenceOpPtr newIfOps[freqNum];
      {
	SplitBlock (ifBlock->GetSequence(),
		    ifBlock->GetImportIdentToRegs(),
		    ifBlock->GetExportIdentToRegs(),
		    newIfOps, true);
      }
      SequenceOpPtr newElseOps[freqNum];
      {
	SplitBlock (elseBlock->GetSequence(),
		    elseBlock->GetImportIdentToRegs(),
		    elseBlock->GetExportIdentToRegs(),
		    newElseOps, true);
      }
      // Frequency at which the condition needs to be evaluated
      int condFreq = HighestFreq (commonFreqs);
      
      RenamedBranchOutputs ifRenames[freqNum];
      RenamedBranchOutputs elseRenames[freqNum];
      
      for (int f = 0; f < freqNum; f++)
      {
	//if (!newSeqOps[f]) continue;
	//if ((commonFreqs & (1 << f)) == 0) continue;
	if ((combinedFreqs & (1 << f)) == 0) continue;
	// @@@ Should be possible to emit branches to VP even if condition is FP
	assert(newIfOps[f] != 0);
	assert(newElseOps[f] != 0);
	
	if (f == condFreq)
	{
	  SequenceOpPtr augmentedIfOp (AugmentBranchBlockWithRenames ("if", newIfOps[f], ifRenames, f));
	  SequenceOpPtr augmentedElseOp (AugmentBranchBlockWithRenames ("else", newElseOps[f], elseRenames, f));
	  
	  SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpBranch> (conditionReg,
										      augmentedIfOp,
										      augmentedElseOp));
	  parent.outputSeq[f]->AddOp (newSeqOp);
	}
	else
	{
	  EmitUnconditionalBranchBlock ("if", newIfOps[f], f, ifRenames[f]);
	  EmitUnconditionalBranchBlock ("else", newElseOps[f], f, elseRenames[f]);
	}
      }
    }
    
    void SequenceSplitter::InputVisitor::OpWhile (const RegisterPtr& conditionReg,
						  const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
						  const SequenceOpPtr& seqOpBody)
    {
      boost::shared_ptr<intermediate::SequenceOpBlock> body (
	boost::shared_dynamic_cast<intermediate::SequenceOpBlock> (seqOpBody));
      assert (body);

      /* Do a couple of "dry-run" splits to determine the required frequencies 
	 for looped vars */
      AvailabilityMap loopedRegFreqs;
      // Frequencies of looped regs start out with the frequency of the first-loop reg.
      BOOST_FOREACH(const LoopedReg& loopedReg, loopedRegs)
      {
	loopedRegFreqs[loopedReg.second] = parent.GetRegAvailability (loopedReg.first);
      }
      for (int n = 0; n < freqNum-1; n++)
      {
	SequencePtr blockSequence (body->GetSequence());
	const Sequence::IdentifierToRegMap& identToRegIDs_imp = body->GetImportIdentToRegs();
	const Sequence::IdentifierToRegMap& identToRegIDs_exp = body->GetExportIdentToRegs();
	SequenceSplitter blockSplitter (parent.progSplit, false);
	blockSplitter.SetInputSequence (blockSequence);
	
	// Forward frequencies to subSeqSplitter
	const Sequence::RegisterImpMappings& blockImports = blockSequence->GetImports();
	for (Sequence::RegisterImpMappings::const_iterator imports = blockImports.begin();
	    imports != blockImports.end();
	    ++imports)
	{
	  Sequence::IdentifierToRegMap::const_iterator impRegID = identToRegIDs_imp.find (imports->first);
	  assert (impRegID != identToRegIDs_imp.end());
	  RegisterPtr reg (impRegID->second);
	  RegisterPtr reg_subseq (imports->second);

	  unsigned int reg_subseqAvail;
	  AvailabilityMap::const_iterator loopedRegFreq = loopedRegFreqs.find (reg);
	  if (loopedRegFreq != loopedRegFreqs.end())
	    reg_subseqAvail = loopedRegFreq->second;
	  else
	    reg_subseqAvail = parent.GetRegAvailability (reg);
	  blockSplitter.SetLocalRegFreqFlags (reg_subseq, reg_subseqAvail);
	}
		
	blockSplitter.PerformSplit();
	
	// Update frequencies of looped regs
	const Sequence::RegisterExpMappings& blockExports = blockSequence->GetExports();
	for (Sequence::RegisterExpMappings::const_iterator exports = blockExports.begin();
	    exports != blockExports.end();
	    ++exports)
	{
	  Sequence::IdentifierToRegMap::const_iterator expRegID = identToRegIDs_exp.find (exports->first);
	  assert (expRegID != identToRegIDs_imp.end());
	  RegisterPtr reg (expRegID->second);
	  RegisterPtr reg_subseq (exports->second);
	  
	  AvailabilityMap::iterator loopedRegFreq = loopedRegFreqs.find (reg);
	  if (loopedRegFreq == loopedRegFreqs.end()) continue;
	  loopedRegFreq->second = blockSplitter.GetRegAvailability (reg_subseq);
	}
      }
      
      //unsigned int combinedFreqs = 0;
      // Take "dry-run" frequencies for looped regs and promote actual loop input registers
      BOOST_FOREACH(const LoopedReg& loopedReg, loopedRegs)
      {
	unsigned int postLoopAvail = loopedRegFreqs[loopedReg.second];
	//combinedFreqs |= postLoopAvail;
	int freq = HighestFreq (postLoopAvail);
	PromoteRegister (loopedReg.first, freq);
	// Force reg to be only available in highest freq
	if (loopedReg.second != conditionReg)
	  parent.SetRegAvailability (loopedReg.second, 1 << freq);
      }
      
      // Compute highest frequency of condition and all loop inputs
      std::vector<RegisterPtr> allInputs;
      {
	for (std::vector<std::pair<RegisterPtr, RegisterPtr> >::const_iterator loopedReg = loopedRegs.begin();
	     loopedReg != loopedRegs.end();
	     ++loopedReg)
	{
	  allInputs.push_back (loopedReg->first);
	}      
      }
      // Propagate condition and all sequence inputs to highest frequency
      /* @@@ Force highest frequency to cover the case where a looped reg gets a higher
	 frequency after a loop */
      unsigned int combinedFreqs = ComputeCombinedFreqs (allInputs);
      int highestFreq = freqHighest/*ComputeHighestFreq (allInputs)*/;
      //unsigned int commonFreqs = PromoteAll (highestFreq, allInputs);
      
      /*
      {
	for (std::vector<std::pair<RegisterPtr, RegisterPtr> >::const_iterator loopedReg = loopedRegs.begin();
	     loopedReg != loopedRegs.end();
	     ++loopedReg)
	{
	  // Set 'writeable' looped regs to same frequency as originals
	  parent.SetRegAvailability (loopedReg->second,
				     parent.GetRegAvailability (loopedReg->first));
	}      
      }
      */
      
      // Output branch op to frequencies supported by condition and all sequence inputs
      SequenceOpPtr newOps[freqNum];
      SplitBlock (body->GetSequence(),
		  body->GetImportIdentToRegs(),
		  body->GetExportIdentToRegs(),
		  newOps, true,
		  true);
      // TODO: Could filter looped regs
      for (int f = 0; f < freqNum; f++)
      {
	if (f == freqUniform) continue;
	//if ((commonFreqs & (1 << f)) == 0) continue;
	if ((combinedFreqs & (1 << f)) == 0) continue;
	assert(newOps[f] != 0);
	
	std::vector<LoopedReg> newLoopedRegs;
	BOOST_FOREACH(const LoopedReg& loopedReg, loopedRegs)
	{
	  unsigned int avail = parent.GetRegAvailability (loopedReg.second);
	  if ((avail & (1 << f)) != 0)
	    newLoopedRegs.push_back (loopedReg);
	}
	
	SequenceOpPtr newWhileOp (boost::make_shared<intermediate::SequenceOpWhile> (conditionReg,
										     newLoopedRegs,
										     newOps[f]));
	parent.outputSeq[f]->AddOp (newWhileOp);
      }
    }
    
    void SequenceSplitter::InputVisitor::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      SequenceOpPtr newSeqOp (boost::make_shared<intermediate::SequenceOpReturn> (outParamVals));
      
      /* 'return': only execute on fragment frequency */
      BOOST_FOREACH(const RegisterPtr& reg, outParamVals)
      {
	PromoteRegister (reg, freqFragment);
      }
      parent.outputSeq[freqFragment]->AddOp (newSeqOp);
    }
    
    void SequenceSplitter::InputVisitor::OpFunctionCall (const UnicodeString& funcIdent,
							 const std::vector<RegisterPtr>& inParams,
							 const std::vector<RegisterPtr>& outParams)
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
      
      std::vector<unsigned int> inputParamFreqFlags;
      BOOST_FOREACH(const RegisterPtr& reg, inParams)
      {
	inputParamFreqFlags.push_back (parent.GetRegAvailability (reg));
      }
      UnicodeString freqFuncIdents[freqNum];
      std::vector<unsigned int> outputParamFreqs;
      std::vector<ProgramSplitter::FunctionTransferValues> transferValues[freqNum-1];
      parent.progSplit.GetSplitFunctions (funcIdent, inputParamFreqFlags, freqFuncIdents,
					  outputParamFreqs,
					  transferValues);
	
      // Set availability of output values
      assert (outputParamFreqs.size() == outParams.size());
      for (size_t i = 0; i < outParams.size(); i++)
      {
	const RegisterPtr& reg = outParams[i];
	parent.SetRegAvailability (reg, outputParamFreqs[i]);
      }
      
      // Generate lists of registers used for 'transfer' between frequencies
      std::vector<RegisterPtr> transferIn[freqNum];
      std::vector<RegisterPtr> transferOut[freqNum];
      for (int f = 0; f < freqNum-1; f++)
      {
	BOOST_FOREACH(const ProgramSplitter::FunctionTransferValues& tfv, transferValues[f])
	{
	  UnicodeString transferIdent (parent.GetTransferIdent ());
	  
	  RegisterPtr reg (parent.AllocateRegister (tfv.valueType, transferIdent));
	  
	  transferOut[f].push_back (reg);
	  transferIn[f+1].push_back (reg);
	  parent.transferRegs[f].push_back (reg);
	}
      }
      
      
      for (int f = 0; f < freqNum; f++)
      {
	if (freqFuncIdents[f].isEmpty()) continue;
	
	// @@@ Somewhat crude: propagate all input vars to frequency of function split.
	// (Needed for recursive funcs)
	PromoteAll (freqFragment, inParams);
	
	// Add 'transfer' parameters to function call
	std::vector<RegisterPtr> newOutParams (outParams);
	newOutParams.insert (newOutParams.end(), transferOut[f].begin(), transferOut[f].end());
	std::vector<RegisterPtr> newInParams (inParams);
	newInParams.insert (newInParams.end(), transferIn[f].begin(), transferIn[f].end());
	
	SequenceOpPtr newOp (boost::make_shared<intermediate::SequenceOpFunctionCall> (freqFuncIdents[f],
										       newInParams,
										       newOutParams));
	parent.outputSeq[f]->AddOp (newOp);
      }
    }
			  
    void SequenceSplitter::InputVisitor::OpBuiltinCall (const RegisterPtr& destination,
							intermediate::BuiltinFunction what,
							const std::vector<RegisterPtr>& inParams)
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
    
    SequenceSplitter::SequenceSplitter (ProgramSplitter& progSplit,
					bool mergeUniformToVF)
     : progSplit (progSplit), mergeUniformToVF (mergeUniformToVF)					
    {
    }
    
    void SequenceSplitter::SetInputFreqFlags (const UnicodeString& input, unsigned int freqFlags)
    {
      paramFlags[input] = freqFlags;
    }
    
    void SequenceSplitter::PerformSplit ()
    {
      transferIdentNum = 0;
      regAvailability = setAvailability;
      transferRegs[0].clear();
      for (int f = 0; f < freqNum; f++)
      {
	outputSeq[f] = boost::make_shared<intermediate::Sequence> ();
	
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
	  Sequence::IdentifierToRegMap::const_iterator toReg =
	    inputSeq->GetIdentifierToRegisterMap().find (inputFreq->first);
	  if (toReg != inputSeq->GetIdentifierToRegisterMap().end())
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
      
      for (int f = 0; f < freqNum; f++)
      {
	outputSeq[f]->CleanUnusedImportsExports ();
      }
      
      // Insert 'uniform' sequence at start of both V/F sequences
      if (mergeUniformToVF)
      {
	// Remove transfer regs that originate from the uniform seq
	for (int f = 0; f < freqNum-1; f++)
	{
	  TransferRegsVector::iterator it = transferRegs[f].begin();
	  while (it != transferRegs[f].end())
	  {
	    RegisterPtr reg (*it);
	    unsigned int avail = regAvailability[reg];

	    if (LowestFreq (avail) == freqUniform)
	      transferRegs[f].erase (it);
	    else
	      ++it;
	  }
	}

	for (size_t i = 0; i < outputSeq[freqUniform]->GetNumOps(); i++)
	{
	  SequenceOpPtr uniOp (outputSeq[freqUniform]->GetOp (i));
	  outputSeq[freqVertex]->InsertOp (i, uniOp);
	  outputSeq[freqFragment]->InsertOp (i, uniOp);
	}

	outputSeq[freqVertex]->AddImports (outputSeq[freqUniform]->GetImports());
	outputSeq[freqVertex]->AddExports (outputSeq[freqUniform]->GetExports());
	outputSeq[freqFragment]->AddImports (outputSeq[freqUniform]->GetImports());
	outputSeq[freqFragment]->AddExports (outputSeq[freqUniform]->GetExports());
	
	outputSeq[freqUniform]->Clear ();
      }
    }
    
    int SequenceSplitter::GetDefaultFrequencyForType (const parser::SemanticsHandler::TypePtr& type)
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
    
    unsigned int SequenceSplitter::GetRegAvailability (const RegisterPtr& reg)
    {
      AvailabilityMap::const_iterator avail = regAvailability.find (reg);
      if (avail == regAvailability.end())
      {
	int defaultFreq = GetDefaultFrequencyForType (reg->GetOriginalType());
	
	unsigned int defaultAvail = 1 << defaultFreq;
	regAvailability[reg] = defaultAvail;
	
	const char* const defFreqName[freqNum] = { "uniform",  "vertex", "fragment" };
	const UnicodeString& regName = reg->GetName();
	std::string regNameStr;
	regName.toUTF8String (regNameStr);
	// FIXME: use a special 'warning' mechanism for this
	std::cerr << "Register " << regNameStr << " has no associated availability, using '"
	  << defFreqName[defaultFreq] << "'" << std::endl;
	return defaultAvail;
      }
      return avail->second;
    }

    UnicodeString SequenceSplitter::GetTransferIdent ()
    {
      UnicodeString transferIdent ("transfer$");
      UChar transferSuffix[charsToFormatUint + 1];
      u_snprintf (transferSuffix, sizeof (transferSuffix)/sizeof (UChar),
		  "%u", transferIdentNum++);
      transferIdent.append (transferSuffix);
      return transferIdent;
    }
    
    s1::intermediate::RegisterPtr
    SequenceSplitter::AllocateRegister (const s1::parser::SemanticsHandler::TypePtr& originalType,
					const UnicodeString& name)
    {
      // Generate registers for all output sequences are the same across all frequency program. (Makes life easier.)
      RegisterPtr reg (outputSeq[0]->AllocateRegister (originalType, name));
      outputSeq[0]->SetIdentifierRegister (name, reg);
      for (int f = 1; f < freqNum; f++)
      {
	outputSeq[f]->SetIdentifierRegister (name, reg);
      }
      return reg;
    }
  } // namespace splitter
} // namespace s1
