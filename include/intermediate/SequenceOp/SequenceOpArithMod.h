#ifndef __INTERMEDIATE_SEQUENCEOPARITHMOD_H__
#define __INTERMEDIATE_SEQUENCEOPARITHMOD_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpArithMod : public SequenceOpBinary
    {
    public:
      SequenceOpArithMod (Sequence::RegisterID destination,
			  Sequence::RegisterID source1,
			  Sequence::RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPARITHMOD_H__
