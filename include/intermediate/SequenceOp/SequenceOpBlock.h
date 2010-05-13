#ifndef __INTERMEDIATE_SEQUENCEOPBLOCK_H__
#define __INTERMEDIATE_SEQUENCEOPBLOCK_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpBlock : public SequenceOp
    {
      SequencePtr subSequence;
    public:
      SequenceOpBlock (const SequencePtr& subSequence);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPBLOCK_H__
