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
        BaseType base;
        unsigned int dim;
      };
      struct MatrixType
      {
        BaseType base;
        unsigned int cols;
        unsigned int rows;
      };
      Class typeClass;
      union
      {
        BaseType base;
        SamplerType sampler;
        ArrayType array;
        VectorType vec;
        MatrixType mat;
      };

      static bool CompatibleLossless (BaseType base1, BaseType base2);
      static bool IsPrecisionHigherEqual (BaseType type, BaseType other);
      static uc::String ToString (BaseType base);
    public:
      Type (BaseType base) : typeClass (Base), base (base) {}
      Type (SamplerType sampler) : typeClass (Sampler), sampler (sampler) {}
      Type (TypePtr aBase) : typeClass (Array)
      {
        new (&array) ArrayType;
        array.base = aBase;
      }
      Type (BaseType vBase, unsigned int d) : typeClass (Vector)
      {
        vec.base = vBase;
        vec.dim = d;
      }
      Type (BaseType mBase, unsigned int c, unsigned int r) : typeClass (Matrix)
      {
        mat.base = mBase;
        mat.cols = c;
        mat.rows = r;
      }

      ~Type ()
      {
        if (typeClass == Array)
          array.~ArrayType();
      }

      Class GetTypeClass() const { return typeClass; }
      BaseType GetBaseType() const { S1_ASSERT(typeClass == Base, BaseType::Invalid); return base; }
      SamplerType GetSamplerType() const { S1_ASSERT(typeClass == Sampler, SamplerType::_2D); return sampler; }

      Type* GetArrayBase() const
      {
        S1_ASSERT(typeClass == Array, nullptr);
        return array.base.get();
      }
      BaseType GetVMBase() const
      {
        S1_ASSERT((typeClass == Vector) || (typeClass == Matrix), BaseType::Invalid);
        if (typeClass == Vector)
          return vec.base;
        else
          return mat.base;
      }

      unsigned int GetVectorTypeComponents() const { S1_ASSERT(typeClass == Vector, 0); return vec.dim; }

      unsigned int GetMatrixTypeCols() const { S1_ASSERT(typeClass == Matrix, 0); return mat.cols; }
      unsigned int GetMatrixTypeRows() const { S1_ASSERT(typeClass == Matrix, 0); return mat.rows; }

      /// Returns whether this type is losslessly assignable to \a to.
      bool CompatibleLossless (const Type& to) const;
      bool CompatibleLossy (const Type& to) const;
      bool IsEqual (const Type& other) const;
      bool IsPrecisionHigherEqual (const Type& other) const;

      uc::String ToString() const;

      /**\name Type utilities
       * @{ */
      static Type* GetHigherPrecisionType (Type* t1, Type* t2);
      static BaseType DetectNumericType (const uc::String& numericStr);
      /** @} */
    };
  } // namespace semantics
} // namespace s1

#endif // SEMANTICS_HANDLER_H_
