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

#ifndef __INTERMEDIATE_TYPEIMPL_H__
#define __INTERMEDIATE_TYPEIMPL_H__

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"

namespace s1
{
  namespace intermediate
  {
    struct IntermediateGeneratorSemanticsHandler::TypeImpl : public CommonType
    {
      TypeImpl (BaseType base) : CommonType (base) {}
      TypeImpl (SamplerType sampler) : CommonType (sampler) {}
      TypeImpl (TypePtr aBase) : CommonType (aBase) {}
      TypeImpl (TypePtr vBase, unsigned int d) : CommonType (vBase, d) {}
      TypeImpl (TypePtr mBase, unsigned int c, unsigned int r)
        : CommonType (mBase, c, r) {}
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_TYPEIMPL_H__
