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

#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpChangeArrayElement.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpChangeArrayElement::SequenceOpChangeArrayElement (RegisterPtr destination,
								RegisterPtr source,
								RegisterPtr index,
								RegisterPtr newValue)
     : SequenceOpUnary (destination, source), index (index), newValue (newValue)
    {}
			    
    RegisterSet SequenceOpChangeArrayElement::GetReadRegisters () const
    {
      RegisterSet set (SequenceOpUnary::GetReadRegisters ());
      set.insert (index);
      set.insert (newValue);
      return set;
    }

    void SequenceOpChangeArrayElement::Visit (SequenceVisitor& visitor)
    {
      visitor.OpChangeArrayElement (destination, source, index, newValue);
    }
    
  } // namespace intermediate
} // namespace s1
