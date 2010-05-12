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
				      const parser::SemanticsHandler::BlockPtr& block)
     : identifier (identifier), returnType (returnType), params (params),
       body (boost::static_pointer_cast<IntermediateGeneratorSemanticsHandler::BlockImpl> (block)->GetSequence ())
    {
    }

  } // namespace intermediate
} // namespace s1

