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
      static bool InlineAllBlocks (const intermediate::SequenceBuilderPtr& outputSeqBuilder,
				   const intermediate::SequencePtr& inputSeq);
    };
  } // namespace optimize
} // namespace s1

#endif // __INLINER_H__
