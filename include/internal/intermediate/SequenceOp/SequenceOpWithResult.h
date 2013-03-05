#ifndef __INTERMEDIATE_SEQUENCEOPWITHRESULT_H__
#define __INTERMEDIATE_SEQUENCEOPWITHRESULT_H__

#include "SequenceOp.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpWithResult : public SequenceOp
    {
    protected:
      RegisterPtr destination;
    public:
      SequenceOpWithResult (RegisterPtr destination);
      
      RegisterSet GetWrittenRegisters () const;
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPWITHRESULT_H__
