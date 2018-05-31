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

#ifndef __INTERMEDIATE_PROGRAMFUNCTION_H__
#define __INTERMEDIATE_PROGRAMFUNCTION_H__

#include "intermediate/Sequence.h"
#include "semantics/Handler.h"

#include "forwarddecl.h"

namespace s1
{
  namespace intermediate
  {
    class ProgramFunction
    {
      uc::String originalIdentifier;
      uc::String identifier;
      semantics::Scope::FunctionFormalParameters params;
      SequencePtr body;
      bool isEntryFunction;
    public:
      ProgramFunction (const uc::String& originalIdentifier,
		       const uc::String& identifier,
		       const semantics::Scope::FunctionFormalParameters& params,
		       const SequencePtr& body,
		       bool isEntryFunction);

      const uc::String& GetOriginalIdentifier() const
      { return originalIdentifier; }
      const uc::String& GetIdentifier() const
      { return identifier; }
      const semantics::Scope::FunctionFormalParameters& GetParams() const
      { return params; }
      SequencePtr GetBody() const
      { return body; }
      bool IsEntryFunction() const
      { return isEntryFunction; }
      
      void SetTransferMapping (const uc::String& transferVal,
			       const RegisterPtr& programReg);
      typedef std::pair<uc::String, RegisterPtr> TransferMappingPair;
      typedef std::vector<TransferMappingPair> TransferMappings;
      const TransferMappings& GetTransferMappings () const { return transferMappings; }
      /// Add multiple transfer mappings
      void AddTransferMappings (const TransferMappings& mappings);
      
      void SetParameterFrequency (const uc::String& param, int freq)
      { paramFreqMap[param] = freq; }
      typedef std::unordered_map<uc::String, int> ParameterFrequencyMap;
      const ParameterFrequencyMap& GetParameterFrequencies() const
      { return paramFreqMap; }
      /// Add multiple parameter frequencies
      void AddParameterFrequencies (const ParameterFrequencyMap& freqs);
    private:
      TransferMappings transferMappings;
      ParameterFrequencyMap paramFreqMap;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAMFUNCTION_H__