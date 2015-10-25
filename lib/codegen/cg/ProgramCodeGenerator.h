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

#ifndef __CODEGEN_PROGRAMCODEGENERATOR_H__
#define __CODEGEN_PROGRAMCODEGENERATOR_H__

#include "codegen/CgGenerator.h"
#include "codegen/common/StringsArray.h"
#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace codegen
  {
    class CgOptions;

    class CgGenerator::ProgramCodeGenerator
    {
      const CgOptions& options;
    public:
      ProgramCodeGenerator (const CgOptions& options);

      StringsArrayPtr Generate (const intermediate::ProgramPtr& prog, int frequency);
    };
  } // namespace codegen
} // namespace s1

#endif // __CODEGEN_PROGRAMCODEGENERATOR_H__
