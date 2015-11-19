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

#include "CgTraits.h"

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"

#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>

#include "base/format/Formatter.tpp"

namespace s1
{
  namespace codegen
  {
    CgTraits CgTraits::instance;

    CgTraits::CgTraits ()
      : typeStrBool ("bool"),
        typeStrFloat ("float"),
        typeStrInt ("int"),
        typeStrUInt ("unsigned int")
    {
    }

    uc::String CgTraits::TypeString (intermediate::BasicType type) const
    {
      switch (type)
      {
      case intermediate::Bool:
        return typeStrBool;
      case intermediate::Float:
        return typeStrFloat;
      case intermediate::Int:
        return typeStrInt;
      case intermediate::UInt:
        return typeStrUInt;
      }

      return uc::String ();
    }

    static format::StaticFormatter FormatTypeVector ("{0}{1}");
    static format::StaticFormatter FormatTypeMatrix ("{0}{1}x{2}");
    static format::StaticFormatter FormatSuffix ("{0}[{1}]");

    std::pair<uc::String, uc::String> CgTraits::TypeString (const parser::SemanticsHandler::TypePtr& type,
                                                            const size_t* arraySize) const
    {
      uc::String sizeStr;
      if (arraySize)
      {
        boost::cnv::spirit cnv;
        if (auto s = boost::convert<std::string> (static_cast<unsigned long> (*arraySize), cnv))
        {
          sizeStr = s->c_str ();
        }
      }

      uc::String typeStr;
      uc::String identifierSuffix;
      switch (type->GetTypeClass())
      {
      case parser::SemanticsHandler::Type::Base:
        {
          switch (type->GetBaseType())
          {
          case parser::SemanticsHandler::Void:	typeStr = "void"; break;
          case parser::SemanticsHandler::Bool:	typeStr = typeStrBool; break;
          case parser::SemanticsHandler::Int:	typeStr = typeStrInt; break;
          case parser::SemanticsHandler::UInt:	typeStr = typeStrUInt; break;
          case parser::SemanticsHandler::Float:	typeStr = typeStrFloat; break;
          }
        }
      break;
      case parser::SemanticsHandler::Type::Sampler:
        {
          switch (type->GetSamplerType())
          {
          case parser::SemanticsHandler::_1D:	typeStr = "sampler1D"; break;
          case parser::SemanticsHandler::_2D:	typeStr = "sampler2D"; break;
          case parser::SemanticsHandler::_3D:	typeStr = "sampler3D"; break;
          case parser::SemanticsHandler::CUBE:	typeStr = "samplerCUBE"; break;
          }
        }
      break;
      case parser::SemanticsHandler::Type::Array:
        {
          auto innerTypeStrs = TypeString (type->GetArrayVectorMatrixBaseType (), nullptr);
          uc::String newSuffix;
          FormatSuffix (newSuffix, innerTypeStrs.second, sizeStr);
          return std::make_pair (std::move (innerTypeStrs.first), std::move (newSuffix));
        }
      break;
      case parser::SemanticsHandler::Type::Vector:
        {
          auto innerTypeStrs = TypeString (type->GetArrayVectorMatrixBaseType (), nullptr);
          FormatTypeVector (typeStr, innerTypeStrs.first, type->GetVectorTypeComponents());
        }
        break;
      case parser::SemanticsHandler::Type::Matrix:
        {
          auto innerTypeStrs = TypeString (type->GetArrayVectorMatrixBaseType (), nullptr);
          FormatTypeMatrix (typeStr, innerTypeStrs.first, type->GetMatrixTypeRows(), type->GetMatrixTypeCols());
        }
        break;
      }
      return std::make_pair (std::move (typeStr), std::move (identifierSuffix));
    }
  } // namespace codegen
} // namespace s1
