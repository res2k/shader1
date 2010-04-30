#ifndef __INTERMEDIATE_SEQUENCEOPARITHADD_H__
#define __INTERMEDIATE_SEQUENCEOPARITHADD_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpArithAdd : public SequenceOpBinary
    {
    public:
      SequenceOpArithAdd (RegisterID destination,
			  RegisterID source1,
			  RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPARITHADD_H__
