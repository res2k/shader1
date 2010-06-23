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
				  
      virtual void OpMakeVectorBool (const RegisterID& destination,
				     const std::vector<RegisterID>& sources) = 0;
      virtual void OpMakeVectorInt (const RegisterID& destination,
				     const std::vector<RegisterID>& sources) = 0;
      virtual void OpMakeVectorUInt (const RegisterID& destination,
				     const std::vector<RegisterID>& sources) = 0;
      virtual void OpMakeVectorFloat (const RegisterID& destination,
				     const std::vector<RegisterID>& sources) = 0;

      virtual void OpMakeMatrixBool (const RegisterID& destination,
				     unsigned int matrixRows, unsigned int matrixCols,
				     const std::vector<RegisterID>& sources) = 0;
      virtual void OpMakeMatrixInt (const RegisterID& destination,
				    unsigned int matrixRows, unsigned int matrixCols,
				    const std::vector<RegisterID>& sources) = 0;
      virtual void OpMakeMatrixUInt (const RegisterID& destination,
				     unsigned int matrixRows, unsigned int matrixCols,
				     const std::vector<RegisterID>& sources) = 0;
      virtual void OpMakeMatrixFloat (const RegisterID& destination,
				      unsigned int matrixRows, unsigned int matrixCols,
				      const std::vector<RegisterID>& sources) = 0;

      virtual void OpMakeArray (const RegisterID& destination,
				const std::vector<RegisterID>& sources) = 0;
      virtual void OpExtractArrayElement (const RegisterID& destination,
					  const RegisterID& source,
					  const RegisterID& index) = 0;
      virtual void OpChangeArrayElement (const RegisterID& destination,
					 const RegisterID& source,
					 const RegisterID& index,
					 const RegisterID& newValue) = 0;

      virtual void OpExtractVectorComponent (const RegisterID& destination,
					     const RegisterID& source,
					     unsigned int comp) = 0;
				 
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

      virtual void OpUnaryInv (const RegisterID& destination,
			       const RegisterID& source) = 0;
      virtual void OpUnaryNeg (const RegisterID& destination,
			       const RegisterID& source) = 0;
      virtual void OpUnaryNot (const RegisterID& destination,
			       const RegisterID& source) = 0;
			       
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
			    
      virtual void OpReturn (const RegisterID& retValReg) = 0;
      virtual void OpFunctionCall (const RegisterID& destination,
				   const UnicodeString& funcIdent,
				   const std::vector<RegisterID>& inParams,
				   const std::vector<RegisterID>& outParams) = 0;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEVISITOR_H__
