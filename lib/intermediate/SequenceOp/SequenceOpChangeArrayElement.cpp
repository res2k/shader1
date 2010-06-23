#include "intermediate/SequenceOp/SequenceOpChangeArrayElement.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpChangeArrayElement::SequenceOpChangeArrayElement (RegisterID destination,
								RegisterID source,
								RegisterID index,
								RegisterID newValue)
     : SequenceOpUnary (destination, source), index (index), newValue (newValue)
    {}
			    
    RegisterIDSet SequenceOpChangeArrayElement::GetReadRegisters () const
    {
      RegisterIDSet set (SequenceOpUnary::GetReadRegisters ());
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
