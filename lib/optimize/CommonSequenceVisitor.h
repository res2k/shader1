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
      typedef intermediate::SequencePtr SequencePtr;
      typedef intermediate::SequenceOpPtr SequenceOpPtr;
      
      CommonSequenceVisitor (const intermediate::SequencePtr& newSequence)
        : CloningSequenceVisitor (newSequence) {}

      // Overridden by optimizing visitors
      virtual void PostVisitSequence (CommonSequenceVisitor* visitor,
				      const SequencePtr& newSequence,
				      const RegisterMap& regMap)
      { }
      // Simply to wrap the method taking a CommonSequenceVisitor*
      virtual void PostVisitSequence (CloningSequenceVisitor* visitor,
				      const SequencePtr& newSequence,
				      const RegisterMap& regMap)
      { PostVisitSequence (static_cast<CommonSequenceVisitor*> (visitor), newSequence, regMap); }
    };
  } // namespace optimize
} // namespace s1

#endif // __COMMONSEQUENCEVISITOR_H__
