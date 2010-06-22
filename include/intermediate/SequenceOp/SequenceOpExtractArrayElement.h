#ifndef __INTERMEDIATE_SEQUENCEOPEXTRACTARRAYELEMENT_H__
#define __INTERMEDIATE_SEQUENCEOPEXTRACTARRAYELEMENT_H__

#include "../BasicType.h"
#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpExtractArrayElement : public SequenceOpUnary
    {
      RegisterID index;
    public:
      SequenceOpExtractArrayElement (RegisterID destination,
				     RegisterID source,
				     RegisterID index);
			    
      RegisterIDSet GetReadRegisters () const;

      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPEXTRACTARRAYELEMENT_H__
