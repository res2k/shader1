#ifndef __INTERMEDIATE_SEQUENCEOPUNARYNEG_H__
#define __INTERMEDIATE_SEQUENCEOPUNARYNEG_H__

#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpUnaryNeg : public SequenceOpUnary
    {
    public:
      SequenceOpUnaryNeg (RegisterID destination,
			  RegisterID source);
			  
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPUNARYNEG_H__
