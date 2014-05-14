/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

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
