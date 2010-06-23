#ifndef __INTERMEDIATE_SEQUENCEOPGETARRAYLENGTH_H__
#define __INTERMEDIATE_SEQUENCEOPGETARRAYLENGTH_H__

#include "../BasicType.h"
#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpGetArrayLength : public SequenceOpUnary
    {
    public:
      SequenceOpGetArrayLength (RegisterID destination,
				RegisterID source);
			    
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPGETARRAYLENGTH_H__
