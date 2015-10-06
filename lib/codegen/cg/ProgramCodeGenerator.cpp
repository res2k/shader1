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

#include "intermediate/Program.h"
#include "FunctionCodeGenerator.h"
#include "splitter/Frequency.h"

#include <boost/make_shared.hpp>
#include <string>

namespace s1
{
  namespace codegen
  {
    StringsArrayPtr CgGenerator::ProgramCodeGenerator::Generate (const intermediate::ProgramPtr& prog,
								 int frequency)
    {
      StringsArrayPtr resultStrings (boost::make_shared<StringsArray> ());
      
      const intermediate::Program::TransferValues& transferValues (prog->GetTransferValues());
      if ((frequency == splitter::freqVertex) && (transferValues.size() > 0))
      {
	resultStrings->AddString ("struct V2F");
	resultStrings->AddString ("{");
	for (intermediate::Program::TransferValues::const_iterator transferVal = transferValues.begin();
	    transferVal != transferValues.end();
	    ++transferVal)
	{
	  std::string identifierSuffix;
	  std::string line ("  ");
	  line.append (CgGenerator::TypeToCgType (transferVal->first, identifierSuffix));
	  line.append (" ");
	  line.append (CgGenerator::NameToCgIdentifier (transferVal->second));
	  line.append (identifierSuffix);
	  line.append (";");
	  resultStrings->AddString (line);
	}
	resultStrings->AddString ("};");
	resultStrings->AddString (std::string ());
      }
      
      for (size_t i = 0; i < prog->GetNumFunctions(); i++)
      {
	intermediate::ProgramFunctionPtr func (prog->GetFunction (i));
	FunctionCodeGenerator funcGen;
        if (func->IsEntryFunction ())
        {
          resultStrings->AddStrings (*(funcGen.Generate ("main",
                                                         func,
                                                         prog->GetOutputParameters (),
                                                         prog->GetParameterArraySizes (),
                                                         transferValues.size () > 0,
                                                         frequency)));
        }
        else
        {
          resultStrings->AddStrings (*(funcGen.Generate (NameToCgIdentifier (func->GetIdentifier ()).c_str(),
                                                         func,
                                                         intermediate::Program::OutputParameters (),
                                                         intermediate::Program::ParameterArraySizes (),
                                                         transferValues.size () > 0,
                                                         frequency)));
        }
	resultStrings->AddString (std::string ());
      }
      
      return resultStrings;
    }
  } // namespace codegen
} // namespace s1

