#ifndef __INTERMEDIATE_PROGRAMFUNCTION_H__
#define __INTERMEDIATE_PROGRAMFUNCTION_H__

#include <unicode/unistr.h>

#include "parser/SemanticsHandler.h"

#include "forwarddecl.h"
#include "RegisterID.h"

namespace s1
{
  namespace intermediate
  {
    class ProgramFunction
    {
      UnicodeString identifier;
      parser::SemanticsHandler::TypePtr returnType;
      parser::SemanticsHandler::Scope::FunctionFormalParameters params;
      SequencePtr body;
      bool isEntryFunction;
      int execFreq;
    public:
      ProgramFunction (const UnicodeString& identifier,
		       const parser::SemanticsHandler::TypePtr& returnType,
		       const parser::SemanticsHandler::Scope::FunctionFormalParameters& params,
		       const SequencePtr& body,
		       bool isEntryFunction);

      const UnicodeString& GetIdentifier() const
      { return identifier; }
      const parser::SemanticsHandler::TypePtr& GetReturnType() const
      { return returnType; }
      const parser::SemanticsHandler::Scope::FunctionFormalParameters& GetParams() const
      { return params; }
      SequencePtr GetBody() const
      { return body; }
      bool IsEntryFunction() const
      { return isEntryFunction; }
      
      void SetExecutionFrequence (int freq) { execFreq = freq; }
      int GetExecutionFrequence () const { return execFreq; }
      
      void SetTransferMapping (const UnicodeString& transferVal,
			       const RegisterID& programReg);
      typedef std::vector<std::pair<UnicodeString, RegisterID> > TransferMappings;
      const TransferMappings& GetTransferMappings () const { return transferMappings; }
    private:
      TransferMappings transferMappings;
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_PROGRAMFUNCTION_H__