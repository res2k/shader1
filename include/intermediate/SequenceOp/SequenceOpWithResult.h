#ifndef __INTERMEDIATE_SEQUENCEOPWITHRESULT_H__
#define __INTERMEDIATE_SEQUENCEOPWITHRESULT_H__

#include "../RegisterID.h"
#include "SequenceOp.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpWithResult : public SequenceOp
    {
    protected:
      RegisterID destination;
    public:
      SequenceOpWithResult (RegisterID destination);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPWITHRESULT_H__
