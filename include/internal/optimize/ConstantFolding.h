#ifndef __CONSTANTFOLDING_H__
#define __CONSTANTFOLDING_H__

#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace optimize
  {
    class ConstantFolding
    {
      class FoldingVisitor;
    public:
      static bool FoldConstants (const intermediate::SequencePtr& outputSeq,
				 const intermediate::SequencePtr& inputSeq);
    };
  } // namespace optimize
} // namespace s1

#endif // __CONSTANTFOLDING_H__
