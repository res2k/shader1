#ifndef __INTERMEDIATE_SEQUENCEOPBUILTINCALL_H__
#define __INTERMEDIATE_SEQUENCEOPBUILTINCALL_H__

#include "SequenceOpWithResult.h"
#include "../forwarddecl.h"
#include "../BuiltinFunction.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpBuiltinCall : public SequenceOpWithResult
    {
      BuiltinFunction what;
      std::vector<RegisterPtr> inParams;
    public:
      SequenceOpBuiltinCall (const RegisterPtr& destination,
			     BuiltinFunction what,
			     const std::vector<RegisterPtr>& inParams);
      
      RegisterSet GetReadRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPBUILTINCALL_H__
