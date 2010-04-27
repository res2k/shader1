#ifndef __INTERMEDIATE_SEQUENCEVISITOR_H__
#define __INTERMEDIATE_SEQUENCEVISITOR_H__

#include "Sequence.h"

namespace s1
{
  namespace intermediate
  {
    struct SequenceVisitor
    {
      virtual ~SequenceVisitor() {}
      
      virtual void OpConstBool (const Sequence::RegisterID& destination,
				bool value) = 0;
      virtual void OpConstInt (const Sequence::RegisterID& destination,
			       int value) = 0;
      virtual void OpConstUInt (const Sequence::RegisterID& destination,
				unsigned int value) = 0;
      virtual void OpConstFloat (const Sequence::RegisterID& destination,
				 float value) = 0;
				 
      virtual void OpAssign (const Sequence::RegisterID& destination,
			     const Sequence::RegisterID& source) = 0;
				 
      virtual void OpCastToBool (const Sequence::RegisterID& destination,
			         const Sequence::RegisterID& source) = 0;
      virtual void OpCastToInt (const Sequence::RegisterID& destination,
			        const Sequence::RegisterID& source) = 0;
      virtual void OpCastToUInt (const Sequence::RegisterID& destination,
			         const Sequence::RegisterID& source) = 0;
      virtual void OpCastToFloat (const Sequence::RegisterID& destination,
			          const Sequence::RegisterID& source) = 0;

      virtual void OpArithAdd (const Sequence::RegisterID& destination,
			       const Sequence::RegisterID& source1,
			       const Sequence::RegisterID& source2) = 0;
      virtual void OpArithSub (const Sequence::RegisterID& destination,
			       const Sequence::RegisterID& source1,
			       const Sequence::RegisterID& source2) = 0;
      virtual void OpArithMul (const Sequence::RegisterID& destination,
			       const Sequence::RegisterID& source1,
			       const Sequence::RegisterID& source2) = 0;
      virtual void OpArithDiv (const Sequence::RegisterID& destination,
			       const Sequence::RegisterID& source1,
			       const Sequence::RegisterID& source2) = 0;
      virtual void OpArithMod (const Sequence::RegisterID& destination,
			       const Sequence::RegisterID& source1,
			       const Sequence::RegisterID& source2) = 0;
};
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEVISITOR_H__
