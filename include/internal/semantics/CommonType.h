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

#ifndef SEMANTICS_COMMONTYPE_H_
#define SEMANTICS_COMMONTYPE_H_

#include "Type.h"

namespace s1
{
  namespace semantics
  {
    struct CommonType : public Type
    {
      Class typeClass;
      BaseType base;
      SamplerType sampler;
      TypePtr avmBase;
      unsigned int vectorDim;
      unsigned int matrixCols;
      unsigned int matrixRows;

      CommonType (BaseType base) : typeClass (Base), base (base) {}
      CommonType (SamplerType sampler) : typeClass (Sampler), sampler (sampler) {}
      CommonType (TypePtr aBase) : typeClass (Array), avmBase (aBase) {}
      CommonType (TypePtr vBase, unsigned int d)
        : typeClass (Vector), avmBase (vBase), vectorDim (d) {}
      CommonType (TypePtr mBase, unsigned int c, unsigned int r)
        : typeClass (Matrix), avmBase (mBase), matrixCols (c), matrixRows (r) {}

      Class GetTypeClass() const { return typeClass; }
      BaseType GetBaseType() const { return base; }
      SamplerType GetSamplerType() const { return sampler; }

      TypePtr GetArrayVectorMatrixBaseType() const { return avmBase; }

      unsigned int GetVectorTypeComponents() const { return vectorDim; }

      unsigned int GetMatrixTypeCols() const { return matrixCols; }
      unsigned int GetMatrixTypeRows() const { return matrixRows; }

      /// Returns whether this type is losslessly assignable to \a to.
      bool CompatibleLossless (const CommonType& to) const;
      bool CompatibleLossy (const CommonType& to) const;
      bool IsEqual (const CommonType& other) const;
      bool IsPrecisionHigherEqual (const CommonType& other) const;

      uc::String ToString() const;
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_COMMONTYPE_H_
