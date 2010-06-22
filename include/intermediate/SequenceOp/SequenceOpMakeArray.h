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
      std::vector<RegisterID> sources;
    public:
      SequenceOpMakeArray (RegisterID destination,
			   const std::vector<RegisterID>& sources);
			    
      RegisterIDSet GetReadRegisters () const;

      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPMAKEARRAY_H__
