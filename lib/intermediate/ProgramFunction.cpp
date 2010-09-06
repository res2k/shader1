#include "base/common.h"

#include "intermediate/IntermediateGeneratorSemanticsHandler.h"
#include "intermediate/ProgramFunction.h"

#include "BlockImpl.h"

namespace s1
{
  namespace intermediate
  {
    ProgramFunction::ProgramFunction (const UnicodeString& identifier,
				      const parser::SemanticsHandler::TypePtr& returnType,
				      const parser::SemanticsHandler::Scope::FunctionFormalParameters& params,
				      const SequencePtr& body,
				      bool isEntryFunction)
     : identifier (identifier), returnType (returnType), params (params), body (body), isEntryFunction (isEntryFunction),
       execFreq (-1)
    {
      /*boost::shared_ptr<IntermediateGeneratorSemanticsHandler::BlockImpl> blockImpl (
	boost::static_pointer_cast<IntermediateGeneratorSemanticsHandler::BlockImpl> (block));
      blockImpl->FinishBlock();
      body = blockImpl->GetSequence ();*/
    }

    void ProgramFunction::SetTransferMapping (const UnicodeString& transferVal,
					      const RegisterID& programReg)
    {
      transferMappings.push_back (std::make_pair (transferVal, programReg));
    }
  } // namespace intermediate
} // namespace s1

