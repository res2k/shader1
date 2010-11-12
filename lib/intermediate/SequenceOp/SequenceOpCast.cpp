#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpCast.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCast::SequenceOpCast (RegisterPtr destination,
				    BasicType castTo,
				    RegisterPtr source)
     : SequenceOpUnary (destination, source), castTo (castTo)
    {
    }
    
    void SequenceOpCast::Visit (SequenceVisitor& visitor)
    {
      switch (castTo)
      {
      case Bool:
	visitor.OpCast (destination, SequenceVisitor::Bool, source);
	break;
      case Int:
	visitor.OpCast (destination, SequenceVisitor::Int, source);
	break;
      case UInt:
	visitor.OpCast (destination, SequenceVisitor::UInt, source);
	break;
      case Float:
	visitor.OpCast (destination, SequenceVisitor::Float, source);
	break;
      }
    }
  } // namespace intermediate
} // namespace s1
