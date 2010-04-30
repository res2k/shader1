#ifndef __INTERMEDIATE_SEQUENCEOPARITHDIV_H__
#define __INTERMEDIATE_SEQUENCEOPARITHDIV_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpArithDiv : public SequenceOpBinary
    {
    public:
      SequenceOpArithDiv (RegisterID destination,
			  RegisterID source1,
			  RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPARITHDIV_H__
