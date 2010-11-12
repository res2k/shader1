#ifndef __INTERMEDIATE_SEQUENCEOPMAKEVECTOR_H__
#define __INTERMEDIATE_SEQUENCEOPMAKEVECTOR_H__

#include "../BasicType.h"
#include "SequenceOpWithResult.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpMakeVector : public SequenceOpWithResult
    {
      BasicType vectorBaseType;
      std::vector<RegisterPtr> sources;
    public:
      SequenceOpMakeVector (RegisterPtr destination,
			    BasicType vectorBaseType,
			    const std::vector<RegisterPtr>& sources);
			    
      RegisterSet GetReadRegisters () const;

      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPMAKEVECTOR_H__
