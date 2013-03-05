#ifndef __INTERMEDIATE_SEQUENCEOPEXTRACTVECTORCOMPONENT_H__
#define __INTERMEDIATE_SEQUENCEOPEXTRACTVECTORCOMPONENT_H__

#include "../BasicType.h"
#include "SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpExtractVectorComponent : public SequenceOpUnary
    {
      unsigned int comp;
    public:
      SequenceOpExtractVectorComponent (RegisterPtr destination,
					RegisterPtr source,
					unsigned int vecComp);
			    
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPEXTRACTVECTORCOMPONENT_H__
