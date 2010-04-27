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
      Sequence::RegisterID source1;
      Sequence::RegisterID source2;
    public:
      SequenceOpBinary (Sequence::RegisterID destination,
			Sequence::RegisterID source1,
			Sequence::RegisterID source2);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPBINARY_H__
