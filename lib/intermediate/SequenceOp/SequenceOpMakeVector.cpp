#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpMakeVector.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpMakeVector::SequenceOpMakeVector (RegisterPtr destination,
						BasicType vectorBaseType,
						const std::vector<RegisterPtr>& sources)
      : SequenceOpWithResult (destination), vectorBaseType (vectorBaseType), sources (sources)
    {}
			  
    RegisterSet SequenceOpMakeVector::GetReadRegisters () const
    {
      RegisterSet set;
      set.insert (sources.begin(), sources.end());
      return set;
    }

    void SequenceOpMakeVector::Visit (SequenceVisitor& visitor)
    {
      visitor.OpMakeVector (destination, vectorBaseType, sources);
    }
  } // namespace intermediate
} // namespace s1
