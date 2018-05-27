/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef SEMANTICS_TYPE_H_
#define SEMANTICS_TYPE_H_

#include "forwarddecl.h"

namespace s1
{
  namespace semantics
  {
    /// Representation of a type
    struct Type
    {
      enum Class
      {
        Base, Sampler, Array, Vector, Matrix
      };

      virtual ~Type() {}
      
      virtual Class GetTypeClass() const = 0;
      virtual BaseType GetBaseType() const = 0;
      virtual SamplerType GetSamplerType() const = 0;
      
      virtual TypePtr GetArrayVectorMatrixBaseType() const = 0;
      
      virtual unsigned int GetVectorTypeComponents() const = 0;
      
      virtual unsigned int GetMatrixTypeCols() const = 0;
      virtual unsigned int GetMatrixTypeRows() const = 0;
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_HANDLER_H_
