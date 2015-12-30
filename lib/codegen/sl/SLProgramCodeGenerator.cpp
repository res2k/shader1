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

#include "codegen/sl/SLProgramCodeGenerator.h"

#include "codegen/sl/SLTraits.h"
#include "codegen/sl/SLFunctionCodeGenerator.h"
#include "intermediate/Program.h"
#include "splitter/Frequency.h"

#include <boost/make_shared.hpp>
#include <string>

namespace s1
{
  namespace codegen
  {
    namespace sl
    {
      ProgramCodeGenerator::ProgramCodeGenerator (const Traits& traits, const Options& options)
        : traits (traits), options (options)
      {
      }

      StringsArrayPtr ProgramCodeGenerator::Generate (const intermediate::ProgramPtr& prog,
                                                      int frequency)
      {
        StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());

        if (StringsArrayPtr preamble = GeneratePreamble (prog, frequency))
        {
          resultStrings->AddStrings (*preamble);
          if (resultStrings->Size () > 0) resultStrings->AddString ("");
        }

        const intermediate::Program::TransferValues& transferValues (prog->GetTransferValues ());
        for (size_t i = 0; i < prog->GetNumFunctions (); i++)
        {
          intermediate::ProgramFunctionPtr func (prog->GetFunction (i));
          auto funcGen = CreateFunctionCodeGenerator ();
          if (func->IsEntryFunction ())
          {
            resultStrings->AddStrings (*(funcGen->Generate ("main", func, prog, frequency)));
          }
          else
          {
            std::string funcName;
            traits.ConvertIdentifier (func->GetIdentifier ()).toUTF8String (funcName);
            resultStrings->AddStrings (*(funcGen->Generate (funcName.c_str (), func, prog, frequency)));
          }
          resultStrings->AddString (std::string ());
        }

        return resultStrings;
      }
    } // namespace sl
  } // namespace codegen
} // namespace s1

