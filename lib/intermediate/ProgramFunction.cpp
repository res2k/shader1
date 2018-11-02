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

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/ProgramFunction.h"

namespace s1
{
  namespace intermediate
  {
    ProgramFunction::ProgramFunction (const uc::String& originalIdentifier,
                                      const uc::String& identifier,
                                      const semantics::FunctionFormalParameters& params,
                                      const SequencePtr& body,
                                      bool isEntryFunction)
     : originalIdentifier (originalIdentifier) ,identifier (identifier),
       params (params), body (body),
       isEntryFunction (isEntryFunction)
    {
    }

    void ProgramFunction::SetTransferMapping (const uc::String& transferVal,
                                              const RegisterPtr& programReg)
    {
      transferMappings.emplace_back (transferVal, programReg);
    }

    void ProgramFunction::AddTransferMappings (const TransferMappings& mappings)
    {
      transferMappings.reserve (transferMappings.size () + mappings.size ());
      for (const auto& m : mappings)
      {
        SetTransferMapping (m.first, m.second);
      }
    }

    void ProgramFunction::AddParameterFrequencies (const ParameterFrequencyMap& freqs)
    {
      paramFreqMap.reserve (paramFreqMap.size () + freqs.size ());
      paramFreqMap.insert (freqs.begin (), freqs.end ());
    }
  } // namespace intermediate
} // namespace s1

