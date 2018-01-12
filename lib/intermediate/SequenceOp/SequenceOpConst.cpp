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
#include "intermediate/SequenceOp/SequenceOpConst.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    void SequenceOpConst::Visit (SequenceVisitor& visitor)
    {
      switch (type)
      {
      case Invalid:
        // Do nothing
        break;
      case Bool:
        visitor.OpConstBool (destination, boolVal);
        break;
      case Int:
        visitor.OpConstInt (destination, intVal);
        break;
      case UInt:
        visitor.OpConstUInt (destination, uintVal);
        break;
      case Float:
        visitor.OpConstFloat (destination, floatVal);
        break;
      }
    }
    
  } // namespace intermediate
} // namespace s1
