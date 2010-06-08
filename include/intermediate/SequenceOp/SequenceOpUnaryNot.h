#ifndef __INTERMEDIATE_SEQUENCEOPUNARYNOT_H__
#define __INTERMEDIATE_SEQUENCEOPUNARYNOT_H__

#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpUnaryNot : public SequenceOpUnary
    {
    public:
      SequenceOpUnaryNot (RegisterID destination,
			  RegisterID source);
			  
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPUNARYNOT_H__
