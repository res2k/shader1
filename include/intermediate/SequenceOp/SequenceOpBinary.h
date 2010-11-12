#ifndef __INTERMEDIATE_SEQUENCEOPBINARY_H__
#define __INTERMEDIATE_SEQUENCEOPBINARY_H__

#include "SequenceOpWithResult.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpBinary : public SequenceOpWithResult
    {
    protected:
      RegisterPtr source1;
      RegisterPtr source2;
    public:
      SequenceOpBinary (RegisterPtr destination,
			RegisterPtr source1,
			RegisterPtr source2);
			
      RegisterSet GetReadRegisters () const;
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPBINARY_H__
