#ifndef __INTERMEDIATE_SEQUENCEOPUNARYINV_H__
#define __INTERMEDIATE_SEQUENCEOPUNARYINV_H__

#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpUnaryInv : public SequenceOpUnary
    {
    public:
      SequenceOpUnaryInv (RegisterID destination,
			  RegisterID source);
			  
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPUNARYINV_H__
