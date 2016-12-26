/*
    Shader1
    Copyright (c) 2016 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef __CLONINGHELPER_H__
#define __CLONINGHELPER_H__

#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace optimize
  {
    class CloningHelper
    {
    public:
      /// Clone a program's "metadata" (excludes functions)
      static intermediate::ProgramPtr CloneProgramMeta (intermediate::ProgramPtr inputProg);
    };
  } // namespace optimize
} // namespace s1

#endif // __CLONINGHELPER_H__
