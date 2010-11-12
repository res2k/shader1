#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpChangeArrayElement.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpChangeArrayElement::SequenceOpChangeArrayElement (RegisterPtr destination,
								RegisterPtr source,
								RegisterPtr index,
								RegisterPtr newValue)
     : SequenceOpUnary (destination, source), index (index), newValue (newValue)
    {}
			    
    RegisterSet SequenceOpChangeArrayElement::GetReadRegisters () const
    {
      RegisterSet set (SequenceOpUnary::GetReadRegisters ());
      set.insert (index);
      set.insert (newValue);
      return set;
    }

    void SequenceOpChangeArrayElement::Visit (SequenceVisitor& visitor)
    {
      visitor.OpChangeArrayElement (destination, source, index, newValue);
    }
    
  } // namespace intermediate
} // namespace s1
