#ifndef __INTERMEDIATE_SEQUENCEOPWITHRESULT_H__
#define __INTERMEDIATE_SEQUENCEOPWITHRESULT_H__

#include "Sequence.h"
#include "SequenceOp.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpWithResult : public SequenceOp
    {
      Sequence::RegisterID destination;
    public:
      SequenceOpWithResult (Sequence::RegisterID destination);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPWITHRESULT_H__
