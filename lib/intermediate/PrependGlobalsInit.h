/*
    Shader1
    Copyright (c) 2010-2015 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __PREPENDGLOBALSINIT_H__
#define __PREPENDGLOBALSINIT_H__

#include "intermediate/CloningSequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    /**
     * Sequence visitor to prepend global variables initialization
     * to another sequence.
     */
    class PrependGlobalsInit : public CloningSequenceVisitor
    {
    public:
      /**
       * Construct.
       * \param newSequenceBuilder The Sequence builder the cloned ops are added to.
       * \param oldSequence The original Sequence.
       * \param initSeq Globals initialization sequence to prepend.
       */
      PrependGlobalsInit (const SequenceBuilderPtr& newSequenceBuilder,
                          const SequencePtr& oldSequence,
                          const SequencePtr& initSeq);
    protected:
      CloningSequenceVisitor* Clone (const SequenceBuilderPtr& newSequenceBuilder,
                                     const SequencePtr& oldSequence,
                                     const RegisterMap& regMap);
      CloningSequenceVisitor* Clone (const SequenceBuilderPtr& newSequenceBuilder,
                                     const RegisterMap& regMap);
    };
  } // namespace intermediate
} // namespace s1

#endif // __PREPENDGLOBALSINIT_H__
