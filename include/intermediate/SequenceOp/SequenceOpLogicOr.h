#ifndef __INTERMEDIATE_SEQUENCEOPLOGICOR_H__
#define __INTERMEDIATE_SEQUENCEOPLOGICOR_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpLogicOr : public SequenceOpBinary
    {
    public:
      SequenceOpLogicOr (RegisterID destination,
			 RegisterID source1,
			 RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPLOGICOR_H__
