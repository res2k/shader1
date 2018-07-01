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
#include "semantics/Type.h"

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace semantics
  {
    bool Type::CompatibleLossless (BaseType base1, BaseType base2)
    {
      if ((base1 == BaseType::Void) || (base2 == BaseType::Void))
        // Void is not even compatible to itself
        return false;
      if (((base1 == BaseType::Int) || (base1 == BaseType::UInt))
          && ((base2 == BaseType::Int) || (base2 == BaseType::UInt)))
        // int and unsigned int are assumed assignable without precision loss
        return true;
      if (((base1 == BaseType::Int) || (base1 == BaseType::UInt))
          && (base2 == BaseType::Float))
        // assignment from int or unsigned int to float is assumed assignable without precision loss
        return true;
      // Otherwise, types must be equal
      return base1 == base2;
    }

    bool Type::IsPrecisionHigherEqual (BaseType type, BaseType other)
    {
        // Void is not even of equal precision to itself...
        if ((type == BaseType::Void) || (other == BaseType::Void))
          return false;
        // Treat 'int' as higher precision as 'unsigned int' (b/c 'int' has a sign)
        if ((type == BaseType::Int)
            && ((other == BaseType::Int) || (other == BaseType::UInt)))
          return true;
        // Float is considered higher precision than int or unsigned int (has fractions)
        if ((type == BaseType::Float)
            && ((other == BaseType::Int) || (other == BaseType::UInt)))
          // assignment from int or unsigned int to float is assumed assignable without precision loss
          return true;
        // Otherwise, types must be equal
        return type == other;
    }

    uc::String Type::ToString (BaseType base)
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
      S1_ASSERT_NOT_REACHED (uc::String());
    }

    bool Type::CompatibleLossless (const Type& to) const
    {
      if (typeClass != to.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        return CompatibleLossless (base, to.base);
      case Sampler:
        // Samplers have to be equal
        return sampler == to.sampler;
      case Array:
        // Array assignments are compatible when the contained members are
        return GetArrayBase()->CompatibleLossless (*(to.GetArrayBase()));
      case Vector:
        // Vectors: base types must be compatible and both types have the same number of components
        return CompatibleLossless (GetVMBase(), to.GetVMBase())
          && (GetVectorTypeComponents() == to.GetVectorTypeComponents());
      case Matrix:
        // Matrices: base types must be compatible and both types have the same number of rows/cols
        return CompatibleLossless (GetVMBase(), to.GetVMBase())
          && (GetMatrixTypeCols() == to.GetMatrixTypeCols())
          && (GetMatrixTypeRows() == to.GetMatrixTypeRows());
      }
      S1_ASSERT_NOT_REACHED (false);
    }

    bool Type::CompatibleLossy (const Type& to) const
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

    bool Type::IsEqual (const Type& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        return base == other.base;
      case Sampler:
        return sampler == other.sampler;
      case Array:
        return GetArrayBase()->IsEqual (*(other.GetArrayBase()));
      case Vector:
        return (GetVMBase() == other.GetVMBase())
          && (GetVectorTypeComponents() == other.GetVectorTypeComponents());
      case Matrix:
        return (GetVMBase() == other.GetVMBase())
          && (GetMatrixTypeCols() == other.GetMatrixTypeCols())
          && (GetMatrixTypeRows() == other.GetMatrixTypeRows());
      }
      S1_ASSERT_NOT_REACHED (false);
    }

    bool Type::IsPrecisionHigherEqual (const Type& other) const
    {
      if (typeClass != other.typeClass) return false;
      switch (typeClass)
      {
      case Base:
        return IsPrecisionHigherEqual (base, other.base);
      case Sampler:
        // Samplers have to be equal
        return sampler == other.sampler;
      case Array:
        // Array type is higher/equal prec when the contained members are
        return GetArrayBase()->IsPrecisionHigherEqual (*(other.GetArrayBase()));
      case Vector:
        // Vectors: base type is higher/equal prec if both types have the same number of components
        return IsPrecisionHigherEqual (GetVMBase(), other.GetVMBase())
          && (GetVectorTypeComponents() == other.GetVectorTypeComponents());
      case Matrix:
        // Matrices: base type is higher/equal prec if both types have the same number of rows/cols
        return IsPrecisionHigherEqual (GetVMBase(), other.GetVMBase())
          && (GetMatrixTypeCols() == other.GetMatrixTypeCols())
          && (GetMatrixTypeRows() == other.GetMatrixTypeRows());
      }
      S1_ASSERT_NOT_REACHED (false);
    }

    DECLARE_STATIC_FORMATTER(FormatArray, "{0}[]");
    DECLARE_STATIC_FORMATTER(FormatVector, "{0}{1}");
    DECLARE_STATIC_FORMATTER(FormatMatrix, "{0}{1}x{2}");

    uc::String Type::ToString() const
    {
      switch (typeClass)
      {
      case Base:
        return ToString (base);
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
          return FormatArray.to<uc::String> (GetArrayBase()->ToString());
        }
      case Vector:
        {
          return FormatVector.to<uc::String> (ToString (GetVMBase()), GetVectorTypeComponents());
        }
      case Matrix:
        {
          return FormatMatrix.to<uc::String> (ToString (GetVMBase()), GetMatrixTypeCols(), GetMatrixTypeRows());
        }
      }
      S1_ASSERT_NOT_REACHED (uc::String());
    }

    Type* Type::GetHigherPrecisionType (Type* t1, Type* t2)
    {
      if (t1->IsPrecisionHigherEqual (*t2))
        return t1;
      else if (t2->IsPrecisionHigherEqual (*t1))
        return t2;
      return nullptr;
    }

    BaseType Type::DetectNumericType (const uc::String& numericStr)
    {
      if (numericStr.startsWith ("0x") || numericStr.startsWith ("0X"))
      {
        // Hex number: always unsigned int
        return BaseType::UInt;
      }
      if ((numericStr.indexOf ('.') != uc::String::npos)
        || (numericStr.indexOf ('e') != uc::String::npos)
        || (numericStr.indexOf ('E') != uc::String::npos))
      {
        // Contains '.', 'e' or 'E': must be float number
        return BaseType::Float;
      }
      // Can only be an integer
      return numericStr.startsWith ("-") ? BaseType::Int : BaseType::UInt;
    }
  } // namespace semantics
} // namespace s1
