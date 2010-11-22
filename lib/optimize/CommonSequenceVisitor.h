#ifndef __COMMONSEQUENCEVISITOR_H__
#define __COMMONSEQUENCEVISITOR_H__

#include "intermediate/Sequence.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace optimize
  {
    class CommonSequenceVisitor : public intermediate::SequenceVisitor
    {
    public:
      typedef intermediate::RegisterPtr RegisterPtr;
      typedef intermediate::Sequence Sequence;
      typedef intermediate::SequenceOpPtr SequenceOpPtr;
      
      intermediate::SequencePtr newSequence;
      
      CommonSequenceVisitor (const intermediate::SequencePtr& newSequence);
      
      virtual RegisterPtr MapRegister (const RegisterPtr& reg)
      { return reg; }
      virtual void AddOpToSequence (const SequenceOpPtr& seqOp)
      {
	newSequence->AddOp (seqOp);
      }
		      
      void SetVisitedOp (const intermediate::SequenceOpPtr& op)
      { visitedOp = op; }
      
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
    protected:
      intermediate::SequenceOpPtr visitedOp;
      
      virtual CommonSequenceVisitor* Clone (const intermediate::SequencePtr& newSequence) = 0;
    };
  } // namespace optimize
} // namespace s1

#endif // __COMMONSEQUENCEVISITOR_H__
