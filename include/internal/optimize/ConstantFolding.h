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
      static bool FoldConstants (const intermediate::SequenceBuilderPtr& outputSeqBuilder,
                                 const intermediate::SequencePtr& inputSeq);
    };
  } // namespace optimize
} // namespace s1

#endif // __CONSTANTFOLDING_H__
