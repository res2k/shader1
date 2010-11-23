#include "base/common.h"
#include "optimize/DeadCodeElimination.h"

#include "CommonSequenceVisitor.h"

#include <boost/foreach.hpp>

namespace s1
{
  namespace optimize
  {
    class DeadCodeElimination::DeadCodeChecker : public CommonSequenceVisitor
    {
      bool seqChanged;
      intermediate::RegisterSet usedRegisters;
    public:
      DeadCodeChecker (const intermediate::SequencePtr& outputSeq,
		       const intermediate::RegisterSet& usedRegistersSeed)
       : CommonSequenceVisitor (outputSeq), seqChanged (false), usedRegisters (usedRegistersSeed)
      {
      }
      
      bool HasChangedSeq() const { return seqChanged; }
      
      CommonSequenceVisitor* Clone (const intermediate::SequencePtr& newSequence)
      {
	return new DeadCodeChecker (newSequence, usedRegisters);
      }
      
      void AddOpToSequence (const SequenceOpPtr& seqOp)
      {
	// DeadCodeChecker is meant to use with reverse Sequence visiting,
	// so to 'add' an op, insert it at the beginning
	newSequence->InsertOp (0, seqOp);
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
      usedRegisters.insert (conditionReg);
      CommonSequenceVisitor::OpBranch (conditionReg, seqOpIf, seqOpElse);
    }
    
    void DeadCodeElimination::DeadCodeChecker::OpWhile (const RegisterPtr& conditionReg,
							const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
							const intermediate::SequenceOpPtr& seqOpBody)
    {
      usedRegisters.insert (conditionReg);
      
      typedef std::pair<RegisterPtr, RegisterPtr> RegPair;
      BOOST_FOREACH(const RegPair& loopPair, loopedRegs)
      {
	usedRegisters.insert (loopPair.first);
	usedRegisters.insert (loopPair.second);
      }
      CommonSequenceVisitor::OpWhile (conditionReg, loopedRegs, seqOpBody);
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
    
    bool DeadCodeElimination::EliminateDeadCode (const intermediate::SequencePtr& outputSeq,
						 const intermediate::SequencePtr& inputSeq,
						 const intermediate::RegisterSet& usedRegistersSeed)
    {
      DeadCodeChecker checker (outputSeq, usedRegistersSeed);
      inputSeq->ReverseVisit (checker);
      return checker.HasChangedSeq();      
    }
  } // namespace optimize
} // namespace s1