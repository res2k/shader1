#ifndef __INTERMEDIATE_SEQUENCEOPLOGIC_H__
#define __INTERMEDIATE_SEQUENCEOPLOGIC_H__

#include "../SequenceVisitor.h"
#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpLogic : public SequenceOpBinary
    {
      SequenceVisitor::LogicOp op;
    public:
      SequenceOpLogic (RegisterID destination,
		       SequenceVisitor::LogicOp op,
		       RegisterID source1,
		       RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPLOGIC_H__
