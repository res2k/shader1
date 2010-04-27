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
      Sequence::RegisterID source;
    public:
      SequenceOpUnary (Sequence::RegisterID destination,
		       Sequence::RegisterID source);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPUNARY_H__
