#ifndef __INTERMEDIATE_SEQUENCEOPCHANGEARRAYELEMENT_H__
#define __INTERMEDIATE_SEQUENCEOPCHANGEARRAYELEMENT_H__

#include "../BasicType.h"
#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpChangeArrayElement : public SequenceOpUnary
    {
      RegisterPtr index;
      RegisterPtr newValue;
    public:
      SequenceOpChangeArrayElement (RegisterPtr destination,
				    RegisterPtr source,
				    RegisterPtr index,
				    RegisterPtr newValue);
			    
      RegisterSet GetReadRegisters () const;

      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPCHANGEARRAYELEMENT_H__
