#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpMakeArray.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpMakeArray::SequenceOpMakeArray (RegisterPtr destination,
					      const std::vector<RegisterPtr>& sources)
      : SequenceOpWithResult (destination), sources (sources)
    {}
			  
    RegisterSet SequenceOpMakeArray::GetReadRegisters () const
    {
      RegisterSet set;
      set.insert (sources.begin(), sources.end());
      return set;
    }

    void SequenceOpMakeArray::Visit (SequenceVisitor& visitor)
    {
      visitor.OpMakeArray (destination, sources);
    }
  } // namespace intermediate
} // namespace s1
