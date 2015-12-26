/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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

#include "ProgramCodeGenerator.h"

#include "CgTraits.h"
#include "FunctionCodeGenerator.h"

#include "codegen/cg/CgOptions.h"
#include "splitter/Frequency.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace codegen
  {
    CgGenerator::ProgramCodeGenerator::ProgramCodeGenerator (const CgOptions& options)
      : sl::ProgramCodeGenerator (CgTraits::instance, options)
    {
    }

    const CgOptions& CgGenerator::ProgramCodeGenerator::GetCgOptions () const
    {
      return static_cast<const CgOptions&> (options);
    }

    StringsArrayPtr CgGenerator::ProgramCodeGenerator::GeneratePreamble (const intermediate::ProgramPtr & prog, int frequency)
    {
      StringsArrayPtr resultStrings = boost::make_shared<StringsArray> ();

      const intermediate::Program::TransferValues& transferValues (prog->GetTransferValues ());
      if ((frequency == splitter::freqVertex) && (transferValues.size () > 0))
      {
        resultStrings->AddString ("struct V2F");
        resultStrings->AddString ("{");
        for (intermediate::Program::TransferValues::const_iterator transferVal = transferValues.begin ();
        transferVal != transferValues.end ();
          ++transferVal)
        {
          std::string identifierSuffix;
          std::string line ("  ");
          auto typeStrings = traits.TypeString (transferVal->first, nullptr);
          typeStrings.first.toUTF8String (line);
          line.append (" ");
          uc::String identStr = traits.ConvertIdentifier (transferVal->second);
          identStr.toUTF8String (line);
          typeStrings.second.toUTF8String (line);
          line.append (";");
          resultStrings->AddString (line);
        }
        resultStrings->AddString ("};");
        resultStrings->AddString (std::string ());
      }
      return resultStrings;
    }

    std::unique_ptr<sl::FunctionCodeGenerator> CgGenerator::ProgramCodeGenerator::CreateFunctionCodeGenerator ()
    {
      return std::unique_ptr<sl::FunctionCodeGenerator> (new FunctionCodeGenerator (GetCgOptions()));
    }

  } // namespace codegen
} // namespace s1

