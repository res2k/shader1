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

#ifndef __INTERMEDIATE_SEQUENCEOPUNARY_H__
#define __INTERMEDIATE_SEQUENCEOPUNARY_H__

#include "SequenceOpWithResult.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpUnary : public SequenceOpWithResult
    {
    protected:
      RegisterPtr source;
    public:
      SequenceOpUnary (RegisterPtr destination,
		       RegisterPtr source);
		       
      RegisterSet GetReadRegisters () const;
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPUNARY_H__
