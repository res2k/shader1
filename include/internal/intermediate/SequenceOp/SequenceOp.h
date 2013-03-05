#ifndef __INTERMEDIATE_SEQUENCEOP_H__
#define __INTERMEDIATE_SEQUENCEOP_H__

#include "../Sequence.h"

#include <boost/shared_ptr.hpp>

namespace s1
{
  namespace intermediate
  {
    struct SequenceVisitor;
    
    class SequenceOp
    {
    public:
      virtual ~SequenceOp() {}
      
      /// Get the registers read from by this operation
      virtual RegisterSet GetReadRegisters () const = 0;
      /// Get the registers written to by this operation
      virtual RegisterSet GetWrittenRegisters () const = 0;
      
      virtual void Visit (SequenceVisitor& visitor) = 0;
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOP_H__