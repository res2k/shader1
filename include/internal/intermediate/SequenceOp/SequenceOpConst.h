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

#ifndef __INTERMEDIATE_SEQUENCEOPCONST_H__
#define __INTERMEDIATE_SEQUENCEOPCONST_H__

#include "../BasicType.h"
#include "SequenceOpWithResult.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpConst : public SequenceOpWithResult
    {
      BasicType type;
      union
      {
	bool boolVal;
	int intVal;
	unsigned int uintVal;
	float floatVal;
      };
    public:
      SequenceOpConst (RegisterPtr destination, bool val)
       : SequenceOpWithResult (destination), type (BasicType::Bool), boolVal (val) {}
      SequenceOpConst (RegisterPtr destination, int val)
       : SequenceOpWithResult (destination), type (BasicType::Int), intVal (val) {}
      SequenceOpConst (RegisterPtr destination, unsigned int val)
       : SequenceOpWithResult (destination), type (BasicType::UInt), uintVal (val) {}
      SequenceOpConst (RegisterPtr destination, float val)
       : SequenceOpWithResult (destination), type (BasicType::Float), floatVal (val) {}

      RegisterSet GetReadRegisters () const { return RegisterSet(); }
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPCONST_H__
