#include "intermediate/SequenceOp/SequenceOpCast.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCast::SequenceOpCast (Sequence::RegisterID destination,
				    BasicType castTo,
				    Sequence::RegisterID source)
     : SequenceOpUnary (destination, source), castTo (castTo)
    {
    }
    
    void SequenceOpCast::Visit (SequenceVisitor& visitor)
    {
      switch (castTo)
      {
      case Bool:
	visitor.OpCastToBool (destination, source);
	break;
      case Int:
	visitor.OpCastToInt (destination, source);
	break;
      case UInt:
	visitor.OpCastToUInt (destination, source);
	break;
      case Float:
	visitor.OpCastToFloat (destination, source);
	break;
      }
    }
  } // namespace intermediate
} // namespace s1
