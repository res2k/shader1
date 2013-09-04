#ifndef __COMMONSEQUENCEVISITOR_H__
#define __COMMONSEQUENCEVISITOR_H__

#include "intermediate/CloningSequenceVisitor.h"

namespace s1
{
  namespace optimize
  {
    class CommonSequenceVisitor : public intermediate::CloningSequenceVisitor
    {
    public:
      typedef intermediate::RegisterPtr RegisterPtr;
      typedef intermediate::Sequence Sequence;
      typedef intermediate::SequenceOpPtr SequenceOpPtr;
      
      CommonSequenceVisitor (const intermediate::SequencePtr& newSequence)
        : CloningSequenceVisitor (newSequence) {}
    };
  } // namespace optimize
} // namespace s1

#endif // __COMMONSEQUENCEVISITOR_H__
