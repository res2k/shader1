#ifndef __INTERMEDIATE_SEQUENCEOPMAKEARRAY_H__
#define __INTERMEDIATE_SEQUENCEOPMAKEARRAY_H__

#include "../BasicType.h"
#include "SequenceOpWithResult.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpMakeArray : public SequenceOpWithResult
    {
      std::vector<RegisterPtr> sources;
    public:
      SequenceOpMakeArray (RegisterPtr destination,
			   const std::vector<RegisterPtr>& sources);
			    
      RegisterSet GetReadRegisters () const;

      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPMAKEARRAY_H__
