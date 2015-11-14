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

#include "codegen/cg/CgGenerator.h"

#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>

namespace s1
{
  namespace codegen
  {
    CgTraits CgTraits::instance;

    CgTraits::CgTraits ()
      : typeStrBool ("bool"), typeStrFloat ("float"), typeStrInt ("int"), typeStrUInt ("unsigned int")
    {
    }

    uc::String CgTraits::ConvertIdentifier (const uc::String& identifier) const
    {
      return CgGenerator::NameToCgIdentifier (identifier).c_str ();
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

    std::pair<uc::String, uc::String> CgTraits::TypeString (const parser::SemanticsHandler::TypePtr& type,
                                                            const size_t* arraySize) const
    {
      std::string typeSuffix;
      std::string sizeStr;
      if (arraySize)
      {
        boost::cnv::spirit cnv;
        if (auto s = boost::convert<std::string> (static_cast<unsigned long> (*arraySize), cnv))
        {
          sizeStr = std::move (*s);
        }
      }
      std::string typeStr = CgGenerator::TypeToCgType (type, typeSuffix, sizeStr);
      return std::make_pair (typeStr.c_str (), typeSuffix.c_str ());
    }
  } // namespace codegen
} // namespace s1
