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
      struct ArrayType
      {
        TypePtr base;
      };
      struct VectorType
      {
        TypePtr base;
        unsigned int dim;
      };
      struct MatrixType
      {
        TypePtr base;
        unsigned int cols;
        unsigned int rows;
      };
      Class typeClass;
      union
      {
        BaseType base;
        SamplerType sampler;
        ArrayType arrayData;
        VectorType vecData;
        MatrixType matData;
      };
    public:
      Type (BaseType base) : typeClass (Base), base (base) {}
      Type (SamplerType sampler) : typeClass (Sampler), sampler (sampler) {}
      Type (TypePtr aBase) : typeClass (Array)
      {
        new (&arrayData) ArrayType;
        arrayData.base = aBase;
      }
      Type (TypePtr vBase, unsigned int d) : typeClass (Vector)
      {
        new (&vecData) VectorType;
        vecData.base = vBase;
        vecData.dim = d;
      }
      Type (TypePtr mBase, unsigned int c, unsigned int r) : typeClass (Matrix)
      {
        new (&matData) MatrixType;
        matData.base = mBase;
        matData.cols = c;
        matData.rows = r;
      }

      ~Type ()
      {
        if (typeClass == Array)
          arrayData.~ArrayType();
        else if (typeClass == Vector)
          vecData.~VectorType();
        else if (typeClass == Matrix)
          matData.~MatrixType();
      }

      Class GetTypeClass() const { return typeClass; }
      BaseType GetBaseType() const { S1_ASSERT(typeClass == Base, BaseType::Invalid); return base; }
      SamplerType GetSamplerType() const { S1_ASSERT(typeClass == Sampler, SamplerType::_2D); return sampler; }

      Type* GetAVMBase() const
      {
        S1_ASSERT((typeClass == Array) || (typeClass == Vector) || (typeClass == Matrix), nullptr);
        if (typeClass == Array)
          return arrayData.base.get();
        else if (typeClass == Vector)
          return vecData.base.get();
        else
          return matData.base.get();
      }

      unsigned int GetVectorTypeComponents() const { S1_ASSERT(typeClass == Vector, 0); return vecData.dim; }

      unsigned int GetMatrixTypeCols() const { S1_ASSERT(typeClass == Matrix, 0); return matData.cols; }
      unsigned int GetMatrixTypeRows() const { S1_ASSERT(typeClass == Matrix, 0); return matData.rows; }

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
