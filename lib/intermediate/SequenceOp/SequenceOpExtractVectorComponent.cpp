#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpExtractVectorComponent.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpExtractVectorComponent::SequenceOpExtractVectorComponent (RegisterPtr destination,
									RegisterPtr source,
									unsigned int vecComp)
     : SequenceOpUnary (destination, source), comp (vecComp)
    {}
			    
    void SequenceOpExtractVectorComponent::Visit (SequenceVisitor& visitor)
    {
      visitor.OpExtractVectorComponent (destination, source, comp);
    }
    
  } // namespace intermediate
} // namespace s1
