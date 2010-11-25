#ifndef __INLINER_H__
#define __INLINER_H__

#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace optimize
  {
    class Inliner
    {
      class InlineBlockVisitor;
      class InliningVisitor;
    public:
      static bool InlineAllBlocks (const intermediate::SequencePtr& outputSeq,
				   const intermediate::SequencePtr& inputSeq);
    };
  } // namespace optimize
} // namespace s1

#endif // __INLINER_H__
