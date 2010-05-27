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
				      const parser::SemanticsHandler::BlockPtr& block,
				      bool isEntryFunction)
     : identifier (identifier), returnType (returnType), params (params), isEntryFunction (isEntryFunction)
    {
      boost::shared_ptr<IntermediateGeneratorSemanticsHandler::BlockImpl> blockImpl (
	boost::static_pointer_cast<IntermediateGeneratorSemanticsHandler::BlockImpl> (block));
      blockImpl->FinishBlock();
      body = blockImpl->GetSequence ();
    }

  } // namespace intermediate
} // namespace s1

