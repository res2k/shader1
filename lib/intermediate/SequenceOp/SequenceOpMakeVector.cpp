#include "intermediate/SequenceOp/SequenceOpMakeVector.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpMakeVector::SequenceOpMakeVector (RegisterID destination,
						BasicType vectorBaseType,
						const std::vector<RegisterID>& sources)
      : SequenceOpWithResult (destination), vectorBaseType (vectorBaseType), sources (sources)
    {}
			  
    RegisterIDSet SequenceOpMakeVector::GetReadRegisters () const
    {
      RegisterIDSet set;
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
