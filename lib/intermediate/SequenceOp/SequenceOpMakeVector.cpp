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
      switch (vectorBaseType)
      {
      case Bool:
	visitor.OpMakeVector (destination, SequenceVisitor::Bool, sources);
	break;
      case Int:
	visitor.OpMakeVector (destination, SequenceVisitor::Int, sources);
	break;
      case UInt:
	visitor.OpMakeVector (destination, SequenceVisitor::UInt, sources);
	break;
      case Float:
	visitor.OpMakeVector (destination, SequenceVisitor::Float, sources);
	break;
      }
    }
  } // namespace intermediate
} // namespace s1
