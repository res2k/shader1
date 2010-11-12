#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpExtractArrayElement.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpExtractArrayElement::SequenceOpExtractArrayElement (RegisterPtr destination,
								  RegisterPtr source,
								  RegisterPtr index)
     : SequenceOpUnary (destination, source), index (index)
    {}
			    
    RegisterSet SequenceOpExtractArrayElement::GetReadRegisters () const
    {
      RegisterSet set (SequenceOpUnary::GetReadRegisters ());
      set.insert (index);
      return set;
    }

    void SequenceOpExtractArrayElement::Visit (SequenceVisitor& visitor)
    {
      visitor.OpExtractArrayElement (destination, source, index);
    }
    
  } // namespace intermediate
} // namespace s1
