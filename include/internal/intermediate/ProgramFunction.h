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

#include <unicode/unistr.h>

#include "intermediate/Sequence.h"
#include "parser/SemanticsHandler.h"

#include "forwarddecl.h"

namespace s1
{
  namespace intermediate
  {
    class ProgramFunction
    {
      UnicodeString originalIdentifier;
      UnicodeString identifier;
      parser::SemanticsHandler::Scope::FunctionFormalParameters params;
      SequencePtr body;
      bool isEntryFunction;
    public:
      ProgramFunction (const UnicodeString& originalIdentifier,
		       const UnicodeString& identifier,
		       const parser::SemanticsHandler::Scope::FunctionFormalParameters& params,
		       const SequencePtr& body,
		       bool isEntryFunction);

      const UnicodeString& GetOriginalIdentifier() const
      { return originalIdentifier; }
      const UnicodeString& GetIdentifier() const
      { return identifier; }
      const parser::SemanticsHandler::Scope::FunctionFormalParameters& GetParams() const
      { return params; }
      SequencePtr GetBody() const
      { return body; }
      bool IsEntryFunction() const
      { return isEntryFunction; }
      
      void SetTransferMapping (const UnicodeString& transferVal,
			       const RegisterPtr& programReg);
      typedef std::pair<UnicodeString, RegisterPtr> TransferMappingPair;
      typedef std::vector<TransferMappingPair> TransferMappings;
      const TransferMappings& GetTransferMappings () const { return transferMappings; }
      
      void SetParameterFrequency (const UnicodeString& param, int freq)
      { paramFreqMap[param] = freq; }
      typedef boost::unordered_map<UnicodeString, int> ParameterFrequencyMap;
      const ParameterFrequencyMap& GetParameterFrequencies() const
      { return paramFreqMap; }
    private:
      TransferMappings transferMappings;
      ParameterFrequencyMap paramFreqMap;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAMFUNCTION_H__