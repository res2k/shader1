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

#include "base/common.h"
#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "base/intrusive_ptr.h"
#include "semantics/CommonType.h"

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace semantics
  {
    bool CommonType::CompatibleLossless (const CommonType& to) const
    {
      if (typeClass != to.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        if ((base == BaseType::Void) || (to.base == BaseType::Void))
          // Void is not even compatible to itself
          return false;
        if (((base == BaseType::Int) || (base == BaseType::UInt))
            && ((to.base == BaseType::Int) || (to.base == BaseType::UInt)))
          // int and unsigned int are assumed assignable without precision loss
          return true;
        if (((base == BaseType::Int) || (base == BaseType::UInt))
            && (to.base == BaseType::Float))
          // assignment from int or unsigned int to float is assumed assignable without precision loss
          return true;
        // Otherwise, types must be equal
        return base == to.base;
      case Sampler:
        // Samplers have to be equal
        return sampler == to.sampler;
      case Array:
        // Array assignments are compatible when the contained members are
        return static_cast<CommonType*> (avmBase.get())->CompatibleLossless (
          *(static_cast<CommonType*> (to.avmBase.get())));
      case Vector:
        // Vectors: base types must be compatible and both types have the same number of components
        return static_cast<CommonType*> (avmBase.get())->CompatibleLossless (
          *(static_cast<CommonType*> (to.avmBase.get())))
          && (vectorDim == to.vectorDim);
      case Matrix:
        // Matrices: base types must be compatible and both types have the same number of rows/cols
        return static_cast<CommonType*> (avmBase.get())->CompatibleLossless (
          *(static_cast<CommonType*> (to.avmBase.get())))
          && (matrixCols == to.matrixCols)
          && (matrixRows == to.matrixRows);
      }
      S1_ASSERT_NOT_REACHED (false);
    }

    bool CommonType::CompatibleLossy (const CommonType& to) const
    {
      // Lossless compatibility implies lossy compatibility ;)
      if (CompatibleLossless (to)) return true;

      if (typeClass != to.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        if ((base == BaseType::Float)
            && ((to.base == BaseType::Int) || (to.base == BaseType::UInt)))
          // assignment from float to int or unsigned int possible with precision loss
          return true;
      default:
        break;
      }
      return false;
    }

    bool CommonType::IsEqual (const CommonType& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        return base == other.base;
      case Sampler:
        return sampler == other.sampler;
      case Array:
        return static_cast<CommonType*> (avmBase.get())->IsEqual (
          *(static_cast<CommonType*> (other.avmBase.get())));
      case Vector:
        return static_cast<CommonType*> (avmBase.get())->IsEqual (
          *(static_cast<CommonType*> (other.avmBase.get())))
          && (vectorDim == other.vectorDim);
      case Matrix:
        return static_cast<CommonType*> (avmBase.get())->IsEqual (
          *(static_cast<CommonType*> (other.avmBase.get())))
          && (matrixCols == other.matrixCols)
          && (matrixRows == other.matrixRows);
      }
      S1_ASSERT_NOT_REACHED (false);
    }

    bool CommonType::IsPrecisionHigherEqual (const CommonType& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        // Void is not even of equal precision to itself...
        if ((base == BaseType::Void) || (other.base == BaseType::Void))
          return false;
        // Treat 'int' as higher precision as 'unsigned int' (b/c 'int' has a sign)
        if ((base == BaseType::Int)
            && ((other.base == BaseType::Int) || (other.base == BaseType::UInt)))
          return true;
        // Float is considered higher precision than int or unsigned int (has fractions)
        if ((base == BaseType::Float)
            && ((other.base == BaseType::Int) || (other.base == BaseType::UInt)))
          // assignment from int or unsigned int to float is assumed assignable without precision loss
          return true;
        // Otherwise, types must be equal
        return base == other.base;
      case Sampler:
        // Samplers have to be equal
        return sampler == other.sampler;
      case Array:
        // Array type is higher/equal prec when the contained members are
        return static_cast<CommonType*> (avmBase.get())->IsPrecisionHigherEqual (
          *(static_cast<CommonType*> (other.avmBase.get())));
      case Vector:
        // Vectors: base type is higher/equal prec if both types have the same number of components
        return static_cast<CommonType*> (avmBase.get())->IsPrecisionHigherEqual (
          *(static_cast<CommonType*> (other.avmBase.get())))
          && (vectorDim == other.vectorDim);
      case Matrix:
        // Matrices: base type is higher/equal prec if both types have the same number of rows/cols
        return static_cast<CommonType*> (avmBase.get())->IsPrecisionHigherEqual (
          *(static_cast<CommonType*> (other.avmBase.get())))
          && (matrixCols == other.matrixCols)
          && (matrixRows == other.matrixRows);
      }
      S1_ASSERT_NOT_REACHED (false);
    }

    DECLARE_STATIC_FORMATTER(FormatArray, "{0}[]");
    DECLARE_STATIC_FORMATTER(FormatVector, "{0}{1}");
    DECLARE_STATIC_FORMATTER(FormatMatrix, "{0}{1}x{2}");

    uc::String CommonType::ToString() const
    {
      switch (typeClass)
      {
      case Base:
        {
          switch (base)
          {
            case BaseType::Invalid: return uc::String ("INVALID");
            case BaseType::Void: return uc::String ("void");
            case BaseType::Bool: return uc::String ("bool");
            case BaseType::Int: return uc::String ("int");
            case BaseType::UInt: return uc::String ("unsigned int");
            case BaseType::Float: return uc::String ("float");
          }
        }
        break;
      case Sampler:
        {
          switch (sampler)
          {
            case SamplerType::_1D: return uc::String ("sampler1D");
            case SamplerType::_2D: return uc::String ("sampler2D");
            case SamplerType::_3D: return uc::String ("sampler3D");
            case SamplerType::CUBE: return uc::String ("samplerCUBE");
          }
        }
        break;
      case Array:
        {
          uc::String s;
          FormatArray (s, static_cast<CommonType*> (avmBase.get())->ToString());
          return s;
        }
      case Vector:
        {
          uc::String s;
          FormatVector (s, static_cast<CommonType*> (avmBase.get())->ToString(), vectorDim);
          return s;
        }
      case Matrix:
        {
          uc::String s;
          FormatMatrix (s, static_cast<CommonType*> (avmBase.get())->ToString(), matrixCols, matrixRows);
          return s;
        }
      }
      S1_ASSERT_NOT_REACHED (uc::String());
    }
  } // namespace semantics
} // namespace s1
