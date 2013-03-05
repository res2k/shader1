#ifndef __INTERMEDIATE_SEQUENCEOPUNARY_H__
#define __INTERMEDIATE_SEQUENCEOPUNARY_H__

#include "SequenceOpWithResult.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpUnary : public SequenceOpWithResult
    {
    protected:
      RegisterPtr source;
    public:
      SequenceOpUnary (RegisterPtr destination,
		       RegisterPtr source);
		       
      RegisterSet GetReadRegisters () const;
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPUNARY_H__