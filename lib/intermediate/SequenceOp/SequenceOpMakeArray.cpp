#include "intermediate/SequenceOp/SequenceOpMakeArray.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpMakeArray::SequenceOpMakeArray (RegisterID destination,
					      const std::vector<RegisterID>& sources)
      : SequenceOpWithResult (destination), sources (sources)
    {}
			  
    RegisterIDSet SequenceOpMakeArray::GetReadRegisters () const
    {
      RegisterIDSet set;
      set.insert (sources.begin(), sources.end());
      return set;
    }

    void SequenceOpMakeArray::Visit (SequenceVisitor& visitor)
    {
      visitor.OpMakeArray (destination, sources);
    }
  } // namespace intermediate
} // namespace s1
