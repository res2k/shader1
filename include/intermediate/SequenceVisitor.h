#ifndef __INTERMEDIATE_SEQUENCEVISITOR_H__
#define __INTERMEDIATE_SEQUENCEVISITOR_H__

#include "forwarddecl.h"
#include "RegisterID.h"
#include "Sequence.h"

namespace s1
{
  namespace intermediate
  {
    struct SequenceVisitor
    {
      virtual ~SequenceVisitor() {}
      
      virtual void OpConstBool (const RegisterID& destination,
				bool value) = 0;
      virtual void OpConstInt (const RegisterID& destination,
			       int value) = 0;
      virtual void OpConstUInt (const RegisterID& destination,
				unsigned int value) = 0;
      virtual void OpConstFloat (const RegisterID& destination,
				 float value) = 0;
				 
      virtual void OpAssign (const RegisterID& destination,
			     const RegisterID& source) = 0;
				 
      virtual void OpCastToBool (const RegisterID& destination,
			         const RegisterID& source) = 0;
      virtual void OpCastToInt (const RegisterID& destination,
			        const RegisterID& source) = 0;
      virtual void OpCastToUInt (const RegisterID& destination,
			         const RegisterID& source) = 0;
      virtual void OpCastToFloat (const RegisterID& destination,
			          const RegisterID& source) = 0;

      virtual void OpArithAdd (const RegisterID& destination,
			       const RegisterID& source1,
			       const RegisterID& source2) = 0;
      virtual void OpArithSub (const RegisterID& destination,
			       const RegisterID& source1,
			       const RegisterID& source2) = 0;
      virtual void OpArithMul (const RegisterID& destination,
			       const RegisterID& source1,
			       const RegisterID& source2) = 0;
      virtual void OpArithDiv (const RegisterID& destination,
			       const RegisterID& source1,
			       const RegisterID& source2) = 0;
      virtual void OpArithMod (const RegisterID& destination,
			       const RegisterID& source1,
			       const RegisterID& source2) = 0;

      virtual void OpCompareEq (const RegisterID& destination,
				const RegisterID& source1,
				const RegisterID& source2) = 0;
      virtual void OpCompareNE (const RegisterID& destination,
				const RegisterID& source1,
				const RegisterID& source2) = 0;
      virtual void OpCompareLT (const RegisterID& destination,
				const RegisterID& source1,
				const RegisterID& source2) = 0;
      virtual void OpCompareLE (const RegisterID& destination,
				const RegisterID& source1,
				const RegisterID& source2) = 0;
      virtual void OpCompareGT (const RegisterID& destination,
				const RegisterID& source1,
				const RegisterID& source2) = 0;
      virtual void OpCompareGE (const RegisterID& destination,
				const RegisterID& source1,
				const RegisterID& source2) = 0;
				
      virtual void OpLogicAnd (const RegisterID& destination,
			       const RegisterID& source1,
			       const RegisterID& source2) = 0;
      virtual void OpLogicOr (const RegisterID& destination,
			      const RegisterID& source1,
			      const RegisterID& source2) = 0;

      virtual void OpBlock (const SequencePtr& subSequence,
			    const Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
			    const Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
			    const std::vector<RegisterID>& writtenRegisters) = 0;
			    
      virtual void OpBranch (const RegisterID& conditionReg,
			     const SequenceOpPtr& seqOpIf,
			     const SequenceOpPtr& seqOpElse) = 0;
      virtual void OpWhile (const RegisterID& conditionReg,
			    const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
			    const SequenceOpPtr& seqOpBody) = 0;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEVISITOR_H__
