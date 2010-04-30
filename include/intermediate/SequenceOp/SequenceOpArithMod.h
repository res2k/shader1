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
      SequenceOpArithMod (RegisterID destination,
			  RegisterID source1,
			  RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPARITHMOD_H__
