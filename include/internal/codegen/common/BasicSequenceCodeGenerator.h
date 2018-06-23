/*
Shader1
Copyright (c) 2015 Frank Richter


This library is free software; you can redistribute it and/or
modify it under the terms of the wxWindows Library Licence.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Please see the wxWindows Library Licence for more details.
The licence texts can be found in the source distribution files
LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __CODEGEN_BASICSEQUENCECODEGENERATOR_H__
#define __CODEGEN_BASICSEQUENCECODEGENERATOR_H__

#include "codegen/common/StringsArray.h"
#include "intermediate/Sequence.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace codegen
  {
    class BasicSequenceCodeGenerator
    {
    protected:
      typedef intermediate::RegisterPtr RegisterPtr;
      typedef intermediate::Sequence Sequence;

      class Visitor : public intermediate::SequenceVisitor
      {
      protected:
        StringsArrayPtr target;
      public:
        Visitor (const StringsArrayPtr& target);
      };

      const intermediate::Sequence& seq;
      StringsArrayPtr strings;
    public:
      BasicSequenceCodeGenerator (const intermediate::Sequence& seq);
      virtual ~BasicSequenceCodeGenerator() {}

      virtual StringsArrayPtr Generate () = 0;
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_BASICSEQUENCECODEGENERATOR_H__
