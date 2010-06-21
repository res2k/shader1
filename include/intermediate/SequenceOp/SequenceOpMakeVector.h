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
      std::vector<RegisterID> sources;
    public:
      SequenceOpMakeVector (RegisterID destination,
			    BasicType vectorBaseType,
			    const std::vector<RegisterID>& sources);
			    
      RegisterIDSet GetReadRegisters () const;

      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPMAKEVECTOR_H__
