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
      RegisterID index;
      RegisterID newValue;
    public:
      SequenceOpChangeArrayElement (RegisterID destination,
				    RegisterID source,
				    RegisterID index,
				    RegisterID newValue);
			    
      RegisterIDSet GetReadRegisters () const;

      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPCHANGEARRAYELEMENT_H__
