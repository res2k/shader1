#include "base/common.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/ProgramFunction.h"

#include "BlockImpl.h"

namespace s1
{
  namespace intermediate
  {
    ProgramFunction::ProgramFunction (const UnicodeString& originalIdentifier,
				      const UnicodeString& identifier,
				      const parser::SemanticsHandler::Scope::FunctionFormalParameters& params,
				      const SequencePtr& body,
				      bool isEntryFunction)
     : originalIdentifier (originalIdentifier) ,identifier (identifier),
       params (params), body (body),
       isEntryFunction (isEntryFunction), execFreq (-1)
    {
    }

    void ProgramFunction::SetTransferMapping (const UnicodeString& transferVal,
					      const RegisterID& programReg)
    {
      transferMappings.push_back (std::make_pair (transferVal, programReg));
    }
  } // namespace intermediate
} // namespace s1

