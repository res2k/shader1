/*
Shader1
Copyright (c) 2015 Frank Richter


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

#include "GLSLTraits.h"

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "base/uc/boost_convert.h"
#include "semantics/Type.h"

#include <boost/convert.hpp>
#include "base/boost_convert_spirit.hpp"

#include "base/format/Formatter.tpp"

#include <assert.h>

namespace s1
{
  namespace codegen
  {
    namespace glsl
    {
      uc::String Traits::typeStrInvalid ("INVALID");
      uc::String Traits::typeStrBool ("bool");
      uc::String Traits::typeStrFloat ("float");
      uc::String Traits::typeStrInt ("int");

      Traits::Traits ()
      {
      }

      const char* Traits::GetVecTypePrefix (intermediate::BasicType type) const
      {
        const char* prefix = "";

        switch (type)
        {
        case intermediate::BasicType::Invalid:
          prefix = "INVALID"; // produce invalid code
          break;
        case intermediate::BasicType::Bool:
          prefix = "b";
          break;
        case intermediate::BasicType::UInt:
          // TODO: unsigned integer - added some time between GLSL 1.10 and 4.50
        case intermediate::BasicType::Int:
          prefix = "i";
          break;
        case intermediate::BasicType::Float:
          // default prefix
          break;
        }
        return prefix;
      }

      uc::String Traits::TypeString (intermediate::BasicType type) const
      {
        switch (type)
        {
        case intermediate::BasicType::Invalid:
          return typeStrInvalid;
        case intermediate::BasicType::Bool:
          return typeStrBool;
        case intermediate::BasicType::Float:
          return typeStrFloat;
        case intermediate::BasicType::UInt:
          // TODO: unsigned integer - added some time between GLSL 1.10 and 4.50
        case intermediate::BasicType::Int:
          return typeStrInt;
        }

        return uc::String ();
      }

      DECLARE_STATIC_FORMATTER(FormatTypeVector, "{0}vec{1}");
      DECLARE_STATIC_FORMATTER(FormatTypeMatrixShort, "mat{0}");
      DECLARE_STATIC_FORMATTER(FormatTypeMatrix, "mat{0}x{1}");
      DECLARE_STATIC_FORMATTER(FormatSuffix, "{0}[{1}]");

      uc::String Traits::FormatVector (intermediate::BasicType type, unsigned int componentCount) const
      {
        return FormatTypeVector.to<uc::String> (GetVecTypePrefix (type), componentCount);
      }

      uc::String Traits::FormatMatrix (intermediate::BasicType type, unsigned int rowCount, unsigned int colCount) const
      {
        // FIXME: GLSL doesn't know non-float(double) matrices!
        if (rowCount == colCount)
          return FormatTypeMatrixShort.to<uc::String> (rowCount);
        else
          return FormatTypeMatrix.to<uc::String> (rowCount, colCount);
      }

      std::pair<uc::String, uc::String> Traits::TypeString (const semantics::TypePtr& type,
                                                            const size_t* arraySize) const
      {
        uc::String sizeStr;
        if (arraySize)
        {
          boost::cnv::spirit cnv;
          if (auto s = boost::convert<uc::String> (*arraySize, cnv))
          {
            sizeStr = std::move (*s);
          }
        }

        uc::String typeStr;
        uc::String identifierSuffix;
        switch (type->GetTypeClass ())
        {
        case semantics::Type::Base:
          {
            switch (type->GetBaseType ())
            {
            case semantics::BaseType::Invalid:
              typeStr = typeStrInvalid;
              break;
            case semantics::BaseType::Void:	typeStr = "void"; break;
            case semantics::BaseType::Bool:	typeStr = typeStrBool; break;
            case semantics::BaseType::Int:
            case semantics::BaseType::UInt:
              typeStr = typeStrInt;
              break;
            case semantics::BaseType::Float:	typeStr = typeStrFloat; break;
            }
          }
          break;
        case semantics::Type::Sampler:
          {
            switch (type->GetSamplerType ())
            {
            case semantics::SamplerType::_1D:	  typeStr = "sampler1D"; break;
            case semantics::SamplerType::_2D:	  typeStr = "sampler2D"; break;
            case semantics::SamplerType::_3D:	  typeStr = "sampler3D"; break;
            case semantics::SamplerType::CUBE:  typeStr = "samplerCube"; break;
            }
          }
          break;
        case semantics::Type::Array:
          {
            auto innerTypeStrs = TypeString (type->GetArrayVectorMatrixBaseType (), nullptr);
            return std::make_pair (std::move (innerTypeStrs.first),
                                   FormatSuffix.to<uc::String> (innerTypeStrs.second, sizeStr));
          }
          break;
        case semantics::Type::Vector:
          {
            typeStr = FormatVector (ConvertBasicType (type->GetArrayVectorMatrixBaseType ()->GetBaseType()),
                                    type->GetVectorTypeComponents());
          }
          break;
        case semantics::Type::Matrix:
          {
            typeStr = FormatMatrix (ConvertBasicType (type->GetArrayVectorMatrixBaseType ()->GetBaseType()),
                                    type->GetMatrixTypeRows (), type->GetMatrixTypeCols ());
          }
          break;
        }
        return std::make_pair (std::move (typeStr), std::move (identifierSuffix));
      }
    } // namespace glsl
  } // namespace codegen
} // namespace s1
