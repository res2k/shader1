#ifndef __INTERMEDIATE_SEQUENCEOPRETURN_H__
#define __INTERMEDIATE_SEQUENCEOPRETURN_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"
#include "../RegisterID.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpReturn : public SequenceOp
    {
      RegisterID retValReg;
    public:
      SequenceOpReturn (RegisterID retValReg);
      
      RegisterIDSet GetReadRegisters () const;
      RegisterIDSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPRETURN_H__
