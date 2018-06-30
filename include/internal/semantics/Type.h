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

#include "Base.h"

namespace s1
{
  namespace semantics
  {
    /// Representation of a type
    class Type : public Base
    {
    public:
      enum Class
      {
        Base, Sampler, Array, Vector, Matrix
      };
    protected:
      Class typeClass;
      BaseType base;
      SamplerType sampler;
      TypePtr avmBase;
      unsigned int vectorDim;
      unsigned int matrixCols;
      unsigned int matrixRows;
    public:
      Type (BaseType base) : typeClass (Base), base (base) {}
      Type (SamplerType sampler) : typeClass (Sampler), sampler (sampler) {}
      Type (TypePtr aBase) : typeClass (Array), avmBase (aBase) {}
      Type (TypePtr vBase, unsigned int d)
        : typeClass (Vector), avmBase (vBase), vectorDim (d) {}
      Type (TypePtr mBase, unsigned int c, unsigned int r)
        : typeClass (Matrix), avmBase (mBase), matrixCols (c), matrixRows (r) {}

      Class GetTypeClass() const { return typeClass; }
      BaseType GetBaseType() const { return base; }
      SamplerType GetSamplerType() const { return sampler; }

      Type* GetAVMBase() const { return avmBase.get(); }

      unsigned int GetVectorTypeComponents() const { return vectorDim; }

      unsigned int GetMatrixTypeCols() const { return matrixCols; }
      unsigned int GetMatrixTypeRows() const { return matrixRows; }

      /// Returns whether this type is losslessly assignable to \a to.
      bool CompatibleLossless (const Type& to) const;
      bool CompatibleLossy (const Type& to) const;
      bool IsEqual (const Type& other) const;
      bool IsPrecisionHigherEqual (const Type& other) const;

      uc::String ToString() const;
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_HANDLER_H_
