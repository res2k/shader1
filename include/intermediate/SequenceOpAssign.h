#ifndef __INTERMEDIATE_SEQUENCEOPASSIGN_H__
#define __INTERMEDIATE_SEQUENCEOPASSIGN_H__

#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpAssign : public SequenceOpUnary
    {
    public:
      SequenceOpAssign (Sequence::RegisterID destination,
			Sequence::RegisterID source);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPASSIGN_H__
