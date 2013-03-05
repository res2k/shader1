#ifndef __INTERMEDIATE_SEQUENCEOPRETURN_H__
#define __INTERMEDIATE_SEQUENCEOPRETURN_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpReturn : public SequenceOp
    {
      std::vector<RegisterPtr> outParamVals;
    public:
      SequenceOpReturn (const std::vector<RegisterPtr>& outParamVals);
      
      RegisterSet GetReadRegisters () const;
      RegisterSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPRETURN_H__