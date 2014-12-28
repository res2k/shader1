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

#include <boost/foreach.hpp>

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
				    const RegisterMap& regMap)
      {
	intermediate::RegisterSet newUsedRegisters;
	BOOST_FOREACH(const RegisterPtr& reg, usedRegisters)
	{
	  RegisterMap::const_iterator newSeqReg = regMap.find (reg);
	  if (newSeqReg != regMap.end())
	  {
	    newUsedRegisters.insert (newSeqReg->second);
	  }
	}
	return new DeadCodeChecker (newSequenceBuilder, newUsedRegisters, seqChanged);
      }
      
      bool VisitBackwards() const { return true; }
      
      void PostVisitSequence (CommonSequenceVisitor* visitor,
			      const intermediate::SequenceBuilderPtr& newSequenceBuilder,
			      const RegisterMap& regMap)
      {
	newSequenceBuilder->CleanUnusedImportsExports();
      }
      
      void AddOpToSequence (const SequenceOpPtr& seqOp)
      {
	// DeadCodeChecker is meant to use with reverse Sequence visiting,
	// so to 'add' an op, insert it at the beginning
	newSequenceBuilder->InsertOp (0, seqOp);
      }
      
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

      void OpLogic (const RegisterPtr& destination,
		    LogicOp op,
		    const RegisterPtr& source1,
		    const RegisterPtr& source2);

      void OpUnary (const RegisterPtr& destination,
		    UnaryOp op,
		    const RegisterPtr& source);
			      
      void OpCompare (const RegisterPtr& destination,
		      CompareOp op,
		      const RegisterPtr& source1,
		      const RegisterPtr& source2);
			
      void OpBlock (const intermediate::SequencePtr& seq,
		    const Sequence::IdentifierToRegMap& identToRegID_imp,
		    const Sequence::IdentifierToRegMap& identToRegID_exp);
		    
      void OpBranch (const RegisterPtr& conditionReg,
		      const intermediate::SequenceOpPtr& seqOpIf,
		      const intermediate::SequenceOpPtr& seqOpElse);
      void OpWhile (const RegisterPtr& conditionReg,
		    const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
		    const intermediate::SequenceOpPtr& seqOpBody);
		    
      void OpReturn (const std::vector<RegisterPtr>& outParamVals);
      void OpFunctionCall (const UnicodeString& funcIdent,
			    const std::vector<RegisterPtr>& inParams,
			    const std::vector<RegisterPtr>& outParams);
      void OpBuiltinCall (const RegisterPtr& destination,
			  intermediate::BuiltinFunction what,
			  const std::vector<RegisterPtr>& inParams);
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
      BOOST_FOREACH(const RegisterPtr& reg, sequenceExported)
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
      BOOST_FOREACH(const RegisterPtr& reg, writtenRegisters)
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
      BOOST_FOREACH(const RegPair& loopPair, loopedRegs)
      {
	readRegisters.insert (loopPair.first);
	writtenRegisters.insert (loopPair.second);
      }
      size_t writtenRegsUsed = 0;
      BOOST_FOREACH(const RegisterPtr& reg, writtenRegisters)
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
      CommonSequenceVisitor::OpWhile (conditionReg, loopedRegs, seqOpBody);
      // @@@ Might DCE on the nested sequence not have eliminated some of these regs?
      usedRegisters.insert (readRegisters.begin(), readRegisters.end());
    }

    void DeadCodeElimination::DeadCodeChecker::OpReturn (const std::vector<RegisterPtr>& outParamVals)
    {
      usedRegisters.insert (outParamVals.begin(), outParamVals.end());
      CommonSequenceVisitor::OpReturn (outParamVals);
    }
    
    void DeadCodeElimination::DeadCodeChecker::OpFunctionCall (const UnicodeString& funcIdent,
							       const std::vector<RegisterPtr>& inParams,
							       const std::vector<RegisterPtr>& outParams)
    {
      size_t outParamsUsed = 0;
      BOOST_FOREACH(const RegisterPtr& reg, outParams)
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
