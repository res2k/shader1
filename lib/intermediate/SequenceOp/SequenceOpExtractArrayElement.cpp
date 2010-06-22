#include "intermediate/SequenceOp/SequenceOpExtractArrayElement.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpExtractArrayElement::SequenceOpExtractArrayElement (RegisterID destination,
								  RegisterID source,
								  RegisterID index)
     : SequenceOpUnary (destination, source), index (index)
    {}
			    
    RegisterIDSet SequenceOpExtractArrayElement::GetReadRegisters () const
    {
      RegisterIDSet set (SequenceOpUnary::GetReadRegisters ());
      set.insert (index);
      return set;
    }

    void SequenceOpExtractArrayElement::Visit (SequenceVisitor& visitor)
    {
      visitor.OpExtractArrayElement (destination, source, index);
    }
    
  } // namespace intermediate
} // namespace s1
