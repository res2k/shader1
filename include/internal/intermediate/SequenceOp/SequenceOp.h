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

#ifndef __INTERMEDIATE_SEQUENCEOP_H__
#define __INTERMEDIATE_SEQUENCEOP_H__

#include "../Sequence.h"

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

namespace s1
{
  namespace intermediate
  {
    struct SequenceVisitor;
    
    class SequenceOp
      : public boost::intrusive_ref_counter<SequenceOp,
                                            boost::thread_unsafe_counter>
    {
    public:
      virtual ~SequenceOp() {}
      
      /// Get the registers read from by this operation
      virtual RegisterSet GetReadRegisters () const = 0;
      /// Get the registers written to by this operation
      virtual RegisterSet GetWrittenRegisters () const = 0;
      
      virtual void Visit (SequenceVisitor& visitor) = 0;
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOP_H__
