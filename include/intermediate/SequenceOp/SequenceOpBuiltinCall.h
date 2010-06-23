#ifndef __INTERMEDIATE_SEQUENCEOPBUILTINCALL_H__
#define __INTERMEDIATE_SEQUENCEOPBUILTINCALL_H__

#include "SequenceOpWithResult.h"
#include "../forwarddecl.h"
#include "../BuiltinFunction.h"
#include "../RegisterID.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpBuiltinCall : public SequenceOpWithResult
    {
      BuiltinFunction what;
      std::vector<RegisterID> inParams;
    public:
      SequenceOpBuiltinCall (const RegisterID& destination,
			     BuiltinFunction what,
			     const std::vector<RegisterID>& inParams);
      
      RegisterIDSet GetReadRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPBUILTINCALL_H__
