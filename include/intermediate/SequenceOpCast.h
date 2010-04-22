#ifndef __INTERMEDIATE_SEQUENCEOPCAST_H__
#define __INTERMEDIATE_SEQUENCEOPCAST_H__

#include "BasicType.h"
#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpCast : public SequenceOpUnary
    {
      BasicType castTo;
    public:
      SequenceOpCast (Sequence::RegisterID destination,
		      BasicType castTo,
		      Sequence::RegisterID source);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPCAST_H__
