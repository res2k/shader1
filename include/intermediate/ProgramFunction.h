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
      typedef std::tr1::unordered_map<UnicodeString, int> ParameterFrequencyMap;
      const ParameterFrequencyMap& GetParameterFrequencies() const
      { return paramFreqMap; }
    private:
      TransferMappings transferMappings;
      ParameterFrequencyMap paramFreqMap;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAMFUNCTION_H__