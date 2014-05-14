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
#include "intermediate/SequenceOp/SequenceOpMakeMatrix.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpMakeMatrix::SequenceOpMakeMatrix (RegisterPtr destination,
						BasicType matrixBaseType,
						unsigned int matrixRows, unsigned int matrixCols,
						const std::vector<RegisterPtr>& sources)
      : SequenceOpWithResult (destination), matrixBaseType (matrixBaseType),
	matrixRows (matrixRows), matrixCols (matrixCols), sources (sources)
    {}
			  
    RegisterSet SequenceOpMakeMatrix::GetReadRegisters () const
    {
      RegisterSet set;
      set.insert (sources.begin(), sources.end());
      return set;
    }

    void SequenceOpMakeMatrix::Visit (SequenceVisitor& visitor)
    {
      visitor.OpMakeMatrix (destination, matrixBaseType, matrixRows, matrixCols, sources);
    }
  } // namespace intermediate
} // namespace s1
