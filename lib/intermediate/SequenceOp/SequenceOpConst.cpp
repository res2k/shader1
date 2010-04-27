#include "intermediate/SequenceOp/SequenceOpConst.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    void SequenceOpConst::Visit (SequenceVisitor& visitor)
    {
      switch (type)
      {
      case Bool:
	visitor.OpConstBool (destination, boolVal);
	break;
      case Int:
	visitor.OpConstInt (destination, intVal);
	break;
      case UInt:
	visitor.OpConstUInt (destination, uintVal);
	break;
      case Float:
	visitor.OpConstFloat (destination, floatVal);
	break;
      }
    }
    
  } // namespace intermediate
} // namespace s1
